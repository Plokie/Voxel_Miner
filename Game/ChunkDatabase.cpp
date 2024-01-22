#include "ChunkDatabase.h"

#include "WorldGen.h"
#include "../Engine/Engine.h"
#include "Inventory.h"
#include "PlayerData.h"
#include "BlockData.h"

ChunkDatabase* ChunkDatabase::_Instance = nullptr;

//https://stackoverflow.com/questions/6691555/converting-narrow-string-to-wide-string
inline std::wstring convert(const std::string& as)
{
	// deal with trivial case of empty string
	if(as.empty())    return std::wstring();

	// determine required length of new string
	size_t reqLength = ::MultiByteToWideChar(CP_UTF8, 0, as.c_str(), (int)as.length(), 0, 0);

	// construct new string of required length
	std::wstring ret(reqLength, L'\0');

	// convert old string to new string
	::MultiByteToWideChar(CP_UTF8, 0, as.c_str(), (int)as.length(), &ret[0], (int)ret.length());

	// return new string ( compiler should optimize this away )
	return ret;
}

string GetChunkFileName(const Vector3Int& chunkIndex) {
	return "chunk" + to_string(chunkIndex.x) + "_" + to_string(chunkIndex.y) + "_" + to_string(chunkIndex.z) + ".dat";
}

void ChunkDatabase::SaveChunkIntoFile(const Vector3Int& chunkIndex, BlockID chunkDataArray[CHUNKSIZE_X][CHUNKSIZE_Y][CHUNKSIZE_Z])
{

	const string& filePath = "Worlds/" + worldName + "/" + GetChunkFileName(chunkIndex);


	CreateDirectory(L"Worlds", NULL);
	CreateDirectory(convert("Worlds/" + worldName).c_str(), NULL);
	unique_lock<mutex> lock(fileAccessMutex); // you can only read/write one file at a time
	int debugIndex = 0;

	ofstream fileStream(filePath, std::ios::binary);
	if(fileStream.is_open()) {
		for(int x = 0; x < CHUNKSIZE_X; x++) {
			for(int y = 0; y < CHUNKSIZE_Y; y++) {
				for(int z = 0; z < CHUNKSIZE_Z; z++) {
					if(chunkDataArray[x][y][z] == ERR) {
						//assert(false);
					}

					fileStream << (char)chunkDataArray[x][y][z];
					debugIndex++;
				}
			}
		}

		if(debugIndex < CHUNKSIZE_X * CHUNKSIZE_Y * CHUNKSIZE_Z)
			assert(false);

		fileStream.close();
	}
	//else {
	//	return;
	//}

}

void ChunkDatabase::LoadChunkFromFile(const Vector3Int& chunkIndex, BlockID chunkDataArray[CHUNKSIZE_X][CHUNKSIZE_Y][CHUNKSIZE_Z]) {
	BlockID chunk1D[CHUNKSIZE_X * CHUNKSIZE_Y * CHUNKSIZE_Z] = {};
	//BlockID* chunk1D = new BlockID(CHUNKSIZE_X * CHUNKSIZE_Y * CHUNKSIZE_Z);

	unique_lock<mutex> lock(fileAccessMutex); // Okay so you can only read one file at a time

	char currentChar;
	const string& filePath = "Worlds/" + worldName + "/" + GetChunkFileName(chunkIndex);
	ifstream chunkFile(filePath, std::ios::binary);
	int index = 0;

	int x=0, y=0, z=0;

	if(!chunkFile.is_open()) {
		assert(false);
		return;
	}

	// Doing the 1D then to 3D array thing might be slower but its easier
	// i couldnt get coordinate conversion working for some reason (im honestly not sure why)
	// honestly it doesnt make THAT big of a difference
	while(chunkFile.read(&currentChar, 1)) { 
		chunk1D[index] = static_cast<BlockID>(currentChar);
		index++;
	}

	index = 0;
	for(int x = 0; x < CHUNKSIZE_X; x++) {
		for(int y = 0; y < CHUNKSIZE_Y; y++) {
			for(int z = 0; z < CHUNKSIZE_Z; z++) {
				if(chunk1D[index] == ERR) {
					//assert(false);
				}

				chunkDataArray[x][y][z] = chunk1D[index];
				index++;
			}
		}
	}
}

void ChunkDatabase::SaveChunkBlockDataDataIntoFile(const Vector3Int& chunkIndex, map<tuple<int, int, int>, BlockData*>& blockDataData) {
	const string& filePath = "Worlds/" + worldName + "/blockData_" + GetChunkFileName(chunkIndex);

	nlohmann::json json = {};

	for(const auto& kvp : blockDataData) {
		int arr[3] = { get<0>(kvp.first), get<1>(kvp.first), get<2>(kvp.first) };
		nlohmann::json arrJson = nlohmann::json::array({arr[0], arr[1], arr[2]});
		string key = arrJson.dump();

		if(kvp.second == nullptr) {
			json[key] = 0;
		}
		else {
			json[key] = kvp.second->Serialize();
		}
	}

	ofstream file(filePath);
	file << json.dump(2);
	file.close();
}

void ChunkDatabase::LoadChunkBlockDataDataFromFile(const Vector3Int& chunkIndex, map<tuple<int, int, int>, BlockData*>& blockDataData) {
	const string& filePath = "Worlds/" + worldName + "/blockData_" + GetChunkFileName(chunkIndex);

	ifstream f(filePath);
	if(!f) {
		f.close();
		assert(false);
		return;
	}

	if(f.good()) {
		ostringstream stringBuff;
		stringBuff << f.rdbuf();

		string jsonString = stringBuff.str();

		nlohmann::json json = nlohmann::json::parse(jsonString);

		for(auto& item : json.items()) {
			nlohmann::json keyJson = nlohmann::json::parse(item.key());
			//nlohmann::json valueJson = nlohmann::json::parse(item.value())

			int arr[3] = { keyJson[0], keyJson[1], keyJson[2] };
			tuple<int, int, int> tpl = { arr[0], arr[1], arr[2] };
			//nlohmann::json::array()
			//item.key()

			BlockData* blockData = nullptr;
			if(item.value() != 0) {
				blockData = new BlockData();
				blockData->Deserialize(item.value());
			}

			blockDataData[tpl] = blockData;
		}
	}
	f.close();
}

void ChunkDatabase::TryLoadChunkHash()
{
	if(!hasLoadedChunkHash) {
		const string& worldDatPath = "Worlds/" + worldName + "/world.dat";
		ifstream f(worldDatPath);
		if(!f) {
			f.close();
			return; // Error reading file
		}

		if(f.good()) { // if file exists
			ostringstream stringBuff;
			stringBuff << f.rdbuf();

			string test = stringBuff.str();


			nlohmann::json worldNameJson = nlohmann::json::parse(test);
			///////
			vector<tuple<int, int, int>> loadedHash = worldNameJson["chunkHash"];
			for(tuple<int, int, int> chunkIndex : loadedHash) {
				chunkHash[chunkIndex] = nullptr;
			}

			if(worldNameJson.find("chunksWithBlockData")!=worldNameJson.end()) {
				vector<tuple<int, int, int>> loadedChunksWithBlockData = worldNameJson["chunksWithBlockData"];
				for(auto& chunkIndex : loadedChunksWithBlockData) {
					chunksWithBlockData[chunkIndex] = nullptr;
				}
			}
		}
		else { // if file doesnt exist
			SaveWorldData(); // create world data file
		}
		f.close();

		hasLoadedChunkHash = true;
	}
}

void ChunkDatabase::SaveWorldData() {
	if(this == nullptr) return;
	const string& worldDatPath = "Worlds/" + worldName + "/world.dat";
	ofstream file(worldDatPath);

	CreateDirectory(L"Worlds", NULL);
	CreateDirectory(convert("Worlds/" + worldName).c_str(), NULL);

	vector<tuple<int, int, int>> chunkHashVector = {};
	for(pair<tuple<int, int, int>, bool> pair : chunkHash) {
		chunkHashVector.push_back(pair.first);
	}

	vector<tuple<int, int, int>> chunksWithBlockDataVector = {};
	for(auto& pair : chunksWithBlockData) {
		chunksWithBlockDataVector.push_back(pair.first);
	}

	//Vector3 playerPos = Engine::Get()->GetScene("game")->GetObject3D("PlayerController")->transform.position;
	//Inventory* inv = Engine::Get()->GetScene("game")->GetObject3D<Inventory>("Inventory");
	PlayerData* playerData = Engine::Get()->GetScene("game")->GetObject3D<PlayerData>("PlayerData");

	vector<nlohmann::json> playerDatas = {};
	playerDatas.push_back(playerData->Serialize());

	nlohmann::json newJson = {
		{"seed", WorldGen::GetSeed()},
		{"worldName", worldName.c_str()},
		{"chunkHash", chunkHashVector},
		{"chunksWithBlockData", chunksWithBlockDataVector},
		//{"playerPos", {playerPos.x, playerPos.y, playerPos.z}},
		//{"inventory", inv->Serialize()}
		{"playerData", playerDatas}
	};
	file << newJson.dump(2);
	file.close();
}

void ChunkDatabase::LoadWorldData()
{
	const string& worldDatPath = "Worlds/" + worldName + "/world.dat";
	ifstream f(worldDatPath);
	if(!f) {
		f.close();
		return; // Error reading file
	}

	//Inventory* inv = Engine::Get()->GetScene("game")->GetObject3D<Inventory>("Inventory");
	PlayerData* playerData = Engine::Get()->GetScene("game")->GetObject3D<PlayerData>("PlayerData");

	if(f.good()) { // if file exists
		ostringstream stringBuff;
		stringBuff << f.rdbuf();

		string test = stringBuff.str();


		nlohmann::json json = nlohmann::json::parse(test);
		///////
		WorldGen::SetSeed(json["seed"]);
		this->worldName = json["worldName"];

		//Engine::Get()->GetScene("game")->GetObject3D("PlayerController")->transform.position = { json["playerPos"][0], json["playerPos"][1], json["playerPos"][2] };
		//inv->Deserialize(json["inventory"]);

		playerData->Deserialize(json["playerData"][0]);

		//playerData->GetInventory()->Deserialize();
	}
	else { // if file doesnt exist
		//inv->LoadDefaultItems();
		//if(!playerData->IsCreative())
			playerData->GetInventory()->LoadDefaultItems();
		SaveWorldData(); // create world data file
	}
	f.close();
}

#define AUTOSAVE_TIME 180.f //autosave every 3 minutes (180 seconds)

void ChunkDatabase::Update(float dt)
{
	autosaveTimer += dt;

	if(autosaveTimer > AUTOSAVE_TIME) {
		autosaveTimer = 0.f;
		Autosave();
	}
}

void ChunkDatabase::SaveChunks()
{
	if(this == nullptr)return;
	AcquireSRWLockExclusive(&chunkHashMutex);
	for(const pair<tuple<int, int, int>, Chunk*>& pair : chunkHash) {
		if(pair.second != nullptr) {
			SaveChunkIntoFile(pair.first, pair.second->blockData);

			if(pair.second->blockDataData.size() > 0) {
				chunksWithBlockData[pair.first] = pair.second;

				// save block data data into file
				SaveChunkBlockDataDataIntoFile(pair.first, pair.second->blockDataData);
			}
		}
	}
	ReleaseSRWLockExclusive(&chunkHashMutex);
}

void ChunkDatabase::UnloadChunk(const Vector3Int& chunkIndex)
{
	AcquireSRWLockExclusive(&chunkHashMutex);

	auto it = chunkHash.find(chunkIndex);
	if(it != chunkHash.end()) {
		Chunk*& chunk = it->second;
		if(chunk != nullptr) {
			unique_lock<std::mutex> lock(chunk->gAccessMutex);
			SaveChunkIntoFile(chunkIndex, chunk->blockData);

			chunk = nullptr;
		}
	}

	auto it2 = chunksWithBlockData.find(chunkIndex);
	if(it2 != chunksWithBlockData.end()) {
		Chunk*& chunk = it2->second;
		if(chunk != nullptr) {
			// save block data data into file
			SaveChunkBlockDataDataIntoFile(chunkIndex, chunk->blockDataData);
		}
		chunk = nullptr;
	}

	ReleaseSRWLockExclusive(&chunkHashMutex);
}

ChunkDatabase::ChunkDatabase()
{
}

ChunkDatabase::~ChunkDatabase()
{
}

void ChunkDatabase::Close() {
	SaveChunks();
	SaveWorldData();
}

void ChunkDatabase::Autosave()
{
	SaveChunks();
	SaveWorldData();
}

void ChunkDatabase::Release()
{
	delete _Instance;
	_Instance = nullptr;
}

ChunkDatabase* ChunkDatabase::Get()
{
	return _Instance;
}

void ChunkDatabase::SetWorldName(const string& name)
{
	Init(name);
}

const string& ChunkDatabase::GetWorldName() const
{
	return worldName;
}

void ChunkDatabase::Init(const string& worldName)
{
	//if(_Instance != nullptr) delete _Instance;
	if(_Instance != nullptr) return;

	_Instance = new ChunkDatabase();

	_Instance->worldName = worldName;

	InitializeSRWLock(&_Instance->chunkHashMutex);

	Engine::Get()->GetScene("game")->GetObject3D("PlayerController")->transform.position.y = WorldGen::SampleWorldHeight(0, 0) + 5.f;

	_Instance->TryLoadChunkHash();
	_Instance->LoadWorldData();

	Graphics::Get()->camera.transform.position = Engine::Get()->GetScene("game")->GetObject3D("PlayerController")->transform.position;
}

bool ChunkDatabase::DoesDataExistForChunk(const Vector3Int& chunkIndex)
{
	TryLoadChunkHash();
	return chunkHash.count(chunkIndex) > 0;
}

void ChunkDatabase::LoadChunkDataInto(const Vector3Int& chunkIndex, Chunk* chunk)
{
	LoadChunkFromFile(chunkIndex, chunk->blockData);
	chunkHash[chunkIndex] = chunk;

	if(chunksWithBlockData.find(chunkIndex) != chunksWithBlockData.end()) {
		LoadChunkBlockDataDataFromFile(chunkIndex, chunk->blockDataData);
	}
}

void ChunkDatabase::SaveChunkData(const Vector3Int& chunkIndex, Chunk* chunk)
{
	chunkHash[chunkIndex] = chunk;
	if(chunk->blockDataData.size() > 0) {
		chunksWithBlockData[chunkIndex] = chunk;
	}

	TryLoadChunkHash();


	//SaveChunkIntoFile(chunkIndex, chunkDataArray);
}

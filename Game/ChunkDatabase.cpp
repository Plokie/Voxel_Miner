#include "ChunkDatabase.h"

#include "WorldGen.h"
#include "../Engine/Engine.h"

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
						assert(false);
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

	//char byte;
	//while(chunkFile.read(&byte, 1)) {
	//	BlockID block = static_cast<BlockID>(byte);
	//	if(block == ERR || block > 39u)
	//		assert(false);

	//	x = (index % CHUNKSIZE_X);
	//	z = (index / (CHUNKSIZE_X * CHUNKSIZE_X));
	//	y = (index % (CHUNKSIZE_X * CHUNKSIZE_X)) / CHUNKSIZE_X;

	//	if(x >= CHUNKSIZE_X || y >= CHUNKSIZE_Y || z >= CHUNKSIZE_Z)
	//		assert(false);

	//	//chunk1D[index] = block;
	//	chunkDataArray[x][y][z] = block; 
	//	if(chunkDataArray[x][y][z] == ERR) { //?
	//		assert(false);
	//	}
	//	index++;
	//}


	//if(x != (CHUNKSIZE_X - 1) || y != (CHUNKSIZE_Y - 1) || z != (CHUNKSIZE_Z - 1))
	//	assert(false);

	//chunkFile.close();

	while(chunkFile.read(&currentChar, 1)) {
		chunk1D[index] = static_cast<BlockID>(currentChar);
		index++;
	}

	index = 0;
	for(int x = 0; x < CHUNKSIZE_X; x++) {
		for(int y = 0; y < CHUNKSIZE_Y; y++) {
			for(int z = 0; z < CHUNKSIZE_Z; z++) {
				if(chunk1D[index] == ERR) {
					assert(false);
				}

				chunkDataArray[x][y][z] = chunk1D[index];
				index++;
			}
		}
	}
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

	Vector3 playerPos = Engine::Get()->GetScene("game")->GetObject3D("PlayerController")->transform.position;

	nlohmann::json newJson = {
		{"seed", WorldGen::GetSeed()},
		{"worldName", worldName.c_str()},
		{"chunkHash", chunkHashVector},
		{"playerPos", {playerPos.x, playerPos.y, playerPos.z}}
	};
	file << newJson.dump(2);
}

void ChunkDatabase::LoadWorldData()
{
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


		nlohmann::json json = nlohmann::json::parse(test);
		///////
		WorldGen::SetSeed(json["seed"]);
		this->worldName = json["worldName"];

		//float arr[3] = json["playerPos"];
		//float x = json["playerPos"][0];
		//float y = json["playerPos"][1];
		//float z = json["playerPos"][2];

		Engine::Get()->GetScene("game")->GetObject3D("PlayerController")->transform.position = { json["playerPos"][0], json["playerPos"][1], json["playerPos"][2] };
	}
	else { // if file doesnt exist
		SaveWorldData(); // create world data file
	}
	f.close();
}

void ChunkDatabase::SaveChunks()
{
	if(this == nullptr)return;
	AcquireSRWLockExclusive(&chunkHashMutex);
	for(const pair<tuple<int, int, int>, Chunk*>& pair : chunkHash) {
		if(pair.second != nullptr) {
			SaveChunkIntoFile(pair.first, pair.second->blockData);
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
}

void ChunkDatabase::SaveChunkData(const Vector3Int& chunkIndex, Chunk* chunk)
{
	chunkHash[chunkIndex] = chunk;
	TryLoadChunkHash();

	//SaveChunkIntoFile(chunkIndex, chunkDataArray);
}

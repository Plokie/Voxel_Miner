#include "ChunkDatabase.h"

ChunkDatabase* ChunkDatabase::_Instance = nullptr; 

//https://stackoverflow.com/questions/6691555/converting-narrow-string-to-wide-string
inline std::wstring convert(const std::string& as)
{
	// deal with trivial case of empty string
	if (as.empty())    return std::wstring();

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

void ChunkDatabase::SaveChunkIntoFile(const Vector3Int& chunkIndex, USHORT chunkDataArray[CHUNKSIZE_X][CHUNKSIZE_Y][CHUNKSIZE_Z])
{
	
	const string& filePath = "Worlds/" + worldName + "/" + GetChunkFileName(chunkIndex);


	CreateDirectory(L"Worlds", NULL);
	CreateDirectory(convert("Worlds/"+worldName).c_str(), NULL);

	ofstream fileStream(filePath);
	if (fileStream.is_open()) {
		for(int x = 0; x < CHUNKSIZE_X; x++) {
			for(int y = 0; y < CHUNKSIZE_Y; y++) {
				for(int z = 0; z < CHUNKSIZE_Z; z++) {
					fileStream << (char)chunkDataArray[x][y][z];
				}
			}
		}

		fileStream.close();
	}
	//else {
	//	return;
	//}
	
}

void ChunkDatabase::LoadChunkFromFile(const Vector3Int& chunkIndex, USHORT chunkDataArray[CHUNKSIZE_X][CHUNKSIZE_Y][CHUNKSIZE_Z]){
	USHORT chunk1D[CHUNKSIZE_X * CHUNKSIZE_Y * CHUNKSIZE_Z] = {};
	char currentChar;
	const string& filePath = "Worlds/" + worldName + "/" + GetChunkFileName(chunkIndex);
	fstream chunkFile(filePath, fstream::in);
	int index = 0;
	while(chunkFile >> noskipws >> currentChar) {
		chunk1D[index] = static_cast<USHORT>(currentChar);
		index++;
	}
	
	index = 0;
	for(int x = 0; x < CHUNKSIZE_X; x++) {
		for(int y = 0; y < CHUNKSIZE_Y; y++) {
			for(int z = 0; z < CHUNKSIZE_Z; z++) {
				chunkDataArray[x][y][z] = chunk1D[index];
				index++;
			}
		}
	}
}

void ChunkDatabase::TryLoadChunkHash()
{
	if (!hasLoadedChunkHash) {
		const string& worldDatPath = "Worlds/" + worldName + "/world.dat";
		ifstream f(worldDatPath);
		if(!f) {
			f.close();
			return; // Error reading file
		}

		if (f.good()) { // if file exists
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
	const string& worldDatPath = "Worlds/" + worldName + "/world.dat";
	ofstream file(worldDatPath);
	
	CreateDirectory(L"Worlds", NULL);
	CreateDirectory(convert("Worlds/" + worldName).c_str(), NULL);

	vector<tuple<int, int, int>> chunkHashVector = {};
	for (pair<tuple<int, int, int>, bool> pair : chunkHash) {
		chunkHashVector.push_back(pair.first);
	}

	nlohmann::json newJson = {
		{"seed", 69},
		{"worldName", worldName.c_str()},
		{"chunkHash", chunkHashVector},
		{"playerPos", {0, 0, 0}}
	};
	file << newJson.dump();
}

void ChunkDatabase::SaveChunks()
{
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
	if(chunkHash.count(chunkIndex)) {
		if(chunkHash[chunkIndex] != nullptr) {
			SaveChunkIntoFile(chunkIndex, chunkHash[chunkIndex]->blockData);

			chunkHash[chunkIndex] = nullptr;
		}
	}
	ReleaseSRWLockExclusive(&chunkHashMutex);
}

ChunkDatabase::ChunkDatabase()
{
}

ChunkDatabase* ChunkDatabase::Get()
{
	return _Instance;
}

void ChunkDatabase::Init()
{
	if (_Instance != nullptr) delete _Instance;
	_Instance = new ChunkDatabase();

	InitializeSRWLock(&_Instance->chunkHashMutex);

	_Instance->TryLoadChunkHash();
}

bool ChunkDatabase::DoesDataExistForChunk(const Vector3Int& chunkIndex)
{
	TryLoadChunkHash();
	return chunkHash.count(chunkIndex)>0;
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
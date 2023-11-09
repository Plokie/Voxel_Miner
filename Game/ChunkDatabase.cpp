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


void ChunkDatabase::SaveChunkIntoFile(const string& worldName, const Vector3Int& chunkIndex, USHORT* chunkDataArray)
{

	const string& filePath = worldName + "/chunk" + to_string(chunkIndex.x) + "_" + to_string(chunkIndex.y) + "_" + to_string(chunkIndex.z) + ".dat";

	//CreateDirectory(worldName., NULL);
	CreateDirectory(convert(worldName).c_str(), NULL);

	ofstream fileStream(filePath);
	if (fileStream.is_open()) {
		fileStream << "This is a test!";
		fileStream.close();
	}
	else {
		return;
	}
}

void ChunkDatabase::LoadChunkFromFile(const string& worldName, const Vector3Int& chunkIndex, USHORT* chunkDataArray)
{
}

void ChunkDatabase::TryLoadChunkHash(const string& worldName)
{
	if (!hasLoadedChunkHash) {
		const string& worldDatPath = worldName + "/world.dat";
		ifstream f(worldDatPath);


		if (f.good()) {
			ofstream file(worldDatPath); // Creates the file if it doesn't already exist
			nlohmann::json worldNameJson = nlohmann::json::parse(file.rdbuf());
			///////


			file.close();
		}
		else {
			SaveWorldData(worldName);
		}

		hasLoadedChunkHash = true;
	}
}

void ChunkDatabase::SaveWorldData(const string& worldName) {
	const string& worldDatPath = worldName + "/world.dat";
	ofstream file(worldDatPath);
	//for ()
	vector<tuple<int, int, int>> chunkHashVector = {};
	for (pair<tuple<int, int, int>, bool> pair : chunkHash) {
		chunkHashVector.push_back(pair.first);
	}

	nlohmann::json newJson = {
		{"worldName", worldName.c_str()},
		{"chunkHash", chunkHashVector}
	};
	file << newJson.dump();
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
}

bool ChunkDatabase::DoesDataExistForChunk(const string& worldName, const Vector3Int& chunkIndex)
{
	return false;
}

void ChunkDatabase::LoadChunkDataInto(const string& worldName, const Vector3Int& chunkIndex, USHORT* chunkDataArray)
{
}

void ChunkDatabase::SaveChunkData(const string& worldName, const Vector3Int& chunkIndex, USHORT* chunkDataArray)
{
	chunkHash[chunkIndex] = true;
	TryLoadChunkHash(worldName);

	SaveChunkIntoFile(worldName, chunkIndex, chunkDataArray);
}

#include "TerrainFeature.h"

#include "Chunk.h"

void TerrainFeature::PutBlock(const Vector3Int& p, const BlockID block)
{
	PutBlock(p.x, p.y, p.z, block);
}

void TerrainFeature::PutBlock(const int x, const int y, const int z, const BlockID block)
{
	_localStructure[{x, y, z}] = block;

	if(_minBoundX < x) _minBoundX = x;
	else if(_maxBoundX > x) _maxBoundX = x;

	if(_minBoundY < y) _minBoundY = y;
	else if(_maxBoundY > y) _maxBoundY = y;

	if(_minBoundZ < z) _minBoundZ = z;
	else if(_maxBoundZ > z) _maxBoundZ = z;
}

void TerrainFeature::_GenerateFunc()
{
	for(int i = 0; i < 5; i++) {
		PutBlock(0, i+1, 0, OAK_LOG);
	}
}

TerrainFeature::TerrainFeature(const Vector3Int& origin)
{
	this->origin = origin;
	_GenerateFunc();
}

void TerrainFeature::Generate(const Vector3Int& origin, Chunk* chunk)
{
	this->origin = origin;
	//this->parentChunk = chunk;

	_GenerateFunc();
}

BlockID TerrainFeature::PopBlockWorld(const float x, const float y, const float z, bool* outWasLastBlock)
{
	tuple<int,int,int> local = Vector3Int(x, y, z) - origin;
	if(_localStructure.find(local) != _localStructure.end()) {
		BlockID block = _localStructure[local];
		_localStructure.erase(local);

		*outWasLastBlock = _localStructure.empty();

		return block;
	}

	return ERR;
}

bool TerrainFeature::IsPosWithinBounds(const Vector3Int& pos)
{
	return IsPosWithinBounds(pos.x,pos.y,pos.z);
}

bool TerrainFeature::IsPosWithinBounds(const float x, const float y, const float z)
{
	return(
		_minBoundX >= x && _maxBoundX <= x&&
		_minBoundY >= y && _maxBoundY <= y&&
		_minBoundZ >= z && _maxBoundZ <= z
	);

}

bool TerrainFeature::IsWorldPosWithinBounds(Vector3Int pos)
{
	pos = pos - origin;
	return IsPosWithinBounds(pos);
}

bool TerrainFeature::IsChunkIndexWithinBounds(const Vector3Int& chunkIndex)
{
	const Vector3Int _minBound = Vector3Int(_minBoundX, _minBoundY, _minBoundZ) + origin;
	const Vector3Int _maxBound = Vector3Int(_maxBoundX, _maxBoundY, _maxBoundZ) + origin;
	
	const Vector3Int chunkMin = Vector3Int(CHUNKSIZE_X, CHUNKSIZE_Y, CHUNKSIZE_Z) ^ chunkIndex;
	const Vector3Int chunkMax = Vector3Int(CHUNKSIZE_X, CHUNKSIZE_Y, CHUNKSIZE_Z) + chunkMin;

	return !(
		_maxBound.x < chunkMin.x ||
		_maxBound.y < chunkMin.y ||
		_minBound.x > chunkMax.x ||
		_minBound.y > chunkMax.y
	);
}

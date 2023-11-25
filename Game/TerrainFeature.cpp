#include "TerrainFeature.h"

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
	PutBlock(0, 0, 0, OAK_LOG);
}

void TerrainFeature::Generate(Vector3Int origin, Chunk* chunk)
{
	this->origin = origin;
	this->parentChunk = chunk;

	_GenerateFunc();
}

bool TerrainFeature::IsPosWithinBounds(const Vector3Int& pos)
{
	return IsPosWithinBounds(pos.x,pos.y,pos.z);
}

bool TerrainFeature::IsPosWithinBounds(const float x, const float y, const float z)
{
	return(
		_minBoundX > x && _maxBoundX < x&&
		_minBoundY > y && _maxBoundY < y&&
		_minBoundZ > z && _maxBoundZ < z
	);

}

bool TerrainFeature::IsChunkIndexWithinBounds(const Vector3Int& chunkIndex)
{
	const Vector3Int _minBound = Vector3Int(_minBoundX, _minBoundY, _minBoundZ) + origin;
	const Vector3Int _maxBound = Vector3Int(_maxBoundX, _maxBoundY, _maxBoundZ) + origin;
	
	const Vector3Int chunkMin = Vector3Int(16, 16, 16) ^ chunkIndex;
	const Vector3Int chunkMax = Vector3Int(16, 16, 16) + chunkMin;

	return !(
		_maxBound.x < chunkMin.x ||
		_maxBound.y < chunkMin.y ||
		_minBound.x > chunkMax.x ||
		_minBound.y > chunkMax.y
	);
	
	//const AABB chunkBox = AABB(Vector3Int(16, 16, 16) + chunkIndex, Vector3Int(8, 8, 8));
	//
	//const Vector3 halfSize = (_maxBound - _minBound) / 2.f;
	//const Vector3 center = _minBound + halfSize;
	//const AABB featureBox = AABB(center, halfSize);

	//return chunkBox.GetMinMax

	/*
	target->min = target->center - target->halfSize;
	target->max = target->center + target->halfSize;

	a = b - c;
	a + c = b

	a + c = b
	*/




	//return false;
}

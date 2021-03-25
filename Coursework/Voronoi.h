#pragma once

#include "BaseGenerationMode.h"
#include <vector>

//	Divide the terrain up into different regions.
class Voronoi : public BaseGenerationMode
{
private:
	struct Point
	{
		int x, y;
	};
public:
	~Voronoi();
	void init(int terrainWidth, int terrainHeight);
	void generateHeightMap(HeightMap* _heightMap, int terrainWidth, int terrainHeight);

private:
	void determineRegions(int numberOfRegions);
	int getPoint(int i);
	int getNearestRegion(std::vector<float> distances);
private:
	int* _points;
	int _terrainWidth, _terrainHeight;
	std::vector<Point> _regions;



};
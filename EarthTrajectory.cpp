#include "EarthTrajectory.h"

EarthTrajectory::EarthTrajectory()
{
}

void EarthTrajectory::push_back(const EarthPoint4D & point)
{
	data.push_back(point);
}

EarthPoint4D EarthTrajectory::operator[](unsigned int i)
{
	return data[i];
}

unsigned int EarthTrajectory::size()
{
	return data.size();
}

vector<EarthPoint4D> EarthTrajectory::interpolateAtHeight(float height)
{
	vector<EarthPoint4D> result;
	result.push_back(data[data.size()-1]);
	return result;
}

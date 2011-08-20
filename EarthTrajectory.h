#ifndef EARTHTRAJECTORY_H
#define EARTHTRAJECTORY_H

#include "BasicDataTypes.h"
#include <vector>
using std::vector;

class EarthTrajectory
{
public:
    EarthTrajectory();
    void push_back(const EarthPoint4D & point);
    EarthPoint4D operator[](unsigned int);
    unsigned int size();
    vector<EarthPoint4D> interpolateAtHeight(float height);

protected:
    vector<EarthPoint4D> data;
};

#endif // EARTHTRAJECTORY_H

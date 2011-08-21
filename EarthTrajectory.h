#ifndef EARTHTRAJECTORY_H
#define EARTHTRAJECTORY_H

#include "BasicDataTypes.h"
#include <string>
#include <vector>
using std::vector;
using std::string;

class EarthTrajectory
{
public:
    EarthTrajectory();
    void readFromFile(const string& filename);
    void push_back(const EarthPoint4D & point);
    EarthPoint4D operator[](unsigned int);
    unsigned int size();
    vector<EarthPoint4D> interpolateAtHeight(float height);

protected:
    vector<EarthPoint4D> data;
};

#endif // EARTHTRAJECTORY_H

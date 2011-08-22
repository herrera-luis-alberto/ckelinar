#ifndef EARTHTRAJECTORY_H
#define EARTHTRAJECTORY_H

#include "BasicDataTypes.h"
#include <string>
#include <vector>
using std::vector;
using std::string;

/*!
  * Class EarthTrajectory handles an array of
  * EarthPoint4D.
  */
class EarthTrajectory
{
public:
    /**
      * EarthTrajectory constructor
      */
    EarthTrajectory();

    /**
      * \brief readFromFile reads the trajectory from a file.
      * This file is created at TrajectoryForecaster.
      * \param const string& filename is the name of the filename.
      */
    void readFromFile(const string& filename);

    /**
      * \brief push_back puts a new EarthPoint4D at the end of
      * the array.
      * \param EarthPoint4D is a pointer to a point in space.
      */
    void push_back(const EarthPoint4D & point);

    /**
      * \brief Return the i-th element of an array.
      * \param unsigned int is the index of the i-th
      * element of the array.
      */
    EarthPoint4D operator[](unsigned int);

    /**
      * \brief size returns the size of an EarthTrajectory.
      */
    unsigned int size();

    /**
      * \brief findLevelPoints cuts the EarthTrajectory of
      * the sonde at a certain height and returns all the
      * EarthPoint4D for that same height.
      */
    vector<EarthPoint4D> findLevelPoints(float height);

protected:

    /**
      * Vector of EarthPoint4D to create the EarthTrajectory.
      */
    vector<EarthPoint4D> data;
};

#endif // EARTHTRAJECTORY_H

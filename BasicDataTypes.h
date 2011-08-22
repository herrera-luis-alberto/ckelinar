#ifndef BASICDATATYPES_H
#define BASICDATATYPES_H

#include "boost/date_time/posix_time/posix_time.hpp"
using boost::posix_time::ptime;

/**
  * struct EarthPoint4D is a structure that contains information
  * about a certain point in space and time. An EarthPoint4D
  * contains the following data:
  *
  */
struct EarthPoint4D
{
    /**
      * latitude measures the latitude of the EarthPoint4D.
      * This variable is measured in degrees.
      */
	float latitude;
        /**
          * longitude measures the longitude of the EarthPoint4D.
          * This variable is measured in degrees.
          */
	float longitude;
        /**
          * height measures the height of the EarthPoint4D.
          * This variable is measured in [m] above sea level.
          */
        float height;
        /**
          * ptime measures the ptime of the EarthPoint4D.
          * This variable is measured in [s].
          */
        ptime time;
};

#endif // BASICDATATYPES_H

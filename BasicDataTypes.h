#ifndef BASICDATATYPES_H
#define BASICDATATYPES_H

#include "boost/date_time/posix_time/posix_time.hpp"
using boost::posix_time::ptime;

struct EarthPoint4D
{
	float latitude;
	float longitude;
	float height;
	ptime time;
};

#endif // BASICDATATYPES_H

#include "EarthTrajectory.h"
#include <fstream>
#include <sstream>
using std::ifstream;
using std::stringstream;

EarthTrajectory::EarthTrajectory()
{
}

void EarthTrajectory::readFromFile(const string& filename)
{
	ifstream input ( filename.c_str() );

	while ( input.good() )
	{
		string line;
		getline( input, line);
		stringstream linedata(line);
		int year, month, day, hour, minute, second;
		int heigh;
		float lat, lon, dummy;
		linedata>>year>>month>>day>>hour>>minute>>second;
		linedata>>dummy>>heigh>>dummy>>dummy>>dummy>>dummy>>dummy>>dummy;
		linedata>>lat>>lon;

		EarthPoint4D point;
		point.latitude = lat;
		point.longitude = lon;
		point.height = heigh;
		point.time = ptime( boost::gregorian::date( year, month, day),
				   boost::posix_time::hours(hour) +
				   boost::posix_time::minutes(minute) +
				   boost::posix_time::seconds(second) );
		data.push_back(point);
	}

	input.close();
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

vector<EarthPoint4D> EarthTrajectory::findLevelPoints(float height)
{
	vector<EarthPoint4D> result;

	if ( data.size() > 0)
		if ( data[0].height == height )
			result.push_back(data[0]);

	for ( int i=0; i<data.size()-1; i++)
	{
		if ( data[i].height < height && height <= data[i+1].height )
		{
			float alpha = (data[i+1].height-height)/(data[i+1].height-data[i].height);
			EarthPoint4D newPoint;
			newPoint.latitude = alpha*data[i].latitude+(1-alpha)*data[i+1].latitude;
			newPoint.longitude= alpha*data[i].longitude+(1-alpha)*data[i+1].longitude;
			newPoint.height= alpha*data[i].height+(1-alpha)*data[i+1].height;
			result.push_back(newPoint);

		}
	}


	return result;
}

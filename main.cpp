#include "TrajectoryForecaster.h"
#include <fstream>
using std::ofstream;
using std::endl;

int main()
{

	TrajectoryForecaster wf("wrfout_d02_160511.nc");

	EarthPoint4D init;
	init.latitude = -33.44;
	init.longitude = -70.68;
	init.time = ptime( boost::gregorian::date( 2011, 5, 18), boost::posix_time::hours(19));
	init.height = 520;

	vector<EarthPoint4D> points = wf.getTrayectory( init, 5, 100);



	ofstream out("out.mat");

	for ( int i=0; i<points.size(); i++)
	{
		out<<points[i].latitude<<" "<<points[i].longitude<<" "<<points[i].height<<endl;
	}

	out.close();



	return 0;
}

#include "TrajectoryForecaster.h"
#include "MapViewer.h"
#include <QApplication>
#include <fstream>
#include <sstream>
using std::ofstream;
using std::endl;
using std::stringstream;

int main(int argc, char *argv[])
{

	QApplication app(argc, argv);
	MapViewer map;

	TrajectoryForecaster wf("wrfout_d02_160511.nc");

	EarthPoint4D init;
	init.latitude = -33.44;
	init.longitude = -70.68;
	init.time = ptime( boost::gregorian::date( 2011, 5, 18), boost::posix_time::hours(19));
	init.height = 520;

	for ( int i=0; i<50; i++)
	{
		stringstream name;
		name<<"Projection_"<<i<<endl;
		EarthTrajectory points = wf.getTrayectory( init, 5, 100, 1);
		map.addPath(points);
		map.addMark(points[points.size()-1]);
	}
	map.show();

	return app.exec();
}

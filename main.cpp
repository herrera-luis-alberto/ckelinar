#include "TrajectoryForecaster.h"
#include "MapViewer.h"
#include <QApplication>
#include <fstream>
using std::ofstream;
using std::endl;

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

	vector<EarthPoint4D> points = wf.getTrayectory( init, 5, 100);

	ofstream out("out.mat");

	for ( int i=0; i<points.size(); i++)
	{
		out<<points[i].latitude<<" "<<points[i].longitude<<" "<<points[i].height<<endl;
	}

	out.close();

	map.setPath("Projection", points);
	map.show();

	return app.exec();
}

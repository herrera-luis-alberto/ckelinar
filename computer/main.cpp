#include "TrajectoryForecaster.h"
#include "MapViewer.h"
#include <QApplication>
#include <fstream>
#include <iostream>
#include <sstream>
#include <boost/filesystem.hpp>
using std::ofstream;
using std::cout;
using std::endl;
using std::stringstream;

namespace fs = boost::filesystem;

void loadHistorical(MapViewer &map)
{
	fs::path path( "/home/blackfire/U/sonda/data/output");

	if ( fs::is_directory(path))
	{
		for (fs::directory_iterator itr(path); itr!=fs::directory_iterator(); ++itr)
		{
			EarthTrajectory tr;
			tr.readFromFile( itr->path().c_str() );
			map.addPath(tr);
			vector<EarthPoint4D> marks = tr.findLevelPoints(5000);
			if ( marks.size()>0)
			{
				map.addMarker(marks[0]);
			}
		}
	}
}

int main(int argc, char *argv[])
{

	QApplication app(argc, argv);
	MapViewer map;

	loadHistorical(map);

	/*TrajectoryForecaster wf("wrfout_d02_160511.nc");

	EarthPoint4D init;
	init.latitude = -33.44;
	init.longitude = -70.68;
	init.time = ptime( boost::gregorian::date( 2011, 5, 18), boost::posix_time::hours(19));
	init.height = 520;

	for ( int i=0; i<0; i++)
	{
		EarthTrajectory points = wf.getTrayectory( init, 5, 100, 1);
		map.addPath(points);
		map.addMark(points[points.size()-1]);
	}*/
	map.show();

	return app.exec();
}

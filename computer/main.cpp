#include "TrajectoryForecaster.h"
#include "MainWindow.h"
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

/*void loadHistorical(MapViewer &map)
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
}*/

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
        MainWindow mainWindow;
        mainWindow.show();
	return app.exec();
}

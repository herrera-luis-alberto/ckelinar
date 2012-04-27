#include "TrajectoryForecaster.h"
#include "BasicDataTypes.h"
#include "MainWindow.h"
#include <QApplication>
#include <QMetaType>
#include <fstream>
#include <iostream>
#include <sstream>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
using std::ofstream;
using std::cout;
using std::endl;
using std::stringstream;

namespace fs = boost::filesystem;
namespace po = boost::program_options;

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
  po::options_description command_line_options_descriptions("Allowed options");
  command_line_options_descriptions.add_options()
      ("wind_forecast", po::value<string>(), "Set wind forecast WRF file.");

  po::variables_map command_line_options;
  po::store(po::parse_command_line(argc, argv,
                                   command_line_options_descriptions),
            command_line_options);
  po::notify(command_line_options);

  TrajectoryForecasterInterface *trajectory_forecaster = NULL;
  if (command_line_options.count("wind_forecast")) {
    trajectory_forecaster = new TrajectoryForecaster(
        command_line_options["wind_forecast"].as<string>());
  } else {
      trajectory_forecaster = new NullTrajectoryForecaster();
  }

  qRegisterMetaType<SondeData>("SondeData");
  QApplication app(argc, argv);
  MainWindow mainWindow(trajectory_forecaster);
  mainWindow.show();
  return app.exec();
}

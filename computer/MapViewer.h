#include "TrajectoryForecaster.h"
#include <map>
#include <QWidget>
using std::map;

class QWebView;
class QStringList;

/**
  * Class MapViewer makes the graphs of the route calculated
  * for the sonde using the Google Maps technology.
  * This class uses the framework Qt for graphical purposes.
  */
class MapViewer : public QWidget
{
public:
        /**
          * Mapviewer constructor.
          */
        MapViewer( QWidget *parent );

        /**
          * \brief addPath adds an EarthTrajectory to the map.
          * \param EarthTrajectory
          */
	void addPath(const EarthTrajectory &path);

        /**
          * \brief addMarker adds a marker to the Google Maps map.
          * \param marker
          */
        void addMarker(const EarthPoint4D &marker);

        void clear();

	void setCenter( double latitudeCenter_, double longitudeCenter_ );
	void setZoomLevel( int zoomLevel_ );

protected:
	double latitudeCenter;
	double longitudeCenter;
	double zoomLevel;
        /**
          * \brief fillHtml is a method that generates html code.
          * It calls the next fill methods, such as fillHead,
          * fillJavaScript.
          * \param code
          */
	void fillHtml(QStringList &code);

        /**
          * \brief fillHead creates the head of the html file.
          * \param code
          */
        void fillHead(QStringList &code);

        /**
          * \brief fillJavaScript puts JavaScript code into the
          * html code that is posted.
          * \param code
          */
        void fillJavaScript(QStringList &code);

        /**
          * \brief fillPathData generates the code that draws the
          * path on the Google Maps map.
          * \param code
          */
	void fillPathData(QStringList &code);

        /**
          * \brief fillMarkerData generates the code to create
          * markers on the map.
          * \param code
          */
        void fillMarkerData(QStringList &code);

        /**
          * \brief fillBody fills the body of the html posted code.
          * \param code
          */
	void fillBody(QStringList &code);

        /**
          * \brief fillKml fills a .kml file that can be opened on
          * Google Earth.
          * \param code
          */
        void fillKml(QStringList &code);
        void fillGoogleEarth(QStringList &code);
        /**
          * \brief fillEarthMap generates the coordinates and height
          * which let Google Earth read and plot the .kml file.
          * \param code
          */
        void EarthMap(QStringList &code);
	QWebView *viewer;
	vector<EarthTrajectory> paths;
	vector<EarthPoint4D> marks;
};

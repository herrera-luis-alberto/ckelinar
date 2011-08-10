#include "MapViewer.h"
#include <sstream>
#include <QStringList>
#include <QGridLayout>
#include <QWebView>
using std::stringstream;


MapViewer::MapViewer()
	: QWidget(NULL)
{
	QGridLayout *layout = new QGridLayout(1,1);

	viewer = new QWebView(this);
	layout->addWidget(viewer);
	QStringList code;
	fillHtml(code);
	viewer->setHtml(code.join("\n"));

	setLayout(layout);
}

void MapViewer::setPath(const string &name, const vector<EarthPoint4D> &path)
{
	paths[name] = path;

	QStringList code;
	fillHtml(code);
	viewer->setHtml(code.join("\n"));
	update();
}



void MapViewer::fillHtml(QStringList &code)
{
	code<<"<html>";
	fillHead(code);
	fillBody(code);
	code<<"</html>";
}

void MapViewer::fillHead(QStringList &code)
{
	code<<"<head><meta name=\"viewport\" content=\"initial-scale=1.0, user-scalable=no\" />"
	    <<"<script type=\"text/javascript\" src=\"http://maps.google.com/maps/api/js?sensor=false\"></script>"
	    <<"<script type=\"text/javascript\">";

	fillJavascript(code);

	code<<"</script></head>";

}

void MapViewer::fillJavascript(QStringList &code)
{
	code<<"function initialize() {"
	    <<"var latlng = new google.maps.LatLng(-33.46, -70.66);"
	    <<"var myOptions = {"
	    <<"zoom: 16,"
	    <<"center: latlng,"
	    <<"mapTypeId: google.maps.MapTypeId.HYBRID};"
	    <<"var map = new google.maps.Map(document.getElementById(\"map_canvas\"), myOptions);";
	fillPathData( code );
	code<<"}";
}

void MapViewer::fillPathData(QStringList &code)
{

	map<string,vector<EarthPoint4D> >::iterator it;

	for ( it = paths.begin(); it != paths.end(); it++)
	{

		code<<"var flightPlanCoordinates = [";

		vector<EarthPoint4D> currentPath = (*it).second;

		for ( int i=0; i<currentPath.size(); i++)
		{
			stringstream line;
			line<<"new google.maps.LatLng("<< currentPath[i].latitude <<","<< currentPath[i].longitude<<")";

			if ( i!=currentPath.size())
				line<<",";

			code<<line.str().c_str();

		}

		code<<"];"
			<<"var flightPath = new google.maps.Polyline({"
			<<"path: flightPlanCoordinates,"
			<<"strokeColor: \"#FF0000\","
			<<"strokeOpacity: 1.0,"
			<<"strokeWeight: 2"
			<<"});"
			<<"flightPath.setMap(map);";
	}

	/*new google.maps.LatLng(37.772323, -122.214897),
	new google.maps.LatLng(21.291982, -157.821856),
	new google.maps.LatLng(-18.142599, 178.431),
	new google.maps.LatLng(-27.46758, 153.027892)*/
}

void MapViewer::fillBody(QStringList &code)
{
	code<<"<body onload=\"initialize()\">"
	    <<"<div id=\"map_canvas\" style=\"width:100%; height:100%\"></div>"
	    <<"</body>";
}


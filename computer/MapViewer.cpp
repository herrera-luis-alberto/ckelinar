#include "MapViewer.h"
#include <sstream>
#include <QStringList>
#include <QGridLayout>
#include <QWebView>
#include <iostream>
using std::stringstream;


MapViewer::MapViewer( QWidget *parent )
        : QWidget(parent)
{
	QGridLayout *layout = new QGridLayout(1,1);

	viewer = new QWebView(this);
	layout->addWidget(viewer);
	QStringList code;
	fillHtml(code);
	viewer->setHtml(code.join("\n"));

	setLayout(layout);
}

void MapViewer::addPath(const EarthTrajectory &path)
{
	paths.push_back( path );

	QStringList code;
	fillHtml(code);
	//std::cout<<code.join("\n").toStdString()<<std::endl;
	viewer->setHtml(code.join("\n"));
}

void MapViewer::addMarker(const EarthPoint4D &mark)
{
	marks.push_back( mark );

	QStringList code;
	fillHtml(code);
	viewer->setHtml(code.join("\n"));
}

void MapViewer::clear()
{
	paths.clear();
	marks.clear();

	QStringList code;
	fillHtml(code);
	viewer->setHtml(code.join("\n"));
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

	fillJavaScript(code);

	code<<"</script></head>";

}

void MapViewer::fillJavaScript(QStringList &code)
{
	code<<"function initialize() {"
            <<"var latlng = new google.maps.LatLng(-33.457967, -70.661896);"
	    <<"var myOptions = {"
            <<"zoom: 18,"
	    <<"panControl: true,"
	    <<"zoomControl: true,"
	    <<"scaleControl: true,"
	    <<"center: latlng,"
	    <<"mapTypeId: google.maps.MapTypeId.HYBRID};"
            <<"var map = new google.maps.Map(document.getElementById(\"map_canvas\"), myOptions);"
            <<"map.setTilt(0);";
	fillPathData( code );
	fillMarkerData( code );
	code<<"}";
}

void MapViewer::fillPathData(QStringList &code)
{

	for ( int i=0; i<paths.size(); i++)
	{

		code<<"var flightPlanCoordinates = [";

		EarthTrajectory currentPath = paths[i];

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

}

void MapViewer::fillMarkerData(QStringList &code)
{
	for ( int i=0; i<marks.size(); i++)
	{
		stringstream line;
		line<<"var latlng = new google.maps.LatLng("<<marks[i].latitude <<", "<< marks[i].longitude<<");";
		code<<line.str().c_str();
		code<<"new google.maps.Marker({ position: latlng,  map: map});";
	}
}

void MapViewer::fillBody(QStringList &code)
{
	code<<"<body onload=\"initialize()\">"
	    <<"<div id=\"map_canvas\" style=\"width:100%; height:100%\"></div>"
	    <<"</body>";
}


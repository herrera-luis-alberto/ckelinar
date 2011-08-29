#include "MapViewer.h"
#include <sstream>
#include <QStringList>
#include <QGridLayout>
#include <QWebView>
#include <QFile>
#include <QTextStream>
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
    std::cout<<"Marker"<<std::endl;
    std::cout<<mark.time<<std::endl;

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

void MapViewer::setCenter( double latitudeCenter_, double longitudeCenter_ )
{
    latitudeCenter = latitudeCenter_;
    longitudeCenter = longitudeCenter_;
}

void MapViewer::setZoomLevel( int zoomLevel_ )
{
    zoomLevel = zoomLevel_;
}


void MapViewer::fillHtml(QStringList &code)
{
	code<<"<html>";
	fillHead(code);
	fillBody(code);
	code<<"</html>";

        QFile SondeMap("SondeMap.html");
        if (SondeMap.open(QFile::WriteOnly | QFile::Text)) {
          QTextStream s(&SondeMap);
          for (int i = 0; i < code.size(); ++i)
            s << code.at(i) << '\n';
        } else {
          std::cerr << "Error opening output file SondeMap.html\n";
        }
        SondeMap.close();
        QStringList bla;
        fillKml( bla);
}

void MapViewer::fillKml(QStringList &code)
{
    code<<"<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
        <<"<kml>"
        <<"  <Document>"
        <<"    <Style id=\"yellowLineGreenPoly\">"
        <<"      <LineStyle>"
        <<"        <color>7f00ffff</color>"
        <<"        <width>4</width>"
        <<"      </LineStyle>"
        <<"      <PolyStyle>"
        <<"        <color>7f00ff00</color>"
        <<"      </PolyStyle>"
        <<"    </Style>";



    fillGoogleEarth(code);


        code<<"  </Document>"
        <<"</kml>";

    QFile EarthMap("EarthMap.kml");
    if (EarthMap.open(QFile::WriteOnly | QFile::Text)) {
    QTextStream s(&EarthMap);
    for (int i = 0; i < code.size(); ++i)
      s << code.at(i) << '\n';
    } else {
    std::cerr << "Error opening output file EarthMap.kml\n";
    }
    EarthMap.close();
}

void MapViewer::fillGoogleEarth(QStringList &code)
{
    for ( int i=0; i<paths.size(); i++)
    {
       EarthTrajectory currentPath = paths[i];
       code<<"    <Placemark>"
       <<"      <name>Absolute Extruded</name>"
       <<"      <description>Transparent green wall with yellow outlines</description>"
       <<"      <styleUrl>#yellowLineGreenPoly</styleUrl>"
       <<"      <LineString>"
       <<"        <extrude>1</extrude>"
       <<"        <tessellate>1</tessellate>"
       <<"        <altitudeMode>absolute</altitudeMode>"
       <<"        <coordinates>";

            for ( int i=0; i<currentPath.size(); i++)
            {
                    stringstream line;
                    line<<currentPath[i].longitude <<","<< currentPath[i].latitude<<","<<currentPath[i].height;

                    code<<line.str().c_str();

            }


    code<<"        </coordinates>"
        <<"      </LineString>"
        <<"    </Placemark>";
    }
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
	    <<QString("var latlng = new google.maps.LatLng(%1, %2);").arg(latitudeCenter).arg(longitudeCenter)
	    <<"var myOptions = {"
	    <<QString("zoom: %1,").arg(zoomLevel)
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


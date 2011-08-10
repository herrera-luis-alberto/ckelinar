#include "MapViewer.h"

MapViewer::MapViewer()
	: QWidget(NULL)
{
	viewer = new QWebView(this);
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
	    <<"var map = new google.maps.Map(document.getElementById(\"map_canvas\"), myOptions);}";

}

void MapViewer::fillBody(QStringList &code)
{
	code<<"<body onload=\"initialize()\">"
	    <<"<div id=\"map_canvas\" style=\"width:100%; height:100%\"></div>"
	    <<"</body>";
}


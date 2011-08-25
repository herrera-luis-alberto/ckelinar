#include "MainWindow.h"
#include "MapViewer.h"
#include "SondeViewer.h"
#include <QGridLayout>
#include <vector>
using std::vector;

MainWindow::MainWindow()
    : forecaster ( "wrfout_d02_160511.nc" )
{
    QGridLayout *layout = new QGridLayout(this);
    map = new MapViewer(this);
    sonde = new SondeViewer(this);
    layout->addWidget( map, 0, 0);
    layout->addWidget( sonde, 1, 0);


    QGridLayout *controlLayout = new QGridLayout;

    mapReload = new QPushButton("Reload Map", this);
    mapLatitudeCenter = new QDoubleSpinBox(this);
    mapLatitudeCenter->setRange(-90, 90);
    mapLatitudeCenter->setValue(-33.457780);
    mapLatitudeCenter->setDecimals(4);
    mapLatitudeCenter->setSingleStep(0.0001);
    mapLongitudeCenter = new QDoubleSpinBox(this);
    mapLongitudeCenter->setRange(-180, 180);
    mapLongitudeCenter->setValue(-70.662813);
    mapLongitudeCenter->setDecimals(4);
    mapLongitudeCenter->setSingleStep(0.0001);
    mapZoomLevel = new QSpinBox(this);
    mapZoomLevel->setRange(1, 20);
    mapZoomLevel->setValue(16);

    initialLatitude = new QDoubleSpinBox(this);
    initialLatitude->setRange(-90, 90);
    initialLatitude->setValue(-33.457780);
    initialLatitude->setDecimals(4);
    initialLatitude->setSingleStep(0.0001);
    initialLongitude = new QDoubleSpinBox(this);
    initialLongitude->setRange(-180, 180);
    initialLongitude->setValue(-70.662813);
    initialLongitude->setDecimals(4);
    initialLongitude->setSingleStep(0.0001);
    initialHeight = new QDoubleSpinBox(this);
    initialHeight->setRange(0, 10000);
    initialHeight->setValue(547);
    initialHeight->setDecimals(0);
    initialHeight->setSingleStep(1);
    initialYear = new QSpinBox(this);
    initialYear->setRange(2010, 2020);
    initialYear->setValue(2011);

    initialMonth = new QSpinBox(this);
    initialMonth->setRange(1, 12);
    initialMonth->setValue(8);
    initialDay = new QSpinBox(this);
    initialDay->setRange(1, 31);
    initialYear->setValue(20);
    initialHour = new QSpinBox(this);
    initialHour->setRange(0, 23);
    initialHour->setValue(19);
    initialMinute = new QSpinBox(this);
    initialMinute->setRange(0, 59);
    initialMinute->setValue(0);

    upSpeed = new QDoubleSpinBox(this);
    upSpeed->setRange(0, 10);
    upSpeed->setValue(5);
    upSpeed->setDecimals(1);
    upSpeed->setSingleStep(0.1);
    downSpeed = new QDoubleSpinBox(this);
    downSpeed->setRange(-10, 0);
    downSpeed->setValue(-5);
    downSpeed->setDecimals(1);
    downSpeed->setSingleStep(0.1);

    releaseHeight = new QDoubleSpinBox(this);
    releaseHeight->setRange(0, 10000);
    releaseHeight->setValue(5000);
    releaseHeight->setDecimals(0);
    releaseHeight->setSingleStep(1);

    controlLayout->addWidget( new QLabel("Map Options"), 0, 0);
    controlLayout->addWidget( mapReload, 0, 1);
    controlLayout->addWidget( new QLabel("Center Latitude"), 1, 0);
    controlLayout->addWidget( mapLatitudeCenter, 1, 1);
    controlLayout->addWidget( new QLabel("Center Longitude"), 2, 0);
    controlLayout->addWidget( mapLongitudeCenter, 2, 1);
    controlLayout->addWidget( new QLabel("Zoom"), 3, 0);
    controlLayout->addWidget( mapZoomLevel, 3, 1);

    controlLayout->addWidget( new QLabel("Forecasting Options"), 4, 0, 1, 2);
    controlLayout->addWidget( new QLabel("Initial Latitude"), 5, 0);
    controlLayout->addWidget( initialLatitude, 5, 1);
    controlLayout->addWidget( new QLabel("Initial Longitude"), 6, 0);
    controlLayout->addWidget( initialLongitude, 6, 1);
    controlLayout->addWidget( new QLabel("Initial Height"), 7, 0);
    controlLayout->addWidget( initialHeight, 7, 1);

    controlLayout->addWidget( new QLabel("Initial Year"), 8, 0);
    controlLayout->addWidget( initialYear, 8, 1);
    controlLayout->addWidget( new QLabel("Initial Month"), 9, 0);
    controlLayout->addWidget( initialMonth, 9, 1);
    controlLayout->addWidget( new QLabel("Initial Day"), 10, 0);
    controlLayout->addWidget( initialDay, 10, 1);
    controlLayout->addWidget( new QLabel("Initial Hour"), 11, 0);
    controlLayout->addWidget( initialHour, 11, 1);
    controlLayout->addWidget( new QLabel("Initial Minute"), 12, 0);
    controlLayout->addWidget( initialMinute, 12, 1);

    controlLayout->addWidget( new QLabel("Up Speed"), 13, 0);
    controlLayout->addWidget( upSpeed, 13, 1);
    controlLayout->addWidget( new QLabel("Down Speed"), 14, 0);
    controlLayout->addWidget( downSpeed, 14, 1);
    controlLayout->addWidget( new QLabel("Release Heaight"), 15, 0);
    controlLayout->addWidget( releaseHeight, 15, 1);


    QGridLayout *dataLayout = new QGridLayout;
    analogPressure = new QLabel(this);
    analogHumidity = new QLabel(this);
    analogTemperature = new QLabel(this);
    sensirionTemperature = new QLabel(this);
    sensirionHumidity = new QLabel(this);

    dataLayout->addWidget( new QLabel("Analog Preassure: "), 6, 0);
    dataLayout->addWidget( analogPressure, 6, 1);
    dataLayout->addWidget( new QLabel("Analog Humidity: "), 7, 0);
    dataLayout->addWidget( analogHumidity, 7, 1);
    dataLayout->addWidget( new QLabel("Analog Temperature: "), 8, 0);
    dataLayout->addWidget( analogTemperature, 8, 1);
    dataLayout->addWidget( new QLabel("Sensirion Humidity: "), 9, 0);
    dataLayout->addWidget( sensirionHumidity, 9, 1);
    dataLayout->addWidget( new QLabel("Sensirion Temperature: "), 10, 0);
    dataLayout->addWidget( sensirionTemperature, 10, 1);


    layout->addLayout( controlLayout, 0, 1);
    layout->addLayout( dataLayout, 1, 1);
    setLayout(layout);


    updateMap = new QTimer(this);

    connect(mapReload, SIGNAL(clicked()), this, SLOT(reloadMap()));
    connect( sonde, SIGNAL(newSondeData(SondeData)), this, SLOT(newSondeData(SondeData)));
    connect(updateMap, SIGNAL(timeout()), this, SLOT(reloadMap()));

    reloadMap();
    updateMap->start( 20000 );

}

void MainWindow::newSondeData(SondeData data)
{
    realTimeTrajectory.push_back(data.location);

    analogHumidity->setText( QString("%1").arg(data.analogHumidity));
    analogPressure->setText( QString("%1").arg(data.analogPressure));
    analogTemperature->setText( QString("%1").arg(data.analogTemperature));

    sensirionHumidity->setText( QString("%1").arg(data.sensirionHumidity));
    sensirionTemperature->setText( QString("%1").arg(data.sensirionTemperature));

}

void MainWindow::reloadMap()
{
    map->setCenter( mapLatitudeCenter->value(), mapLongitudeCenter->value() );
    map->setZoomLevel( mapZoomLevel->value() );
    map->clear();
    map->addPath(realTimeTrajectory);

    EarthPoint4D init;
    init.latitude = initialLatitude->value();
    init.longitude = initialLongitude->value();
    init.height = initialHeight->value();

    int year = initialYear->value();
    int month = initialMonth->value();
    int day = initialDay->value();
    int hour = initialHour->value();
    int minute = initialMinute->value();

    init.time = ptime( boost::gregorian::date( year, month, day),
		       boost::posix_time::hours(hour) + boost::posix_time::minutes(minute) );


    EarthTrajectory staticPredictedUp = forecaster.getTrayectory( init, upSpeed->value(), 100);
    vector<EarthPoint4D> releasePoint = staticPredictedUp.findLevelPoints( releaseHeight->value() );

    if ( releasePoint.size() > 0)
    {
	map->addMarker( releasePoint[0] );
	EarthTrajectory staticPredictedDown = forecaster.getTrayectory( releasePoint[0], downSpeed->value(), 100);
	map->addPath(staticPredictedDown);
    }

    map->addPath(staticPredictedUp);

}

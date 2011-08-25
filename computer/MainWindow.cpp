#include "MainWindow.h"
#include "MapViewer.h"
#include "SondeViewer.h"
#include <QGridLayout>

MainWindow::MainWindow()
{
    QGridLayout *layout = new QGridLayout;
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
    controlLayout->addWidget( new QLabel("Up Speed"), 8, 0);
    controlLayout->addWidget( upSpeed, 8, 1);
    controlLayout->addWidget( new QLabel("Down Speed"), 9, 0);
    controlLayout->addWidget( downSpeed, 9, 1);
    controlLayout->addWidget( new QLabel("Release Heaight"), 10, 0);
    controlLayout->addWidget( releaseHeight, 10, 1);


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
}

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
    setLayout(layout);

    updateMap = new QTimer(this);

    connect( sonde, SIGNAL(newSondeData(SondeData)), this, SLOT(newSondeData(SondeData)));
    connect(updateMap, SIGNAL(timeout()), this, SLOT(reloadMap()));

    updateMap->start( 20000 );

}

void MainWindow::newSondeData(SondeData data)
{
    realTimeTrajectory.push_back(data.location);

}

void MainWindow::reloadMap()
{
    map->clear();
    map->addPath(realTimeTrajectory);
}

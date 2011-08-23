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

    connect( sonde, SIGNAL(newSondeData(SondeData)), this, SLOT(newSondeData(SondeData)));
}

void MainWindow::newSondeData(SondeData data)
{
    realTimeTrajectory.push_back(data.location);
    map->addPath(realTimeTrajectory);
}

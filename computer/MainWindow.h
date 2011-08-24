#include <QWidget>
#include <QTimer>
#include "BasicDataTypes.h"
#include "EarthTrajectory.h"

class MapViewer;
class SondeViewer;

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    MainWindow();
public slots:
    void newSondeData(SondeData data);
    void reloadMap();
protected:
    MapViewer *map;
    SondeViewer *sonde;
    QTimer *updateMap;

    EarthTrajectory realTimeTrajectory;


};

#include <QWidget>
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
protected:
    MapViewer *map;
    SondeViewer *sonde;

    EarthTrajectory realTimeTrajectory;


};

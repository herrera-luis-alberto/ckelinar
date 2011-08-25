#include <QWidget>
#include <QTimer>
#include <QDoubleSpinBox>
#include <QLabel>
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

    QDoubleSpinBox *initialLatitude;
    QDoubleSpinBox *initialLongitude;
    QDoubleSpinBox *initialHeight;
    QDoubleSpinBox *upSpeed;
    QDoubleSpinBox *downSpeed;
    QDoubleSpinBox *releaseHeight;

    QLabel *analogPressure;
    QLabel *analogHumidity;
    QLabel *analogTemperature;
    QLabel *sensirionTemperature;
    QLabel *sensirionHumidity;


};

#include <QWidget>
#include <QTimer>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QPushButton>
#include "BasicDataTypes.h"
#include "EarthTrajectory.h"
#include "TrajectoryForecaster.h"

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
    void uploadMap();
    void uploadKML();
protected:
    TrajectoryForecaster forecaster;
    MapViewer *map;
    SondeViewer *sonde;
    QTimer *updateMap;

    EarthTrajectory realTimeTrajectory;

    QPushButton *mapReload;
    QPushButton *mapUpload;
    QDoubleSpinBox *mapLatitudeCenter;
    QDoubleSpinBox *mapLongitudeCenter;
    QSpinBox *mapZoomLevel;

    QDoubleSpinBox *initialLatitude;
    QDoubleSpinBox *initialLongitude;
    QDoubleSpinBox *initialHeight;
    QSpinBox *initialYear;
    QSpinBox *initialMonth;
    QSpinBox *initialDay;
    QSpinBox *initialHour;
    QSpinBox *initialMinute;
    QDoubleSpinBox *upSpeed;
    QDoubleSpinBox *downSpeed;
    QDoubleSpinBox *releaseHeight;

    QLabel *analogPressure;
    QLabel *analogHumidity;
    QLabel *analogTemperature;
    QLabel *sensirionTemperature;
    QLabel *sensirionHumidity;


};

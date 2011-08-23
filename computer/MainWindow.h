#include <QWidget>

class MapViewer;
class SondeViewer;

class MainWindow : public QWidget
{
public:
    MainWindow();
protected:
    MapViewer *map;
    SondeViewer *sonde;


};

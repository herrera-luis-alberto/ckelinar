#include "TrajectoryForecaster.h"
#include <map>
#include <QWidget>
using std::map;

class QWebView;
class QStringList;

class MapViewer : public QWidget
{
public:
	MapViewer();
	void addPath(const EarthTrajectory &path);
	void addMark(const EarthPoint4D &mark);
protected:
	void fillHtml(QStringList &code);
	void fillHead(QStringList &code);
	void fillJavascript(QStringList &code);
	void fillPathData(QStringList &code);
	void fillMarkData(QStringList &code);
	void fillBody(QStringList &code);
	QWebView *viewer;
	vector<EarthTrajectory> paths;
	vector<EarthPoint4D> marks;
};

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
	void setPath(const string &name, const vector<EarthPoint4D> &path);
protected:
	void fillHtml(QStringList &code);
	void fillHead(QStringList &code);
	void fillJavascript(QStringList &code);
	void fillPathData(QStringList &code);
	void fillBody(QStringList &code);
	QWebView *viewer;
	map<string,vector<EarthPoint4D> > paths;
};

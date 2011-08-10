#include <QWebView>
#include <QStringList>
#include <iostream>
using namespace std;

class MapViewer : public QWidget
{
public:
	MapViewer();

protected:

	void fillHtml(QStringList &code);
	void fillHead(QStringList &code);
	void fillJavascript(QStringList &code);
	void fillBody(QStringList &code);
	QWebView *viewer;
};

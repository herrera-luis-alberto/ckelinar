#ifndef SONDEVIEWER_H
#define SONDEVIEWER_H

#include "BasicDataTypes.h"
#include "SimpleSerial.h"
#include <QThread>
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
using std::string;



class ReadingThread : public QThread
{
    Q_OBJECT
public:
    ReadingThread(const QString &port);
    void run();
    void requestStop();
signals:
    void newSondeData(SondeData data);
    void newSondeMsg(QString msg);
protected:
    void process( const string &frame);
    bool isRunning;
    QString portName;
    SimpleSerial serial;

};

class SondeViewer : public QWidget
{
    Q_OBJECT
public:
    SondeViewer(QWidget *parent);
signals:
    void newSondeData(SondeData data);
public slots:
    void connectSlot();
    void disconnectSlot();

    void newSondeMsg(QString msg);

protected:
    QPushButton *connectButton;
    QPushButton *disconnectButton;
    QLineEdit *port;
    QTextEdit *log;

    ReadingThread *readThread;
};

#endif


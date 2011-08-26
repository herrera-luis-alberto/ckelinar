#ifndef SONDEVIEWER_H
#define SONDEVIEWER_H

#include "BasicDataTypes.h"
#include "SimpleSerial.h"
#include <QThread>
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include "boost/date_time/posix_time/posix_time.hpp"
using std::string;
using boost::posix_time::ptime;

enum SondeFields
{
    FrameCounter = 0,
    PreassureAnalog,
    TemperatureAnalog,
    HumidityAnalog,
    TemperatureSensirion,
    HumiditySensirion,
    GPSTime,
    GPSLatitudeSign,
    GPSLatitude,
    GPSLongitudeSign,
    GPSLongitude,
    GPSHeight,
    SondeFieldCount
};


class ReadingThread : public QThread
{
    Q_OBJECT
public:
    ReadingThread(const QString &port, const QString &logFilename_);
    void run();
    void requestStop();

    /**
      * \brief Convert latitude from GPS to degrees and minutes,
      * the way Google Maps reads it.
      * \param lat is the raw latitude from the GPS.
      */
    float latitude2degrees( float lat );

    /**
      * \brief Convert longitude from GPS to degrees and minutes,
      * the way Google Maps reads it.
      * \param lon is the raw longitude from the GPS.
      */
    float longitude2degrees( float lat );
signals:
    void newSondeData(SondeData data);
    void newSondeMsg(QString msg);
protected:
    SondeData DataFromFields( QStringList fields );
    void process( const string &frame);
    bool isRunning;
    QString portName;
    SimpleSerial serial;
    QString logFilename;

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
    QLineEdit *logFilename;
    QTextEdit *log;

    ReadingThread *readThread;
};

#endif


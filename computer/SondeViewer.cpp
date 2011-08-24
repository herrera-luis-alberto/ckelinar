#include "SondeViewer.h"
#include <QHBoxLayout>
#include <QLabel>
#include <iostream>

using namespace std;

enum SondeFields
{
    PreassureAnalog = 0,
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


ReadingThread::ReadingThread( const QString &port )
    : isRunning ( true )
    , portName ( port )
    , serial(port.toStdString(), 38400)
{
}

void ReadingThread::run()
{
    while( isRunning )
    {
        process( serial.readLine() );

    }
}

void ReadingThread::requestStop()
{
    isRunning = false;
}



SondeData ReadingThread::DataFromFields( QStringList fields ) {
    SondeData data;

    int hour = fields[GPSTime].mid(0,2).toInt();
    int minutes = fields[GPSTime].mid(2,2).toInt();
    double seconds = fields[GPSTime].mid(4).toDouble();
    data.location.time = boost::posix_time::ptime( boost::posix_time::second_clock::universal_time().date(),
                                                  boost::posix_time::hours(hour) +
                                                  boost::posix_time::minutes(minutes) +
                                                  boost::posix_time::seconds(seconds) );

    data.location.latitude = latitude2degrees( fields[GPSLatitude].toFloat() );

    if ( fields[GPSLatitudeSign] == "-" )
        data.location.latitude *= -1;


    data.location.longitude = longitude2degrees( fields[GPSLongitude].toFloat() );
    if ( fields[GPSLongitudeSign] == "-" )
        data.location.longitude *= -1;

    data.location.height = fields[GPSHeight].toFloat();

    return data;
}

float ReadingThread::latitude2degrees( float lat ) {
    float degrees, minutes;

    degrees = int(lat/100);
    minutes = lat-100*degrees;
    lat = degrees + minutes/60;

    return lat;
}

float ReadingThread::longitude2degrees( float lon ) {
    float degrees, minutes;

    degrees = int(lon/100);
    minutes = lon-100*degrees;
    lon = degrees + minutes/60;

    return lon;
}


void ReadingThread::process( const string &frame )
{
    QString myframe(frame.c_str());

    QStringList fields = myframe.split("|");


    if ( fields.size() != SondeFieldCount ) { /* REVISAR ! */
        return;
    }

    SondeData data = DataFromFields( fields );

    cout<<data.location.time<<endl;
    cout<<data.location.latitude<<endl;
    cout<<data.location.longitude<<endl;
    cout<<data.location.height<<endl;

    emit newSondeData( data );
    emit newSondeMsg( myframe );
}


SondeViewer::SondeViewer(QWidget *parent)
    : readThread(NULL)
{
    QGridLayout *layout = new QGridLayout;
    QHBoxLayout *header = new QHBoxLayout;
    connectButton = new QPushButton("Connect", this);
    disconnectButton = new QPushButton("Disconnect", this);
    disconnectButton->setEnabled(false);
    port = new QLineEdit("/dev/ttyUSB0", this);
    log = new QTextEdit(this);
    log->setReadOnly(true);

    header->addWidget( connectButton );
    header->addWidget( disconnectButton );
    header->addWidget( new QLabel("Port:") );
    header->addWidget( port );

    layout->addLayout(header, 0, 0);
    layout->addWidget(log, 1, 0);

    setLayout( layout );

    connect( connectButton, SIGNAL(clicked(bool)), this, SLOT(connectSlot()));
    connect( disconnectButton, SIGNAL(clicked(bool)), this, SLOT(disconnectSlot()));


}

void SondeViewer::connectSlot()
{
   readThread = new ReadingThread( port->text() );
   connectButton->setEnabled(false);
   disconnectButton->setEnabled(true);
   connect( readThread, SIGNAL(newSondeMsg(QString)), this, SLOT(newSondeMsg(QString)));
   connect( readThread, SIGNAL(newSondeData(SondeData)), this, SIGNAL(newSondeData(SondeData)));
   readThread->start();
}

void SondeViewer::disconnectSlot()
{
    readThread->requestStop();
    readThread->wait();
    delete readThread;

    connectButton->setEnabled(true);
    disconnectButton->setEnabled(false);
}

void SondeViewer::newSondeMsg(QString msg)
{
    log->insertPlainText( msg + '\n' );
}

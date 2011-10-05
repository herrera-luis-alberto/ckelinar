#include "SondeViewer.h"
#include <QHBoxLayout>
#include <QLabel>
#include <fstream>
using std::ofstream;
using std::endl;

ReadingThread::ReadingThread( const QString &port, const QString &logFilename_)
    : isRunning ( true )
    , portName ( port )
    , serial(port.toStdString(), 38400)
    , logFilename(logFilename_)
{
}

void ReadingThread::run()
{
    ofstream log( logFilename.toStdString().c_str() );
    while( isRunning )
    {
	string line = serial.readLine();
	log<<":"<<line<<endl;
	process( line );

    }
}

void ReadingThread::requestStop()
{
    isRunning = false;
}

SondeData ReadingThread::DataFromFields( QStringList fields )
{
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

    data.analogHumidity = (fields[HumidityAnalog].toInt()-170)/6.0; //don't know why
    data.analogTemperature = fields[TemperatureAnalog].toInt()*1.083-721; //don't know why

    double pressureVoltage = 5*fields[PreassureAnalog].toDouble()/1023; // 10 bits => 1023
    //data.analogPressure = (1000*pressureVoltage/5+95)/9;  //from datasheet
    data.analogPressure = (222.633*pressureVoltage + 42.2667);  //from datasheet

    //values from sensirion datasheet
    const double SORH_C1 = -2.0468;
    const double SORH_C2 = 0.0367;
    const double SORH_C3 = -1.5955E-6;

    double sensirionValue = fields[HumiditySensirion].toDouble();

    data.sensirionHumidity = SORH_C1 + SORH_C2 * sensirionValue + SORH_C3 * sensirionValue * sensirionValue;

    //values from sensirion datasheet
    const double SOTEMP_D1 = -40.1;
    const double SOTEMP_D2 = 0.01;

    data.sensirionTemperature = SOTEMP_D1 + SOTEMP_D2 * fields[TemperatureSensirion].toDouble();


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
    logFilename = new QLineEdit("log.txt", this);
    log = new QTextEdit(this);
    log->setReadOnly(true);

    header->addWidget( connectButton );
    header->addWidget( disconnectButton );
    header->addWidget( new QLabel("Port:") );
    header->addWidget( port );
    header->addWidget( logFilename );

    layout->addLayout(header, 0, 0);
    layout->addWidget(log, 1, 0);

    setLayout( layout );

    connect( connectButton, SIGNAL(clicked(bool)), this, SLOT(connectSlot()));
    connect( disconnectButton, SIGNAL(clicked(bool)), this, SLOT(disconnectSlot()));


}

void SondeViewer::connectSlot()
{
   readThread = new ReadingThread( port->text(), logFilename->text() );
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
    log->ensureCursorVisible();
}

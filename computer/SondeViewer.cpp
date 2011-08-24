#include "SondeViewer.h"
#include <QHBoxLayout>
#include <QLabel>
#include <iostream>
using namespace std;

ReadingThread::ReadingThread(const QString &port)
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

void ReadingThread::process( const string &frame)
{
    QString myframe(frame.c_str());

    QStringList fields = myframe.split("|");

    if ( fields.size() < 8 )
        return;

    SondeData data;

    float dd, mm;

    //! TODO harcoded => change!!
    data.location.latitude = fields[7].toFloat();
    dd = int(data.location.latitude/100);
    mm = data.location.latitude-100*dd;
    data.location.latitude = dd + mm/60;

    if ( fields[6] == "-" )
        data.location.latitude *= -1;

    data.location.longitude = fields[9].toFloat();
    dd = int(data.location.longitude/100);
    mm = data.location.longitude-100*dd;
    data.location.longitude = dd + mm/60;
    if ( fields[8] == "-" )
        data.location.longitude *= -1;

   cout<<data.location.latitude<<endl;
   cout<<data.location.longitude<<endl;


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

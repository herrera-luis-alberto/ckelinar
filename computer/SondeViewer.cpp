#include "SondeViewer.h"
#include <QHBoxLayout>
#include <QLabel>

ReadingThread::ReadingThread(const QString &port)
    : isRunning ( true )
    , portName ( port )
{
}

void ReadingThread::run()
{
    while( isRunning )
    {
    }
}

void ReadingThread::requestStop()
{
    isRunning = false;
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
   readThread->start();
}

void SondeViewer::disconnectSlot()
{
    readThread->requestStop();
    readThread->wait();

    connectButton->setEnabled(true);
    disconnectButton->setEnabled(false);
}

void SondeViewer::newSondeMsg(QString msg)
{
    log->insertPlainText( msg );
}

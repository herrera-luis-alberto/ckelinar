#include "DataInterface.h"
#include "PortDefinitions.h"

DataInterface::DataInterface()
    : serialImpl ( communicationRxPin, communicationTxPin)
{
    serial = &serialImpl;
}

void DataInterface::begin()
{
  serialImpl.begin( 1200 );
  pinMode(10, OUTPUT);

  const int chipSelect = 8;
  serial->print("SD...");
  if (!SD.begin( chipSelect )) {
    serial->println("FAIL");
    return;
  } else {
    serial->println("OK");
  }

  println("Init...");
}

void DataInterface::startDataFrame()
{
    dataFile = SD.open("datalog.txt", O_CREAT | O_APPEND | O_WRITE);
    print(":");
    first = true;
}

void DataInterface::endDataFrame()
{
    dataFile.close();
}

void DataInterface::write(uint8_t data)
{
  serial->write(data);
  dataFile.write(data);
}

void DataInterface::sendData(int data)
{
  printSeparator();
  print(data);
}

void DataInterface::sendData(const char data[])
{
  printSeparator();
  print(data);
}

void DataInterface::startImage( uint16_t counter)
{
    char name[20]; // 8.3 format
    sprintf ( name, "im%u.jpg", counter );
    imageFile = SD.open( name, O_CREAT | O_WRITE);
}

void DataInterface::writeImage( uint16_t packageSize, byte* package)
{
    imageFile.write( package, packageSize);
}

void DataInterface::endImage()
{
    imageFile.close();
}


bool DataInterface::printSeparator()
{
  if (!first)
    print("|");
  first = false;
}

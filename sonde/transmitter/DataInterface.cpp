#include "DataInterface.h"
#include "PortDefinitions.h"

DataInterface::DataInterface()
{
}

void DataInterface::begin()
{
  Serial.begin( 38400 );
  pinMode(10, OUTPUT);

  const int chipSelect = 8;
  Serial.print("SD...");
  if (!SD.begin( chipSelect )) {
    Serial.println("FAIL");
    return;
  } else {
    Serial.println("OK");
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
  Serial.write(data);
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


bool DataInterface::printSeparator()
{
  if (!first)
    print("|");
  first = false;
}

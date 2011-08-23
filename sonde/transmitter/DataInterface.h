#ifndef DATAINTERFACE_H
#define DATAINTERFACE_H

#include <SD.h>
#include "PortDefinitions.h"

class DataInterface : public Print
{
public:
  DataInterface()
  {
  };

  void begin()
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

  void startDataFrame()
  {
      dataFile = SD.open("datalog.txt", O_CREAT | O_APPEND | O_WRITE);
      print(":");
      first = true;
  }

  void endDataFrame()
  {
      dataFile.close();
  }

  virtual void write(uint8_t data)
  {
    Serial.write(data);
    dataFile.write(data);
  }
  
  void sendData(int data)
  {
    printSeparator();
    print(data);
  }
  
  void sendData(const char data[])
  {
    printSeparator();
    print(data);
  }

private:
  File dataFile;
  bool first;
  
  bool printSeparator()
  {
    if (!first)
      print("|");
    first = false;
  }
};

#endif


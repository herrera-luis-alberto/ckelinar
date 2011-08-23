#ifndef DATAINTERFACE_H
#define DATAINTERFACE_H

#include <SD.h>

class DataInterface : public Print
{
public:
  DataInterface();
  void begin();
  void startDataFrame();
  void endDataFrame();
  virtual void write(uint8_t data);
  void sendData(int data);
  void sendData(const char data[]);

private:
  File dataFile;
  bool first;
  
  bool printSeparator();
};

#endif


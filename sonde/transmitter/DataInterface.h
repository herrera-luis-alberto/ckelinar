#ifndef DATAINTERFACE_H
#define DATAINTERFACE_H

#include <NewSoftSerial.h>

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

  void startImage( uint16_t counter);
  void writeImage( uint16_t packageSize, byte* package);
  void endImage();

  NewSoftSerial serialImpl;

private:
  File dataFile;
  File imageFile;
  bool first;
  
  bool printSeparator();

  Print *serial;
};

#endif


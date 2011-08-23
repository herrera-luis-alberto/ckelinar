#ifndef GPSREADER_H
#define GPSREADER_H

#include "PortDefinitions.h"
#include <SoftwareSerial.h>

enum GPSDataFileds
{
  UTCTime = 0,
  Latitude,
  NSIndicator,
  Longitude,
  EWIndicator,
  PositionFixIndicator,
  SatellitesUsed,
  HDOP,
  MSLAltitude,
  MSLAltitudeUnits,
  GeoidSeparation,
  GeoidSeparationUnits,
  AgeOfDiffCorr,
  DiffReffStationID,
  NoDataFields
};

#define MaxDataLenght 16

enum GPSState
{
  WaitForStart,
  ReadHeader,
  ReadData,
  DataOK
};

class GPSReader
{
public:
  GPSReader();
  
  void begin();
  
  void fetchPositionData();
  char *getRawData( uint8_t field );
  
protected:
  SoftwareSerial port;
  GPSState state;
  char incommingDataBuffers[NoDataFields][MaxDataLenght];
  
  uint8_t currentFieldReading;
  uint8_t currentFieldReadingPos;

  void evaluateStateMachine();
  
  
};

#endif

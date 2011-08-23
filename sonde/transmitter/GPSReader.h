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
#define MaxDataLenght 10

const char *expectedHeader = "GPGGA,";

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
  GPSReader()
  : port (GPSRxPin, GPSTxPin)
  {
    state = WaitForStart;
  }
  
  void begin()
  {
    port.begin(4800);
  }
  
  void fetchPositionData()
  {
    currentFieldReading = 0;
    currentFieldReadingPos = 0;
    state = WaitForStart;
    
    while ( state != DataOK )
      evaluateStateMachine();
  }
  
  char tmp;
  
//protected:
  SoftwareSerial port;
  GPSState state;
  char incommingDataBuffers[NoDataFields][MaxDataLenght];
  
  uint8_t currentFieldReading;
  uint8_t currentFieldReadingPos;

  void evaluateStateMachine()
  {
    uint8_t i;
    char inChar;
    switch ( state )
    {
      case WaitForStart:
	inChar = port.read();
	if ( inChar == '$' )
	  state = ReadHeader;
	break;
      case ReadHeader:
	for ( i=0; i<6; i++)
	{
	  inChar = port.read();
	  if ( inChar != expectedHeader[i] )
	  {
	    state = WaitForStart;
	    return;
	  }
	}
	state = ReadData;
	break;
      case ReadData:
	inChar = port.read();
	if ( inChar == '*' )
	{
	  incommingDataBuffers[currentFieldReading][currentFieldReadingPos] = '\0';
	  state = DataOK;
	  return;
	}
	if ( inChar == ',' )
	{
	  incommingDataBuffers[currentFieldReading][currentFieldReadingPos] = '\0';
	  currentFieldReading++;
	  currentFieldReadingPos = 0;
	  return;
	}
	incommingDataBuffers[currentFieldReading][currentFieldReadingPos++] = inChar;
	
	break;
      case DataOK:
	break;
    }
  }
  
  
};

#endif

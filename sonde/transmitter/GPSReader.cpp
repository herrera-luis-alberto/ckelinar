#include "GPSReader.h"
#include "PortDefinitions.h"
#include <SoftwareSerial.h>

const char *expectedHeader = "GPGGA,";

GPSReader::GPSReader()
: port (GPSRxPin, GPSTxPin)
{
  state = WaitForStart;
}

void GPSReader::begin()
{
  port.begin(4800);
}

void GPSReader::fetchPositionData()
{
  currentFieldReading = 0;
  currentFieldReadingPos = 0;
  state = WaitForStart;
  
  while ( state != DataOK )
    evaluateStateMachine();
}

char *GPSReader::getRawData( uint8_t field )
{
  return incommingDataBuffers[field];
}


void GPSReader::evaluateStateMachine()
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

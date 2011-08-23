#include "PortDefinitions.h"
#include "Sensirion.h"
#include "GPSReader.h"
#include "DataInterface.h"

Sensirion sensirion = Sensirion( sensirionDataPin, sensirionClockPin);
GPSReader gps;
DataInterface dataInterface;

void setup() {
  dataInterface.begin();
  gps.begin();
}

void loop() {
  
  uint16_t rawData;
  dataInterface.startDataFrame();
  dataInterface.sendData( analogRead( analogPressurePort ) );
  dataInterface.sendData( analogRead( analogTemperaturePort ) );
  dataInterface.sendData( analogRead( analogHumidityPort ) );
  
  sensirion.measTemp(&rawData);
  dataInterface.sendData(rawData);
  sensirion.measHumi(&rawData);
  dataInterface.sendData(rawData);

  gps.fetchPositionData();
  dataInterface.sendData( gps.getRawData(UTCTime) );
  dataInterface.sendData( (gps.getRawData(NSIndicator)[0]=='S') ? "-" : "" );
  dataInterface.sendData( gps.getRawData(Latitude) );
  dataInterface.sendData( (gps.getRawData(EWIndicator)[0]=='W') ? "-" : "" );
  dataInterface.sendData( gps.getRawData(Longitude) );
  dataInterface.sendData( gps.getRawData(MSLAltitude) );
  dataInterface.endDataFrame();
  delay(1000);
}


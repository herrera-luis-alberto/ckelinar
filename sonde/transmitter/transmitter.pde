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
  
  
  /*gps.fetchPositionData();
  Serial.println("S: ") ;
  for ( int i=0; i<NoDataFields; i++)
  {
    Serial.print("B:<") ;
    Serial.print( gps.incommingDataBuffers[i] );
    Serial.println(">") ;
  }
  
  return;*/
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
  dataInterface.sendData( gps.incommingDataBuffers[UTCTime] );
  dataInterface.sendData( gps.incommingDataBuffers[Latitude] );
  dataInterface.sendData( gps.incommingDataBuffers[Longitude] );
  dataInterface.sendData( gps.incommingDataBuffers[MSLAltitude] );
  dataInterface.println();
  dataInterface.endDataFrame();
  delay(1000);
}


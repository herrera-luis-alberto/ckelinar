#include "PortDefinitions.h"
#include "Sensirion.h"
#include "GPSReader.h"
#include "DataInterface.h"
#include <CameraC328R.h>
#include "avr/eeprom.h"

Sensirion sensirion = Sensirion( sensirionDataPin, sensirionClockPin);
GPSReader gps;
DataInterface dataInterface;
CameraC328R camera;

int globalCounter;


void setup() {
    globalCounter = 0;
  dataInterface.begin();
  gps.begin();
  Serial.begin(19200);
}

void getJPEGPicture_callback( uint16_t pictureSize, uint16_t packageSize, uint16_t packageCount, byte* package )
{
    dataInterface.serialImpl.print(".");
    dataInterface.writeImage( packageSize, package);
}

void readCamera(uint16_t counter)
{
    dataInterface.serialImpl.print(":");
    dataInterface.serialImpl.print("A");
    if( !camera.sync() )
      return;

    dataInterface.serialImpl.print("B");
    if( !camera.initial( CameraC328R::CT_JPEG, CameraC328R::PR_160x120, CameraC328R::JR_640x480 ) )
      return;

    dataInterface.serialImpl.print("C");
    if( !camera.setPackageSize( 64 ) )
      return;

    dataInterface.serialImpl.print("D");
    if( !camera.setLightFrequency( CameraC328R::FT_50Hz ) )
      return;

    dataInterface.serialImpl.print("E");
    if( !camera.snapshot( CameraC328R::ST_COMPRESSED, 0 ) )
      return;

    dataInterface.serialImpl.print("F");
    dataInterface.startImage( counter );
    if( !camera.getJPEGPicture( CameraC328R::PT_JPEG, PROCESS_DELAY, &getJPEGPicture_callback ) )
      return;

    dataInterface.endImage();
    dataInterface.serialImpl.print("G");

}

void loop() {
  
  uint16_t rawData;


    globalCounter = eeprom_read_dword(0);

  dataInterface.startDataFrame();

  dataInterface.sendData( globalCounter );
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

  if ( globalCounter % 256 == 0)
    readCamera( globalCounter / 256 );

    globalCounter++;

eeprom_write_dword( 0, globalCounter );

  delay(1000);
}


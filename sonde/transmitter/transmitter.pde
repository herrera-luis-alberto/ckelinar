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
int pressureConditionCounter;
int temperatureConditionCounter;
int nanoMucleReleases;


void setup() {

    globalCounter = 0;
    dataInterface.begin();
    gps.begin();
    Serial.begin(19200);
    
    pressureConditionCounter = 0;
    temperatureConditionCounter = 0;
    nanoMucleReleases = 0;
    pinMode( nanoMusclePort,OUTPUT);
    digitalWrite(nanoMusclePort,LOW);
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

void release()
{
  if ( nanoMucleReleases < 30 )
  {
    digitalWrite(nanoMusclePort,HIGH);
    delay( 5000 );
    digitalWrite(nanoMusclePort,LOW);
    
    nanoMucleReleases++;
  }
}

void checkForRelease(int pressure, uint16_t temperature )
{
    const int pressureLimit    = 200;
    const int temperatureLimit = 200;
    const int limitLimit = 5;

    if ( pressure < pressureLimit )
      pressureConditionCounter++;
    else
      pressureConditionCounter = 0;

    if ( temperature < temperatureLimit )
      temperatureConditionCounter++;
    else
      temperatureConditionCounter = 0;

    if ( pressureConditionCounter > limitLimit )
      release();

    if ( temperatureConditionCounter > limitLimit )
      release();
    
    if ( globalCounter > 780 )
      release();

}

void loop() {
  uint16_t rawData;
  int pressure;
  uint16_t temperature;

  globalCounter = eeprom_read_dword(0);

  dataInterface.startDataFrame();

  dataInterface.sendData( globalCounter );
  dataInterface.sendData( pressure = analogRead( analogPressurePort ) );
  dataInterface.sendData( analogRead( analogTemperaturePort ) );
  dataInterface.sendData( analogRead( analogHumidityPort ) );

  sensirion.measTemp(&temperature);
  dataInterface.sendData(temperature);
  sensirion.measHumi(&rawData);
  dataInterface.sendData(rawData);
  
  checkForRelease( pressure, temperature);

  gps.fetchPositionData();

  dataInterface.sendData( gps.getRawData(UTCTime) );
  dataInterface.sendData( (gps.getRawData(NSIndicator)[0]=='S') ? "-" : "" );
  dataInterface.sendData( gps.getRawData(Latitude) );
  dataInterface.sendData( (gps.getRawData(EWIndicator)[0]=='W') ? "-" : "" );
  dataInterface.sendData( gps.getRawData(Longitude) );
  dataInterface.sendData( gps.getRawData(MSLAltitude) );

  dataInterface.endDataFrame();

  if ( globalCounter % 16 == 0)
  readCamera( globalCounter / 16 );

  globalCounter++;

  eeprom_write_dword( 0, globalCounter );

  delay(3000);
}


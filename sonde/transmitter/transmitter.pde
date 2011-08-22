#include "PortDefinitions.h"
#include "Sensirion.h"
#include "DataInterface.h"

Sensirion sensirion = Sensirion( sensirionDataPin, sensirionClockPin);
DataInterface dataInterface;

void setup() {
  dataInterface.begin();
}

void loop() {
  uint16_t rawData;
  dataInterface.startDataFrame();
  dataInterface.print(":");
  dataInterface.print( analogRead( analogPressurePort ) );
  dataInterface.print("|");
  dataInterface.print( analogRead( analogTemperaturePort ) );
  dataInterface.print("|");
  dataInterface.print( analogRead( analogHumidityPort ) );
  dataInterface.print("|");
  sensirion.measTemp(&rawData);
  dataInterface.print(rawData);
  dataInterface.print("|");
  sensirion.measHumi(&rawData);
  dataInterface.print(rawData);
  dataInterface.println();
  dataInterface.startDataFrame();
  delay(1000);
}


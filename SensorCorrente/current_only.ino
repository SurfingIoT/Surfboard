// EmonLibrary examples openenergymonitor.org, Licence GNU GPL V3

#include "EmonLib.h"                   // Include Emon Library
EnergyMonitor emon1;                   // Create an instance

void setup(){  
  Serial.begin(9600);
  emon1.current(7, 47.0);             // Current: input pin, calibration.
}

void loop(){
  double Irms = emon1.calcIrms(1480);  // Calculate Irms only

  Serial.print("Potencia Aparente: ");
  Serial.print(Irms*127.0);	       // Apparent power
  Serial.println(" Watts");         

  Serial.print("Corrente Consumo: ");
  Serial.print(Irms);		       // Irms
  Serial.println(" Amperes"); 
  Serial.println("");         
  
}

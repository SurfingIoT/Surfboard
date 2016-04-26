void testDHT11(){
  while (!Serial.available()) {
    h = dht.readHumidity();
    t = dht.readTemperature();  
    if (isnan(t) || isnan(h)){
        Serial.println("Failed to read from DHT");
    }else{
      Serial.print("Humidity: ");
      Serial.print(h);
      Serial.print(" %\t");
      Serial.print("Temperature: ");
      Serial.print(t);
      Serial.println(" *C");
    }   
  }
  inChar = '0';
  printMenu();
}

char* currentOnly(char* args1) {
    double Irms = emon1.calcIrms(1480);  // Calculate Irms only
    Serial.print(Irms);
    return "";
}


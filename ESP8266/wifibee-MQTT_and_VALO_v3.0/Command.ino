void executeCommand() {
  debug("execute command " + commandString);
  if (commandString.indexOf("config-ssid") > -1) {
    String param = commandString.substring(commandString.indexOf("=") + 1, commandString.length());
    ssid = param;
    beep();
  }
  else if (commandString.indexOf("config-pwd") > -1) {
    String param = commandString.substring(commandString.indexOf("=") + 1, commandString.length());
    password = param;
    beep();
  }
  else if (commandString.indexOf("config-interval") > -1) {
    String param = commandString.substring(commandString.indexOf("=") + 1, commandString.length());
    sensor_interval = param.toInt();
    beep();
  }
  else if (commandString.indexOf("config-reconnect") > -1) {
    wifi_state = 0;
    setup_wifi();
    beep();
  }
  else if (commandString.indexOf("config-discovery") > -1) {
    discovery();
    beep();
  }
  else if (commandString.indexOf("config-save") > -1) {
    save();
    beep();
  }
  else if (commandString.indexOf("config-restore-factory") > -1) {
    restore();
    beep();
  }
  else if (commandString.indexOf("*/") > -1) {
    String t = commandString.substring(2, commandString.length());
    Serial.print(t);
  }
  else {
    Serial.print(commandString);
    delay(100);
    emptySerial();
  }
  commandString = "";
  commandComplete = false;
}

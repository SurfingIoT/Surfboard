void teste_led_RGB(){
    while (!Serial.available()) {
      digitalWrite (ledRGB_R_Pin, HIGH);  
      delay(500);
      digitalWrite (ledRGB_R_Pin, LOW);      
      digitalWrite (ledRGB_G_Pin, HIGH);  
      delay(500);
      digitalWrite (ledRGB_G_Pin, LOW);
      digitalWrite (ledRGB_B_Pin, HIGH);
      delay(500);
      digitalWrite (ledRGB_B_Pin, LOW);
      digitalWrite (TR1_Pin, HIGH);      
      delay(500);
      digitalWrite (TR1_Pin, LOW);        
      

      digitalWrite (ledRGB_R_Pin, HIGH);      
      digitalWrite (ledRGB_G_Pin, HIGH);        
      delay(500);
      digitalWrite (ledRGB_R_Pin, LOW);      
      digitalWrite (ledRGB_G_Pin, HIGH);        
      digitalWrite (ledRGB_B_Pin, HIGH);        
      delay(500);
      digitalWrite (ledRGB_G_Pin, LOW);        
      digitalWrite (ledRGB_B_Pin, HIGH);        
      digitalWrite (ledRGB_R_Pin, HIGH);      
      delay(500);
      digitalWrite (ledRGB_B_Pin, LOW);        
      digitalWrite (ledRGB_R_Pin, LOW);        
      delay(500);
  }
  inChar = '0';
  printMenu();
}

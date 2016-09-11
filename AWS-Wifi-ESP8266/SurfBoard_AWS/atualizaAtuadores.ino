void atualizaAtuadores(String comandoAtuadores){
  
  char atuadoresstring_array[100];                    //we make a char array
  char *vermelho;                                     //char pointer used in string parsing
  char *verde;                                        //char pointer used in string parsing
  char *azul;                                         //char pointer used in string parsing
  char *rele;                                         //char pointer used in string parsing
  char *check;
  //Serial.println(comandoAtuadores);
  
  comandoAtuadores.toCharArray(atuadoresstring_array, 100);   //convert the string to a char array 
  check = strtok(atuadoresstring_array, ",");                 //let's pars the array at each comma
  //Serial.println(check);
  if(strcmp("OK",check)==0){  //verifica se os dados que chegaram do ESP8266 (AWS) sao dados validos de atuadores
    //Serial.println(check);
  
    vermelho = strtok(NULL, ",");                //let's pars the array at each comma
    verde = strtok(NULL, ",");                   //let's pars the array at each comma
    azul = strtok(NULL, ",");                    //let's pars the array at each comma
    rele = strtok(NULL, ",");                    //let's pars the array at each comma

    int pwmVermelho=atoi(vermelho);
    int pwmVerde=atoi(verde);
    int pwmAzul=atoi(azul);
    int releState=atoi(rele);

    analogWrite(portaLedVermelho,pwmVermelho);  //atualiza estado dos atuadores de acordo com o que foi recebido do ESP8266 (AWS)
    analogWrite(portaLedVerde,pwmVerde);  
    analogWrite(portaLedAzul,pwmAzul);  
    digitalWrite(portaRele,releState);  

    Serial.println(cmd);  //envia dados dos sensores da SurfBoard para o ESP8266 publicar na AWS

  } 
}

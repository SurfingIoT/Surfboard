void publish_on_AWS(int *TEMP, int *HUMI, int *POT, int *LIGHT, int *GAS, int *BUTTON, int *R, int *G, int *B, int *REL) {                            //this function will pars the string  
  char sensorstring_array[50];                        //we make a char array
  char *temperatura;                                  //char pointer used in string parsing
  char *umidade;                                      //char pointer used in string parsing
  char *potenciometro;                                //char pointer used in string parsing
  char *sensorLuz;                                    //char pointer used in string parsing
  char *sensorGas;                                    //char pointer used in string parsing
  char *botao;                                        //char pointer used in string parsing
  char *dataHora;                                     //char pointer used in string parsing
//  Serial.println("************* esta e a sensorstring ****************");
//  Serial.println(sensorstring);
//  Serial.println(" ");
  
  sensorstring.toCharArray(sensorstring_array, 50);   //convert the string to a char array 
  temperatura = strtok(sensorstring_array, ",");      //let's pars the array at each comma
  umidade = strtok(NULL, ",");                        //let's pars the array at each comma
  potenciometro = strtok(NULL, ",");                  //let's pars the array at each comma
  sensorLuz = strtok(NULL, ",");                      //let's pars the array at each comma
  sensorGas = strtok(NULL, ",");                      //let's pars the array at each comma
  botao = strtok(NULL, ",");                          //let's pars the array at each comma

  *TEMP=atoi(temperatura);
  *HUMI=atoi(umidade);
  *POT=atoi(potenciometro);
  *LIGHT=atoi(sensorLuz);
  *GAS=atoi(sensorGas);
  *BUTTON=atoi(botao);

  awsSetTable(TEMP,HUMI,POT,LIGHT,GAS,BUTTON,R,G,B,REL);    //chama rotina que atualiza dados na AWS
  yieldEspCPU();
}

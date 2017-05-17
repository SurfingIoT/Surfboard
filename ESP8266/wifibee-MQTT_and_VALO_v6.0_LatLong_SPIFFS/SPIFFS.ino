void formatFS(void){
  SPIFFS.format();
}
 
void createFile(String fileName){
  File wFile;
 
  //Cria o arquivo se ele não existir
  if(SPIFFS.exists(fileName)){
    //Serial.println("Arquivo ja existe!");
  } else {
    //Serial.println("Criando o arquivo...");
    wFile = SPIFFS.open(fileName,"w+");
 
    //Verifica a criação do arquivo
    if(!wFile){
      //Serial.println("Erro ao criar arquivo!");
    } else {
      //Serial.println("Arquivo criado com sucesso!");
    }
  }
  wFile.close();
}
 
void deleteFile(String fileName) {
  //Remove o arquivo
  if(SPIFFS.remove(fileName)){
    //Serial.println("Erro ao remover arquivo!");
  } else {
    //Serial.println("Arquivo removido com sucesso!");
  }
}
 
void writeFile(String filename,String msg) {
 
  //Abre o arquivo para adição (append)
  //Inclue sempre a escrita na ultima linha do arquivo
  //Opens a file. path should be an absolute path starting with a slash (e.g. /dir/filename.txt). 
  //mode is a string specifying access mode. It can be one of "r", "w", "a", "r+", "w+", "a+". 
  //Meaning of these modes is the same as for fopen C function.
  
  File rFile = SPIFFS.open(filename,"w");
 
  if(!rFile){
    //Serial.println("Erro ao abrir arquivo!");
  } else {
    rFile.println(msg);
    //Serial.println(msg);
  }
  rFile.close();
}
 
String readFile(String fileName) {
  //Faz a leitura do arquivo
  File rFile = SPIFFS.open(fileName,"r");
  //Serial.println("Reading file...");
  while(rFile.available()) {
    String line = rFile.readStringUntil('\n');
    return line ;
    //buf += line;
    //buf += "<br />";
  }
  rFile.close();
}
 
void closeFS(void){
  SPIFFS.end();
}
 
void openFS(void){
  //Abre o sistema de arquivos
  if(!SPIFFS.begin()){
    //Serial.println("Erro ao abrir o sistema de arquivos");
  } else {
    //Serial.println("Sistema de arquivos aberto com sucesso!");
  }
}

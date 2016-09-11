void getData(int *TEMP, int *HUMI, int *POT, int *LIGHT, int *GAS, int *BUTTON, int *R, int *G, int *B, int *REL, GetItemOutput getItemOutput) {
        yieldEspCPU(); 
        MinimalMap < AttributeValue > attributeMap = getItemOutput.getItem(); /* Get the "item" from the getItem output. */
        AttributeValue av;
        attributeMap.get("Temperatura", av);  
        *TEMP = atoi(av.getS().getCStr());
        attributeMap.get("Umidade", av);
        *HUMI = atoi(av.getS().getCStr());
        attributeMap.get("Potenciometro", av);
        *POT = atoi(av.getS().getCStr());
        attributeMap.get("Luz", av);
        *LIGHT = atoi(av.getS().getCStr());
        attributeMap.get("Gas", av);
        *GAS = atoi(av.getS().getCStr());
        attributeMap.get("ChaveB2", av);
        *BUTTON = atoi(av.getS().getCStr());
        attributeMap.get("Led_Vermelho", av);
        *R = atoi(av.getS().getCStr());
        attributeMap.get("Led_Verde", av);
        *G = atoi(av.getS().getCStr());
        attributeMap.get("Led_Azul", av);
        *B = atoi(av.getS().getCStr());
        attributeMap.get("Rele", av);
        *REL = atoi(av.getS().getCStr());

        String stringAtuadores = "OK";
        stringAtuadores += ",";
        stringAtuadores += (String)*R;
        stringAtuadores += ",";
        stringAtuadores += (String)*G;
        stringAtuadores += ",";
        stringAtuadores += (String)*B;
        stringAtuadores += ",";
        stringAtuadores += (String)*REL;
        stringAtuadores += ",";

        Serial.println(stringAtuadores);  //envia dados dos atuadores recebidos do AWS para SurfBoard
}

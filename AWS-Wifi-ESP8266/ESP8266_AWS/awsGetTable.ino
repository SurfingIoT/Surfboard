void awsGetTable(int *TEMP, int *HUMI, int *POT, int *LIGHT, int *GAS, int *BUTTON, int *R, int *G, int *B, int *REL) {
    
    // Set the string and number values for the range and hash Keys, respectively.
    hashKey.setS(HASH_KEY_VALUE);
    rangeKey.setN(RANGE_KEY_VALUE);

    //////////////////////////////////////////////////////////////////////////////////////////
    // Create key-value pairs out of the hash and range keys, and create a map out off them, 
    // which is the key. 
    //////////////////////////////////////////////////////////////////////////////////////////
    MinimalKeyValuePair < MinimalString, AttributeValue > pair1(HASH_KEY_NAME, hashKey);
    MinimalKeyValuePair < MinimalString, AttributeValue > pair2(RANGE_KEY_NAME, rangeKey);
    MinimalKeyValuePair<MinimalString, AttributeValue> keyArray[] = { pair1, pair2 };
    getItemInput.setKey(MinimalMap < AttributeValue > (keyArray, KEY_SIZE));

    // Looking to get the R G and B values 
    MinimalString attributesToGet[] = { "Temperatura", "Umidade", "Potenciometro", "Luz", "Gas", "ChaveB2", "Led_Vermelho", "Led_Verde", "Led_Azul", "Rele" };
    getItemInput.setAttributesToGet(MinimalList < MinimalString > (attributesToGet, 10));

    // Set Table Name
    getItemInput.setTableName(TABLE_NAME);

    // Perform getItem and check for errors. 
    GetItemOutput getItemOutput = ddbClient.getItem(getItemInput, actionError); //consulta dados na AWS

    /*
    //////////////////////////////////////////////////////////
    // AWS DynamoDB get/set serial message header
    //////////////////////////////////////////////////////////

    ddbClient.getESPtime(szInfo);  //debug getTime
    Serial.print("------------------------------------------\n");
    Serial.print("Loop start time:  ");
    Serial.print(szInfo);
    Serial.print(" GMT\n");
    
    Serial.print("Current Free Heap:");
    Serial.println(system_get_free_heap_size());
    Serial.println("------------------------------------------\n");
    */
    
    //////////////////////////////////////////////////////////
    // AWS HTTP Request/Response (uncomment for debug)
    //////////////////////////////////////////////////////////
    /************************************************
    Serial.println("AWS Request:\n--------------\n");
    Serial.println(ddbClient.szR); //HTTP Request

    Serial.println("AWS Reply:\n--------------\n");
    Serial.println(ddbClient.szResponse); //HTTP Response
    /*************************************************/
    
    switch (actionError) {
    case NONE_ACTIONERROR:
        getData(TEMP,HUMI,POT,LIGHT,GAS,BUTTON,R,G,B,REL,getItemOutput);
        break;
        
    case INVALID_REQUEST_ACTIONERROR:
        Serial.print("ERROR: ");
        Serial.println(getItemOutput.getErrorMessage().getCStr());
        break;
    case MISSING_REQUIRED_ARGS_ACTIONERROR:
        Serial.println("ERROR: Required arguments were not set for GetItemInput");
        break;
    case RESPONSE_PARSING_ACTIONERROR:
        Serial.println("ERROR: Problem parsing http response of GetItem\n");
        break;
    case CONNECTION_ACTIONERROR:
        Serial.println("ERROR: Connection problem");
        break;
    }
}

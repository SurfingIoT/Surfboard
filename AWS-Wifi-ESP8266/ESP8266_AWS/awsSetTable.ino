void awsSetTable(int *TEMP, int *HUMI, int *POT, int *LIGHT, int *GAS, int *BUTTON, int *R, int *G, int *B, int *REL) {
    //Now lets change the RGB color values and put them to DynamoDB
    // Create an Item. //
    AttributeValue deviceValue;
    deviceValue.setS(HASH_KEY_VALUE);
    AttributeValue tempValue;
    AttributeValue humiValue;
    AttributeValue potValue;
    AttributeValue lightValue;
    AttributeValue gasValue;
    AttributeValue buttonValue;
    AttributeValue redValue;
    AttributeValue greenValue;
    AttributeValue blueValue;
    AttributeValue releValue;
    
    //Increment and set color
    String(*TEMP).toCharArray(szT,10);
    tempValue.setS(szT);
    String(*HUMI).toCharArray(szT,10);
    humiValue.setS(szT);
    String(*POT).toCharArray(szT,10);
    potValue.setS(szT);
    String(*LIGHT).toCharArray(szT,10);
    lightValue.setS(szT);
    String(*GAS).toCharArray(szT,10);
    gasValue.setS(szT);
    String(*BUTTON).toCharArray(szT,10);
    buttonValue.setS(szT);

    String(*R).toCharArray(szT,10);
    redValue.setS(szT);
    String(*G).toCharArray(szT,10);
    greenValue.setS(szT);
    String(*B).toCharArray(szT,10);
    blueValue.setS(szT);
    String(*REL).toCharArray(szT,10);
    releValue.setS(szT);

    MinimalKeyValuePair < MinimalString, AttributeValue > att1(HASH_KEY_NAME, hashKey);
    MinimalKeyValuePair < MinimalString, AttributeValue > att2(RANGE_KEY_NAME, rangeKey);
    MinimalKeyValuePair < MinimalString, AttributeValue > att3("Temperatura", tempValue);
    MinimalKeyValuePair < MinimalString, AttributeValue > att4("Umidade", humiValue);
    MinimalKeyValuePair < MinimalString, AttributeValue > att5("Potenciometro", potValue);
    MinimalKeyValuePair < MinimalString, AttributeValue > att6("Luz", lightValue);
    MinimalKeyValuePair < MinimalString, AttributeValue > att7("Gas", gasValue);
    MinimalKeyValuePair < MinimalString, AttributeValue > att8("ChaveB2", buttonValue);

    MinimalKeyValuePair < MinimalString, AttributeValue > att9("Led_Vermelho", redValue);
    MinimalKeyValuePair < MinimalString, AttributeValue > att10("Led_Verde", greenValue);
    MinimalKeyValuePair < MinimalString, AttributeValue > att11("Led_Azul", blueValue);
    MinimalKeyValuePair < MinimalString, AttributeValue > att12("Rele", releValue);

    
    MinimalKeyValuePair < MinimalString, AttributeValue > itemArray[] = { att1, att2, att3, att4, att5, att6, att7, att8, att9, att10, att11, att12};

    // Set values for putItemInput. //
    putItemInput.setItem(MinimalMap < AttributeValue > (itemArray, 12));
    putItemInput.setTableName(TABLE_NAME);

    // perform putItem and check for errors. //
    PutItemOutput putItemOutput = ddbClient.putItem(putItemInput, actionError); //envia dados para AWS
    switch (actionError) {
        case NONE_ACTIONERROR:
 
            delay(1);
            break;
        case INVALID_REQUEST_ACTIONERROR:
            Serial.print("ERROR: ");
            Serial.println(putItemOutput.getErrorMessage().getCStr());
            break;
        case MISSING_REQUIRED_ARGS_ACTIONERROR:
            Serial.println(
                    "ERROR: Required arguments were not set for PutItemInput");
            break;
        case RESPONSE_PARSING_ACTIONERROR:
            Serial.println("ERROR: Problem parsing http response of PutItem");
            break;
        case CONNECTION_ACTIONERROR:
            Serial.println("ERROR: Connection problem");
            break;
    }
}

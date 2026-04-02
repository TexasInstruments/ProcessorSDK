/*
* The MIT License (MIT)
*
* Copyright (C) 2024 Define Design Deploy Corp.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of
* this software and associated documentation files (the “Software”), to deal in
* the Software without restriction, including without limitation the rights to use,
* copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the
* Software, and to permit persons to whom the Software is furnished to do so,
* subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
* HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
* OTHER DEALINGS IN THE SOFTWARE.
*/

/*!
 * @file        d3_eeprom.c
 * @brief       eeprom cameras
 * @details     Routines for accessing eeprom and parsing information about cameras 
 * @copyright   Copyright (C) 2024 Define Design Deploy Corp. All Rights Reserved.
 */

#define JSMN_HEADER
#include <jsmn.h>

#include "d3_eeprom.h"
#include <iss_sensors.h>
#include <iss_sensor_priv.h>

// If we have more than 32 JSON tokens in the EEPROM, make this more
#define EEPROM_MAX_TOKENS (32)

#define DEBUG_PRINTF    appLogPrintf

// 8192 + null termination
static jsmntok_t TOKENS[EEPROM_MAX_TOKENS];
static char eepromBuffer[8193];

void print_key(char *key_string, size_t key_len);

#ifndef SKIP_EEPROM

camera_type_t getCameraTypeFromEEPROM(void* sensorI2cHandle, uint32_t devAddr) {
    camera_type_t cameraDetected;
    // Get contents of eeprom
    int32_t status = 0;
    uint8_t sensorI2cByteOrder = 255U;
    char str_type_lens[20];

    // Look for parent token "module", "part_number"
    const char parentTokenString[6] = "module";
    const char partNumberString[11] = "part_number";

    (void)memset(&eepromBuffer, 0, sizeof(eepromBuffer));
    
    status = Board_i2c16BitRegRd(sensorI2cHandle, devAddr, 0,(uint8_t *) &eepromBuffer, (uint8_t) sizeof(eepromBuffer)-1, sensorI2cByteOrder, SENSOR_I2C_TIMEOUT);
    if (status != 0) {
        appLogPrintf("EEPROM could not be read");
        appLogPrintf("I2C Timeout while reading devAddr 0x%x \n", devAddr);
        return UNRECOGNIZED;
    }
    
    
    DEBUG_PRINTF("\n getCameraTypeFromEEPROM -> %s \n",eepromBuffer);
    DEBUG_PRINTF("\nsize of jsmntok_t: %d\n", sizeof(jsmntok_t));
    DEBUG_PRINTF("JSMN_UNDEFINED = %d\n", JSMN_UNDEFINED);
    DEBUG_PRINTF("JSMN_OBJECT = %d\n", JSMN_OBJECT);
    DEBUG_PRINTF("JSMN_ARRAY = %d\n", JSMN_ARRAY);
    DEBUG_PRINTF("JSMN_STRING = %d\n", JSMN_STRING);
    DEBUG_PRINTF("JSMN_PRIMITIVE = %d\n", JSMN_PRIMITIVE);


    // Use jsmn to parse the contents of the eeprom
    int8_t jsonParserResultCode;
    uint32_t num_tokens = 0;
    jsmn_parser p;
    
    jsmn_init(&p);
    jsonParserResultCode = jsmn_parse(&p, eepromBuffer, strlen(eepromBuffer), TOKENS, EEPROM_MAX_TOKENS);
    // Provide some feedback about parsing the contents of the eeprom
    if (jsonParserResultCode < 0) {
        appLogPrintf("\n Failed to parse JSON: %d\n", jsonParserResultCode);
        return UNRECOGNIZED;
    }
    else {
        appLogPrintf("\n getCameraTypeFromEEPROM - EEPROM JSON Parser found %d tokens \n", jsonParserResultCode);
        num_tokens = jsonParserResultCode;
    }

    jsmntok_t *key;
    int32_t length;
    int32_t partNumIndex;
    unsigned char stop = 0;

    for (int32_t i = 0; i < num_tokens; i++) {
        key = &TOKENS[i];
        length = key->end - key->start;

        DEBUG_PRINTF("\n");
        DEBUG_PRINTF("key.type: %d\n", key->type);
        DEBUG_PRINTF("key.start: %d\n", key->start);
        DEBUG_PRINTF("key.end: %d\n", key->end);
        DEBUG_PRINTF("key.size: %d\n", key->size);
        DEBUG_PRINTF("length: %d\n", length);

        if ((stop) || (length < 1)) {
            DEBUG_PRINTF("Stop or length negative - stopping\n");
            break;
        }

        char *keyString = &eepromBuffer[key->start];
        DEBUG_PRINTF("\n getCameraTypeFromEEPROM -> Key1 :");
        print_key(keyString, length);
        DEBUG_PRINTF("\n");
        /*
        if (key->type != JSMN_STRING)
            continue;
        */

        if (strncmp(keyString, parentTokenString,sizeof(parentTokenString)) == 0)
        {
            DEBUG_PRINTF("Key string matched parent token string:\n");
            DEBUG_PRINTF("Key string: '");
            print_key(keyString, length);
            DEBUG_PRINTF("'\n");
            DEBUG_PRINTF("Parent string: %s", parentTokenString);
            for (int32_t j = i+1; j < num_tokens; j++)
            {
                key = &TOKENS[j];
                length = key->end - key->start;
                keyString = &eepromBuffer[key->start];

                /*
                if (key->type != JSMN_STRING)
                    continue;
                */

                DEBUG_PRINTF("\n getCameraTypeFromEEPROM -> Key2 :");
                print_key(keyString, length);
                DEBUG_PRINTF("\n");

                if (strncmp(keyString, partNumberString, sizeof(partNumberString)) == 0)
                {
                    key++;
                    char *value_str = &eepromBuffer[key->start];
                    length = key->end - key->start;
                    length = (length > sizeof(str_type_lens)) ? sizeof(str_type_lens) : length;
                    DEBUG_PRINTF("\n getCameraTypeFromEEPROM -> val2 :");
                    print_key(keyString, length);
                    DEBUG_PRINTF("\n");

                    strncpy(str_type_lens, value_str, length);
                    DEBUG_PRINTF("\n getCameraTypeFromEEPROM -> string -> %s \n",str_type_lens);
                    stop = 1;
                    break;
                }
            }
        }
    }
    
    if (strncmp(&str_type_lens[0], IMX390_WIDE_UNSEALED,16) == 0 || strncmp(&str_type_lens[0], IMX390_WIDE_PRO,18) == 0) {
            cameraDetected = IMX390_WIDE;
            appLogPrintf ("\r\n getCameraTypeFromEEPROM - wide \r\n");
    }
    else if (strncmp(&str_type_lens[0], IMX390_MID_UNSEALED,16) == 0 || strncmp(&str_type_lens[0], IMX390_MID_PRO,18) == 0) {
        cameraDetected = IMX390_MID;
        appLogPrintf ("\r\n getCameraTypeFromEEPROM  mid \r\n");
    }
    else if (strncmp(&str_type_lens[0], IMX390_NARROW_UNSEALED,16) == 0 || strncmp(&str_type_lens[0], IMX390_NARROW_PRO,18) == 0) {
        cameraDetected = IMX390_NARROW;
        appLogPrintf ("\r\n getCameraTypeFromEEPROM narrow \r\n");
    }
    else {
        appLogPrintf ("\r\n getCameraTypeFromEEPROM erro \r\n");
        cameraDetected = UNRECOGNIZED;
    }
    return cameraDetected;
}

void print_key(char *key_string, size_t key_len)
{
    for(int32_t k = 0; k < key_len; k++) {
        DEBUG_PRINTF("%c",key_string[k]);
    }
}

#endif
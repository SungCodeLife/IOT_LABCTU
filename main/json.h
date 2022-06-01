/* ---------------------------------------------------------------------------------------------- */
/*  File name:    json.h                                                                          */
/*  Description:  functions                                                                       */
/*  Version:      1.0.0                                                                           */
/*  Date:         24/02/2022                                                                      */
/* ---------------------------------------------------------------------------------------------- */
#ifndef __JSON_H_
#define __JSON_H_

/* ---------------------------------------------------------------------------------------------- */
/*                                         include Library                                        */
/* ---------------------------------------------------------------------------------------------- */

#include "user_cfg.h"
#include <ArduinoJson.h>  // JSON version 6.19.0 by Beniot Blanchon

/* ---------------------------------------------------------------------------------------------- */
/*                                       prototype function                                       */
/* ---------------------------------------------------------------------------------------------- */

void Json_parse_void(String Data, float *Caliration_Para);
String Json_Create_String(uint8_t request, float *json_save);
String Json_CreateAnalog_String(uint8_t request, uint16 *JsonSave);

/* ---------------------------------------------------------------------------------------------- */
/*                                            Function                                            */
/* ---------------------------------------------------------------------------------------------- */

/*******************************************************************************
* @FunctionName:    Json_parse_uint8                                       
* @Description:     pare json get numeric of configuration or get parametter calibration        
* @Param:           String Data
* @Return:          uint8                                                       
*******************************************************************************/
uint8 Json_parse_uint8(String Data)
{
    char jsondata[Data.length()];
    for (int i = 0; i <= Data.length(); ++i)
        jsondata[i] = Data[i];
    StaticJsonDocument <256> doc;
    deserializeJson(doc, jsondata);

    uint8 RequestFServer = doc["task"];
    if (RequestFServer == JSON_NUMERIC_ByServer)
    {
        #if DEBUG_JSON
            SHOWLN(F("JSON SERVER: numeric configuration"));
        #endif
        return 1;
    }
    if (RequestFServer == JSON_CALIB_ByServer)
    {
        #if DEBUG_JSON
            SHOWLN(F("JSON SERVER: calibration"));
        #endif
        return 2;
    }

}

/*******************************************************************************
* @FunctionName:    Json_Create_String                                            
* @Description:     create json: analog json, calibration json, value sensor json      
* @Param:           uint8_t request, float *Caliration_Para, float *Analog_Get, float *Value_Sensor    
* @Return:          String                                                       
*******************************************************************************/
String Json_Create_String(uint8_t request, float *json_save)
{
    /* SHOW json to String output in Serial */
    String output;
    char jsondata[1];
    StaticJsonDocument <256> doc;
    deserializeJson(doc, jsondata);

    if (request == JSON_CALIB_byClient)
    {
        doc["config"]   = EEPROM.read(EEPROM_ADDRESS);
        doc["SAL"][0]   = json_save[SAL_Intercept];
        doc["SAL"][1]   = json_save[SAL_Slope];
        doc["PH"][0]    = json_save[PH_Intercept];
        doc["PH"][1]    = json_save[PH_Slope];
        doc["NH4"][0]   = json_save[NH4_Eo];
        doc["NH4"][1]   = json_save[NH4_m];
        doc["OXI"][0]   = json_save[DO_Intercept];
        doc["OXI"][1]   = json_save[DO_Slope];
        doc["clientID"]     = 0;
        doc["task"]         = 0;
        serializeJson(doc, output);
        #if DEBUG_JSON
            SHOWLN("json created: " + output);
        #endif
    }
    else if (request == JSON_VALUE_SENSOR_byClient)
    {
        doc["Value_Temp"]  = json_save[TEMP];
        doc["Value_SAL"]   = json_save[SAL];
        doc["Value_PH"]    = json_save[PH];
        doc["Value_NH4"]   = json_save[NH4];
        doc["Value_DO"]    = json_save[DO];
        doc["clientID"]    = 0;
        doc["task"]        = 0;
        serializeJson(doc, output);
        #if DEBUG_JSON
            SHOWLN("json created: " + output);
        #endif
    }
    return output;
}

String Json_CreateAnalog_String(uint8_t request, uint16 *JsonSave)
{
    String output;
    char jsondata[1];
    StaticJsonDocument <256> doc;
    deserializeJson(doc, jsondata);

    if (request == JSON_ANALOG_byClient)
    {
        doc["TEMP_vol"] = (JsonSave[TEMP*3+1] + JsonSave[TEMP*3+2] + JsonSave[TEMP*3+3]) / NumberTimes_ReadAnalog;
        doc["SAL_vol"]  = (JsonSave[SAL*3+1] + JsonSave[SAL*3+2] + JsonSave[SAL*3+3]) / NumberTimes_ReadAnalog;
        doc["PH_vol"]   = (JsonSave[PH*3+1] + JsonSave[PH*3+2] + JsonSave[PH*3+3]) / NumberTimes_ReadAnalog;
        doc["NH4_vol"]  = (JsonSave[NH4*3+1] + JsonSave[NH4*3+2] + JsonSave[NH4*3+3]) / NumberTimes_ReadAnalog;
        doc["OXI_vol"]  = (JsonSave[DO*3+1] + JsonSave[DO*3+2] + JsonSave[DO*3+3]) / NumberTimes_ReadAnalog;
        doc["clientID"]     = 0;
        doc["task"]         = 0;
        serializeJson(doc, output);
        #if DEBUG_JSON
            SHOWLN("json created: " + output);
        #endif
    }
    else if(request == JSON_ANAOG_DETAIL_byClient)
    {
        doc["TEMP"][0]  = JsonSave[1];
        doc["TEMP"][1]  = JsonSave[2];
        doc["TEMP"][2]  = JsonSave[3];
        doc["SAL"][0]   = JsonSave[4];
        doc["SAL"][1]   = JsonSave[5];
        doc["SAL"][2]   = JsonSave[6];
        doc["PH"][0]    = JsonSave[7];
        doc["PH"][1]    = JsonSave[8];
        doc["PH"][2]    = JsonSave[9];
        doc["NH4"][0]   = JsonSave[10];
        doc["NH4"][1]   = JsonSave[11];
        doc["NH4"][2]   = JsonSave[12];
        doc["OXI"][0]   = JsonSave[13];
        doc["OXI"][1]   = JsonSave[14];
        doc["OXI"][2]   = JsonSave[15];
        doc["clientID"]     = 0;
        doc["task"]         = 0;
        serializeJson(doc, output);
        #if DEBUG_JSON
            SHOWLN("json created: " + output);
        #endif
    }
    return output;
}

#endif

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

    if (request == JSON_ANALOG_byClient)
    {
        doc["TEMP_vol"] = json_save[TEMP];
        doc["SAL_vol"]  = json_save[SAL];
        doc["PH_vol"]   = json_save[PH];
        doc["NH4_vol"]   = json_save[NH4];
        doc["OXI_vol"]  = json_save[DO];
        doc["clientID"]     = 0;
        doc["task"]         = 0;
        serializeJson(doc, output);
        #if DEBUG_JSON
            SHOWLN("json created: " + output);
        #endif
    }
    else if (request == JSON_CALIB_byClient)
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
    else if(request == JSON_ANAOG_DETAIL_byClient)
    {
        doc["TEMP"][0]  = json_save[0];
        doc["TEMP"][1]  = json_save[0];
        doc["TEMP"][2]  = json_save[0];
        doc["SAL"][0]   = json_save[0];
        doc["SAL"][1]   = json_save[0];
        doc["SAL"][2]   = json_save[0];
        doc["PH"][0]    = json_save[0];
        doc["PH"][1]    = json_save[0];
        doc["PH"][2]    = json_save[0];
        doc["NH4"][0]   = json_save[0];
        doc["NH4"][1]   = json_save[0];
        doc["NH4"][2]   = json_save[0];
        doc["OXI"][0]   = json_save[0];
        doc["OXI"][1]   = json_save[0];
        doc["OXI"][2]   = json_save[0];
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

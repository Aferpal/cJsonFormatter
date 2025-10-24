#ifndef __JSON_H__
#define __JSON_H__
#include "./JSONItemList.h"


typedef struct JSON{
    JSONItemList* items;
}JSON;

JSON* loadJsonFromFile(char* filename);

JSON* formatJsonFromString(char* json_as_string);

int getAsInt(JSON* json, char* name);

double getAsDouble(JSON* json, char* name);

char* getAsString(JSON* json, char* name);

JSON* getAsObject(JSON* json, char* name);

#endif
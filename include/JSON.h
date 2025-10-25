#ifndef __JSON_H__
#define __JSON_H__
#include "./JSONItemList.h"


typedef struct JSON{
    JSONItemList* items;
}JSON;

JSON* loadJsonFromFile(char* filename);

JSON* formatJsonFromString(char* json_as_string);

//setters

void setAsInt(JSON* json, char* name, int value);

void setAsDouble(JSON* json, char* name, double value);

void setAsString(JSON* json, char* name, char* value);

void setAsObject(JSON* json, char* name, JSON* value);

//getters

int getAsInt(JSON* json, char* name);

double getAsDouble(JSON* json, char* name);

char* getAsString(JSON* json, char* name);

JSON* getAsObject(JSON* json, char* name);



//other public functions

void printJson(JSON*);

void exportJson(JSON*, char*);


//destructor

void freeJson(JSON*);

#endif
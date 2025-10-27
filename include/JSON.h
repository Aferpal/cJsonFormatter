#ifndef __JSON_H__
#define __JSON_H__
#include "./JSONItemList.h"


typedef struct JSON{
    JSONItemList* items;
}JSON;

typedef JSON* json_t;

json_t createJsonEmpty();

json_t createJsonFromString(char*);

json_t loadJsonFromFile(char* filename);

json_t formatJsonFromString(char* json_as_string);

//setters

void setAsInt(json_t json, char* name, int value);

void setAsDouble(json_t json, char* name, double value);

void setAsString(json_t json, char* name, char* value);

void setAsObject(json_t json, char* name, json_t value);

//getters

int getAsInt(json_t json, char* name);

double getAsDouble(json_t json, char* name);

char* getAsString(json_t json, char* name);

json_t getAsObject(json_t json, char* name);



//other public functions

void printJson(json_t);

void exportJson(json_t, char*);


//destructor

void freeJson(json_t);

#endif
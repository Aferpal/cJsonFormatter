#ifndef __JSON_H__
#define __JSON_H__
#include "./JSONItemList.h"


typedef struct JSON{
    JSONItemList* items;
}JSON;

typedef JSON* json_t;

typedef JSONList* json_list_t;

json_t createJsonEmpty();

json_t createJsonFromString(const char*);

json_t loadJsonFromFile(char* filename);

//setters

void setAsNumber(json_t json, char* name, double value);

void setAsString(json_t json, char* name, char* value);

void setAsObject(json_t json, char* name, json_t value);

void setAsList(json_t json, char* name, json_list_t value);

//getters

double getAsNumber(json_t json, char* name);

char* getAsString(json_t json, char* name);

json_t getAsObject(json_t json, char* name);

json_list_t getAsList(json_t json, char* name);



//other public functions

void printJson(json_t);

void exportJson(json_t, const char*);

char* jsonToString(json_t);


//destructor

void freeJson(json_t);

#endif
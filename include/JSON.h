#ifndef __JSON_H__
#define __JSON_H__

typedef struct JSONValue JSONValue;

typedef JSONValue* json_t;

json_t createJsonFromString(const char*);

json_t loadJsonFromFile(char* filename);


//other public functions

void printJson(json_t);

void exportJson(json_t, const char*);

char* jsonToString(json_t);


//destructor

void freeJson(json_t*);

#endif
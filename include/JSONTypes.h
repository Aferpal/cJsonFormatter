#ifndef __JSON_TYPES_H__
#define __JSON_TYPES_H__

typedef struct JSON JSON;
typedef struct JSONList JSONList;

enum JSONType{NUMBER, STRING, OBJECT, LIST, NONE};

typedef union JSONValueData{
    double numbervalue;
    char* stringvalue;
    JSON* objectvalue;
    JSONList* listvalue;
}JSONValueData;

typedef struct JSONValue{
    enum JSONType type;
    JSONValueData data;
} JSONValue;


typedef struct JSONList{
    int _size;
    int _reserved;
    JSONValue* _data;
}JSONList;

JSONList* createJsonList();

JSONList* createJsonListWithSize(int);

JSONValue get(JSONList*, int);

void append(JSONList*, JSONValue);

void update(JSONList*, int, JSONValue);

int length(JSONList*);

#endif
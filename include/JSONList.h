#ifndef _JSON_LIST_H_
#define _JSON_LIST_H_

typedef struct JSONValue JSONValue;

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
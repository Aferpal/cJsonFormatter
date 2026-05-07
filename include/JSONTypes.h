#ifndef __JSON_TYPES_H__
#define __JSON_TYPES_H__

typedef struct JSON JSON;
typedef struct JSONList JSONList;

typedef JSON* json_obj_t;
typedef JSONList* json_list_t;

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



/* JSON LISTS */

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




/* JSON OBJECTS */

/* JSON ITEM LIST, JSON KEY-VALUE TREE */
typedef struct JSONItem{
    char* name;
    JSONValue value;
}JSONItem;


typedef struct JSONItemList{ //binary search tree
    JSONItem item;
    struct JSONItemList* right, *left;
}JSONItemList;


JSONItemList* addJSONItem(JSONItemList** list, JSONItem item);

JSONItemList* addJSONStringItem(JSONItemList** list, char* name, char* value);

JSONItemList* addJSONNumberItem(JSONItemList** list, char* name, double value);

JSONItemList* addJSONObjectItem(JSONItemList** list, char* name, JSON* value);

JSONItemList* addJSONListItem(JSONItemList** list, char* name, JSONList* value);





double getJSONItemByNameAsNumber(JSONItemList** list, char* name);

char* getJSONItemByNameAsString(JSONItemList** list, char* name);

JSON* getJSONItemByNameAsObject(JSONItemList** list, char* name);

JSONList* getJSONItemByNameAsList(JSONItemList** list, char* name);



/* PROPER JSON OBJECT DEFINITIONS*/

typedef struct JSON{
    JSONItemList* items;
}JSON;

json_obj_t createJsonEmpty();

//setters

void setAsNumber(json_obj_t json, char* name, double value);

void setAsString(json_obj_t json, char* name, char* value);

void setAsObject(json_obj_t json, char* name, json_obj_t value);

void setAsList(json_obj_t json, char* name, json_list_t value);

//getters

double getAsNumber(json_obj_t json, char* name);

char* getAsString(json_obj_t json, char* name);

json_obj_t getAsObject(json_obj_t json, char* name);

json_list_t getAsList(json_obj_t json, char* name);

#endif
#ifndef __JSONItemList__H__
#define __JsONItemList__H__

typedef struct JSON;

enum JSONType{INT, DOUBLE, STRING, OBJECT};


typedef union JSONValue{
    int intvalue;
    double doublevalue;
    char* stringvalue;
    struct JSON* objectvalue;
}JSONValue;

typedef struct JSONItem{
    char* name;
    JSONValue value;
    enum JSONType type;
}JSONItem;


typedef struct JSONItemList{ //binary search tree
    JSONItem item;
    struct JSONItemList* right, *left;
}JSONItemList;


JSONItemList* addJSONItem(JSONItemList** list, JSONItem item);

JSONItemList* addJSONStringItem(JSONItemList** list, char* name, char* value);

JSONItemList* addJSONIntItem(JSONItemList** list, char* name, int value);

JSONItemList* addJSONDoubleItem(JSONItemList** list, char* name, double value);

JSONItemList* addJSONObjectItem(JSONItemList** list, char* name, struct JSON* value);





JSONItem* getJSONItemByName(JSONItemList** list, char* name);

int getJSONItemByNameAsInt(JSONItemList** list, char* name);

double getJSONItemByNameAsDouble(JSONItemList** list, char* name);

char* getJSONItemByNameAsString(JSONItemList** list, char* name);

struct JSON* getJSONItemByNameAsObject(JSONItemList** list, char* name);




#endif
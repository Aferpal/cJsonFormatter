#ifndef __JSONItemList__H__
#define __JSONItemList__H__

#include "JSONTypes.h"

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



#endif
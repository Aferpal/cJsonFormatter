#ifndef __JSONItemList__H__
#define __JsONItemList__H__


enum JSONType{INT, DOUBLE, STRING, OBJECT};

typedef struct JSON;

typedef union JSONValue{
    int intvalue;
    double doublevalue;
    char* stringvalue;
    JSON* objectvalue;
}JSONValue;

typedef struct JSONItem{
    char* name;
    JSONValue value;
    JSONType type;
}JSONItem;


typedef struct JSONItemList{ //binary search tree
    
}JSONItemList;



#endif
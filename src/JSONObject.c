#include<JSONTypes.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

// constructor 

json_obj_t createJsonEmpty(){
    json_obj_t json = (JSON*)malloc(sizeof(JSON));
    json->items = NULL;
    return json;
}

//setters

void setAsNumber(json_obj_t json, char* name, double value){
    addJSONNumberItem(&(json->items), name, value);
}

void setAsString(json_obj_t json, char* name, char* value){
    addJSONStringItem(&(json->items), name, value);
}

void setAsObject(json_obj_t json, char* name, json_obj_t value){
    addJSONObjectItem(&(json->items), name, value);
}

void setAsList(json_obj_t json, char* name, json_list_t value){
    addJSONListItem(&(json->items), name, value);
}

//getters

double getAsNumber(json_obj_t json, char* name){
    return getJSONItemByNameAsNumber(&(json->items), name);
}

char* getAsString(json_obj_t json, char* name){
    return getJSONItemByNameAsString(&(json->items), name);
}

json_obj_t getAsObject(json_obj_t json, char* name){
    return getJSONItemByNameAsObject(&(json->items), name);
}

json_list_t getAsList(json_obj_t json, char* name){
    return getJSONItemByNameAsList(&(json->items), name);
}



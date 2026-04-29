#include<JSONTypes.h>
#include<stdlib.h>

JSONList* createJsonList(){
    JSONList* list = (JSONList*)malloc(sizeof(JSONList));
    list->_data = NULL;
    list->_reserved = 0;
    list->_size = 0;

    return list;
}

JSONList* createJsonListWithSize(int size){
    JSONList* list = createJsonList();
    list->_data = (JSONValue*)malloc(sizeof(JSONValue) * size);
    list->_reserved = size;
    list->_size = size;

    return list;
}

JSONValue get(JSONList* list, int pos){

    JSONValue value = {__INT32_MAX__};
    if( length(list) <= pos ){
        return value;
    }

    return list->_data[pos];

}

void resize(JSONList* list){
    
    int new_length = list->_reserved * 2;

    list->_data = realloc(list->_data, new_length);

    if( list->_data == NULL ){
        list->_size = 0;
        list->_reserved = 0;
    }

    list->_reserved = new_length;

}

void append(JSONList* list, JSONValue val){

    if( list->_reserved == list->_size ){
        resize(list);
    }

    list->_data[list->_size++] = val;

}

void update(JSONList* list, int pos, JSONValue val){
 
    if( length(list) <= pos ){
        return;
    }

    list->_data[pos] = val;

}

int length(JSONList* list){
    return list->_size;
}
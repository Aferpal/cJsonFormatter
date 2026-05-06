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
    list->_size = 0;

    return list;
}

JSONValue get(JSONList* list, int pos){

    JSONValue value = {__INT32_MAX__};
    if( length(list) <= pos ){
        return value;
    }

    return list->_data[pos];

}

int resize(JSONList* list){
    
    int new_length = list->_reserved == 0 ? 2: list->_reserved* 2;

    JSONValue* tmp = (JSONValue*)realloc(list->_data, new_length * sizeof(JSONValue));

    if( tmp == NULL ){
        list->_size = 0;
        list->_reserved = 0;
        return -1;
    }

    list->_data = tmp;

    list->_reserved = new_length;

    return 0;

}

void append(JSONList* list, JSONValue val){

    if( list->_reserved == list->_size ){
        if(resize(list) == -1){
            return;
        };
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

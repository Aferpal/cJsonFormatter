#include<JSONItemList.h>
#include<string.h>
#include<stdlib.h>

JSONItemList** getTheoricalNodeByName(JSONItemList** runner, char* name){

    if(name == NULL || runner == NULL ){
        return NULL;
    }
    int cmp = -1;


    while((*runner) != NULL && cmp != 0){

        cmp = strcmp(name, (*runner)->item.name);

        if(cmp > 0){

            runner = &((*runner)->right);

        }else if(cmp < 0){

            runner = &((*runner)->left);

        }

    }

    return runner;
}

JSONItemList* addJSONItem(JSONItemList** list, JSONItem item){
    JSONItemList** runner = getTheoricalNodeByName(list, item.name);

    //if error abort and return null
    if(runner == NULL){
        return NULL;
    }

    //if there was no item with the same name
    if( *runner == NULL){
        //allocate memory for the new item
        *runner = (JSONItemList*)malloc(sizeof(JSONItemList));

        //build the item
        (*runner)->left = NULL;
        (*runner)->right = NULL;
        (*runner)->item.name = item.name;
        (*runner)->item.value = item.value;

    }else{  //if there was an item with same name we overwrite it
        (*runner)->item.name = item.name;
        (*runner)->item.value = item.value;
    }

    return *runner;
    
    
}

JSONItemList* addJSONStringItem(JSONItemList** list, char* name, char* value){
    JSONItem item = {
        .name = strdup(name),
        .value = {
            .type = STRING,
            .data.stringvalue = strdup(value)
        }
    };

    return addJSONItem(list, item);
}

JSONItemList* addJSONNumberItem(JSONItemList** list, char* name, double value){
    JSONItem item = {
        .name = strdup(name),
        .value = {
            .type = NUMBER,
            .data.numbervalue = value
        }
    };

    return addJSONItem(list, item);
}

JSONItemList* addJSONObjectItem(JSONItemList** list, char* name, struct JSON* value){
    JSONItem item = {
        .name = strdup(name),
        .value = {
            .type = OBJECT,
            .data.objectvalue = value
        }
    };

    return addJSONItem(list, item);
}

JSONItemList* addJSONListItem(JSONItemList** list, char* name, JSONList* value){
    JSONItem item = {
        .name = strdup(name),
        .value = {
            .type = LIST,
            .data.listvalue = value
        }
    };

    return addJSONItem(list, item);
}





JSONValue* getJSONValueByName(JSONItemList** list, char* name){
    JSONItemList** item = getTheoricalNodeByName(list, name);

    if(item == NULL){
        return NULL;
    }

    if((*item) == NULL){
        return NULL;
    }

    return &((*item)->item.value);

}

double getJSONItemByNameAsNumber(JSONItemList** list, char* name){
    JSONValue* item = getJSONValueByName(list, name);

    if(item == NULL || item->type != NUMBER){
        return -1;
    }

    return item->data.numbervalue;
}

char* getJSONItemByNameAsString(JSONItemList** list, char* name){
    JSONValue* item = getJSONValueByName(list, name);

    if(item == NULL || item->type != STRING){
        return 0;
    }

    return item->data.stringvalue;
}

struct JSON* getJSONItemByNameAsObject(JSONItemList** list, char* name){
    JSONValue* item = getJSONValueByName(list, name);

    if(item == NULL || item->type != OBJECT){
        return 0;
    }

    return (item)->data.objectvalue;
}

JSONList* getJSONItemByNameAsList(JSONItemList** list, char* name){
    JSONValue* item = getJSONValueByName(list, name);

    if(item == NULL || item->type != LIST){
        return 0;
    }

    return (item)->data.listvalue;
}

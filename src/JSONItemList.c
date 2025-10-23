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
        (*runner)->item.type = item.type;

    }else{  //if there was an item with same name we overwrite it
        (*runner)->item.name = item.name;
        (*runner)->item.value = item.value;
        (*runner)->item.type = item.type;
    }

    return *runner;
    
    
}

JSONItemList* addJSONStringItem(JSONItemList** list, char* name, char* value){
    JSONItem item;
    item.name = name;
    item.value.stringvalue = value;
    item.type = STRING;

    return addJSONItem(list, item);
}

JSONItemList* addJSONIntItem(JSONItemList** list, char* name, int value){
    JSONItem item;
    item.name = name;
    item.value.intvalue = value;
    item.type = INT;

    return addJSONItem(list, item);
}

JSONItemList* addJSONDoubleItem(JSONItemList** list, char* name, double value){
    JSONItem item;
    item.name = name;
    item.value.doublevalue = value;
    item.type = DOUBLE;

    return addJSONItem(list, item);
}

JSONItemList* addJSONObjectItem(JSONItemList** list, char* name, struct JSON* value){
    JSONItem item;
    item.name = name;
    item.value.objectvalue = value;
    item.type = OBJECT;

    return addJSONItem(list, item);
}





JSONItem* getJSONItemByName(JSONItemList** list, char* name){
    JSONItemList** item = getTheoricalNodeByName(list, name);

    if(item == NULL){
        return NULL;
    }

    if((*item) == NULL){
        return NULL;
    }

    return &((*item)->item);

}

int getJSONItemByNameAsInt(JSONItemList** list, char* name){
    JSONItem* item = getJSONItemByName(list, name);

    if(item == NULL || item->type != INT){
        return -1;
    }

    return (item)->value.intvalue;
}

double getJSONItemByNameAsDouble(JSONItemList** list, char* name){
    JSONItem* item = getJSONItemByName(list, name);

    if(item == NULL || item->type != DOUBLE){
        return -1;
    }

    return (item)->value.doublevalue;
}

char* getJSONItemByNameAsString(JSONItemList** list, char* name){
    JSONItem* item = getJSONItemByName(list, name);

    if(item == NULL || item->type != STRING){
        return 0;
    }

    return (item)->value.stringvalue;
}

struct JSON* getJSONItemByNameAsObject(JSONItemList** list, char* name){
    JSONItem* item = getJSONItemByName(list, name);

    if(item == NULL || item->type != OBJECT){
        return 0;
    }

    return (item)->value.objectvalue;
}
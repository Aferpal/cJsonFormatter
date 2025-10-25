#include<JSON.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

JSON* loadJsonFromFile(char* filename){

    FILE* json_file = fopen(filename, "r");

    if (json_file == NULL) {
        perror("File Not Found!\n");
        return NULL;
    }

    fseek(json_file, 0L, SEEK_END);
    long int total_length = ftell(json_file);

    fseek(json_file, 0, SEEK_SET);
    char* json_file_buffer = (char*)malloc(total_length*sizeof(char)+sizeof(char)*3);


    fread(json_file_buffer, sizeof(char), total_length, json_file);

    fclose(json_file);

    JSON* res = formatJsonFromString(json_file_buffer);

    free(json_file_buffer);
    
    return res;
}

JSONItem createItemFromString(char* name, char* value, enum JSONType type){
    JSONItem item;
    item.name = strdup(name);
    item.type = type;
    
    switch(type){
        case STRING:
            item.value.stringvalue = strdup(value);
            break;
        case INT:
            item.value.intvalue = atoi(value);
        break;
        case DOUBLE:
            item.value.doublevalue = atof(value);
        break;
        case OBJECT:
            item.value.objectvalue = formatJsonFromString(value);
        break;
        default:
    }

    return item;
}

JSON* formatJsonFromString(char* json_as_string){

    if(json_as_string == NULL){
        return NULL;
    }

    JSON* json_object = (JSON*)malloc(sizeof(JSON));
    json_object->items = NULL;
    char* name, *value;
    JSONItem item;
    enum JSONType type = NONE;
    char valueEndCharacter;

    int counter_for_nested_objects = 0;

    //find the start of first param
    while(*json_as_string != '"'){
        json_as_string++;
    }

    while( *json_as_string != 0 && *json_as_string != '}' ){

        type = NONE;

        json_as_string++;

        name = json_as_string;

         //find the end of a param name
         while(*json_as_string != '"'){
            json_as_string++;
        }

        //end of c string with 0 char
        *json_as_string = 0;

        //jump next
        json_as_string++;

        //we skip possible spaces and the colon between name and value
        while(*json_as_string == ' ' || *json_as_string == ':' || *json_as_string == '\n'){
            json_as_string++;
        }

        //we set the start of the value
        value = json_as_string;

        if(*json_as_string == '"'){ //if its a string
             
            value++; //skip the first \"
            json_as_string++;
            valueEndCharacter='"';
            type = STRING;

        }else if(*json_as_string >= '0' && *json_as_string <= '9'){ //if it is numeric

            type = INT; //declared int but changed to double if a dot is found
            valueEndCharacter=',';

        }else if(*json_as_string == '{'){ //if it is a json object

            type = OBJECT;
            counter_for_nested_objects++;
            json_as_string++;
            valueEndCharacter='\0';

        }

        //very complex while condition, if its a number type then it must be either a dot or a number char, else it mustnt be the designed final character
        while(*json_as_string != valueEndCharacter && !((type == INT || type == DOUBLE) && ((*json_as_string < '0' || *json_as_string > '9'))) && !(type == OBJECT && counter_for_nested_objects == 0) || (type == INT && *json_as_string == '.')){
            if(type == INT && *json_as_string == '.'){
                type = DOUBLE;
            }

            if(type == OBJECT && *json_as_string == '{'){
                counter_for_nested_objects++;
            }

            if(type == OBJECT && *json_as_string == '}'){
                counter_for_nested_objects--;
            }
            json_as_string++;
        }

        if(type==OBJECT){
            json_as_string++;
        }

        //we end the value string with a c string delimiter 
        *json_as_string = 0;

        json_as_string++;

        item = createItemFromString(name, value, type);

        addJSONItem(&(json_object->items), item); 


         //check if there is a name or if end of object
         while(*json_as_string != '"' && *json_as_string != '}' && *json_as_string != 0 ){
            json_as_string++;
        }
    }

    return json_object;
}

//setters

void setAsInt(JSON* json, char* name, int value){
    addJSONIntItem(&(json->items), name, value);
}

void setAsDouble(JSON* json, char* name, double value){
    addJSONDoubleItem(&(json->items), name, value);
}

void setAsString(JSON* json, char* name, char* value){
    addJSONStringItem(&(json->items), name, value);
}

void setAsObject(JSON* json, char* name, JSON* value){
    addJSONObjectItem(&(json->items), name, value);
}

//getters

int getAsInt(JSON* json, char* name){
    return getJSONItemByNameAsInt(&(json->items), name);
}

double getAsDouble(JSON* json, char* name){
    return getJSONItemByNameAsDouble(&(json->items), name);
}

char* getAsString(JSON* json, char* name){
    return getJSONItemByNameAsString(&(json->items), name);
}

JSON* getAsObject(JSON* json, char* name){
    return getJSONItemByNameAsObject(&(json->items), name);
}


//other functions

void exportJsonWithDepth(JSON*, int, FILE*);

void exportJsonItem(JSONItem* item, int depth, FILE* file){
    fprintf(file, "\"%s\": ", item->name);
    switch (item->type)
    {
    case STRING:
        fprintf(file, "\"%s\"", item->value.stringvalue);
        break;
    case INT:
        fprintf(file, "%d", item->value.intvalue);
        break;
    case DOUBLE:
        fprintf(file, "%f", item->value.doublevalue);
        break;
    case OBJECT:
        exportJsonWithDepth(item->value.objectvalue, depth+1, file);
        break;
    default:
        break;
    }
}

void exportJsonItemList(JSONItemList* list, int depth, FILE* file){
    if(list == NULL){
        return;
    }

    exportJsonItemList(list->left, depth, file);

    exportJsonItemList(list->right, depth, file);

    for(int i = 0; i <= depth; i++){
        fprintf(file, "\t");
    }
    exportJsonItem(&(list->item), depth, file);
    fprintf(file, ",\n");
}

void exportJsonItemListFirst(JSONItemList* list, int depth, FILE* file){
    if(list == NULL){
        return;
    }

    exportJsonItemList(list->left, depth, file);

    exportJsonItemList(list->right, depth, file);

    for(int i = 0; i <= depth; i++){
        fprintf(file, "\t");
    }
    exportJsonItem(&(list->item), depth, file);
    fprintf(file, "\n");
}

void exportJsonWithDepth(JSON* json, int depth, FILE* file){
    fprintf(file, "{\n");

    exportJsonItemListFirst(json->items, depth, file);

    for( int i = 0; i< depth; i++){
        fprintf(file, "\t");
    }
    fprintf(file, "}");
}

void exportJson(JSON* json, char* filename){
    FILE* file = fopen(filename, "w");

    if( file != NULL ){
        exportJsonWithDepth(json, 0, file);
    }
}

void printJson(JSON* json){
    exportJsonWithDepth(json, 0, stdout);
    fprintf(stdout, "\n");
}


//Destructor

void freeJsonItemList(JSONItemList* list){

    if(list == NULL){
        return;
    }

    freeJsonItemList(list->left);

    if(list->left != NULL){
        free(list->left);
        list->left = NULL;
    }

    freeJsonItemList(list->right);

    if(list->right != NULL){
        free(list->right);
        list->right = NULL;
    }

    free(list->item.name);

    if(list->item.type == STRING){
        free(list->item.value.stringvalue);
    }else if(list->item.type == OBJECT){
        freeJson(list->item.value.objectvalue);
    }
}

void freeJson(JSON* json){
    freeJsonItemList(json->items);
    free(json->items);
    json->items = NULL;
}


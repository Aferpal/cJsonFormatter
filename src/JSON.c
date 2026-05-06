#include<JSON.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

// CREATION AND FORMATTING

const char* read_value(const char*, JSONValue*); // pre declaration

json_t createJsonEmpty(){
    json_t json = (JSON*)malloc(sizeof(JSON));
    json->items = NULL;
    return json;
}

int is_whitespace(const char c){
    return (c == ' ' || c == '\n' || c == '\t' || c == '\r');
}

const char* skip_whitespaces(const char* string){
    if( string == NULL ){
        return NULL;
    }

    while( is_whitespace(*string) ){
        string++;
    }

    return string;
}

const char* read_string(const char* buffer, JSONValue* out_value){

    if( *buffer != '"'){
        printf("La string no comienza por '\"'\n");
        return NULL; // ha ocurrido un error, debería de haber una coma
    }

    buffer++;

    const char* cpy = buffer;

    while( *buffer != 0 && *buffer != '"'){
        buffer++;
    }

    if( *buffer != '"'){
        printf("La string no terminar por '\"'\n");
        return NULL; // ha ocurrido un error, debería de haber una coma
    }

    out_value->data.stringvalue = strndup(cpy, buffer-cpy);
    out_value->type = STRING;

    buffer++;

    buffer = skip_whitespaces(buffer);

    return buffer;

}



const char* read_list(const char* buffer, JSONValue* out_value){

    if( buffer == NULL || *buffer != '['){
        return NULL;
    }

    buffer++;

    buffer = skip_whitespaces(buffer);

    json_list_t list = createJsonList();

    JSONValue j_value;

    int hasNext = 1;

    while( *buffer != 0 && *buffer != ']' && hasNext != 0){

        hasNext = 0;

        buffer = read_value(buffer, &j_value);

        if( buffer == NULL ){
            return NULL;
        }

        if(*buffer == ','){
            hasNext = 1;
            buffer++;
        }

        append(list, j_value);

        buffer = skip_whitespaces(buffer);

    }

    if( *buffer != ']' || hasNext != 0 ){
        return NULL;
    }


    out_value->data.listvalue = list;
    out_value->type = LIST;

    buffer++;

    return buffer;

}

const char* read_number(const char* buffer, JSONValue* out_value){

    char* end;

    double val = strtod(buffer, &end);

    if( val == 0 && end == buffer ){
        return NULL;
    }else{
        out_value->data.numbervalue = val;
        out_value->type = NUMBER;
    }

    return end;
    

}

const char* nextKeyValuePair(const char* string, JSONItem* item){
    
    if( string == NULL ){
        return NULL;
    }

    string = skip_whitespaces(string);

    JSONValue name;

    string = read_string(string, &name);

    if( string == NULL ){
        printf("El error proviene de haber leido incorrectamente el nombre\n");
        return NULL;
    }

    item->name = name.data.stringvalue;

    string = skip_whitespaces(string);

    if( *string != ':'){
        printf("Formato incorrecto intentando encontrar el caracter ':'\n");
        return NULL;
    }

    string++;

    string = read_value(string, &(item->value));
    
    if( string == NULL ){
        printf("Error intentando leer el value con nombre %s\n", item->name);
        return NULL;
    }

    return string;

}

const char* read_object(const char* buffer, JSONValue* out_value){

    if( buffer == NULL || *buffer != '{'){
        return NULL;
    }

    buffer++;

    buffer = skip_whitespaces(buffer);

    json_t json = createJsonEmpty();

    JSONItem j_item;

    int hasNext = 1;

    while( *buffer != 0 && *buffer != '}' && hasNext != 0){

        hasNext = 0;

        buffer = nextKeyValuePair(buffer, &j_item);

        if( buffer == NULL ){
            return NULL;
        }

        if(*buffer == ','){
            hasNext = 1;
            buffer++;
        }

        addJSONItem(&(json->items), j_item);

        buffer = skip_whitespaces(buffer);

    }

    if( *buffer != '}' || hasNext != 0 ){
        return NULL;
    }


    out_value->data.objectvalue = json;
    out_value->type = OBJECT;

    buffer++;

    return buffer;

}

const char* read_value(const char* buffer, JSONValue* out_value){

    if( buffer == NULL ){
        return NULL;
    }

    buffer = skip_whitespaces(buffer);

    switch (*buffer)
    {

    case '[':

        buffer = read_list(buffer, out_value);

        break;

    case '{':
    
        buffer = read_object(buffer, out_value);

        break;

    case '"':
        
        buffer = read_string(buffer, out_value);

        break;

    default:

        buffer = read_number(buffer, out_value);

        break;
    }

    if(buffer == NULL){
        return NULL;
    }

    buffer = skip_whitespaces(buffer);

    return buffer;

}

JSONValue formatJsonFromString(const char* json_as_string){
    JSONValue res;
    const char* end = read_value(json_as_string, &res);

    if( end == NULL || *end != 0){
        res.type = NONE;
    }

    return res;
}




// loaders

JSONValue createJsonFromString(const char* json_string){
    return formatJsonFromString(json_string);
}

JSONValue loadJsonFromFile(char* filename){

    FILE* json_file = fopen(filename, "r");

    if (json_file == NULL) {
        perror("File Not Found!\n");
        JSONValue res = {.type = NONE};
        return res;
    }

    fseek(json_file, 0L, SEEK_END);
    long int total_length = ftell(json_file);

    fseek(json_file, 0, SEEK_SET);
    char* json_file_buffer = (char*)malloc(total_length*sizeof(char)+sizeof(char)*3);


    size_t b = fread(json_file_buffer, sizeof(char), total_length, json_file);

    json_file_buffer[b] = 0;

    fclose(json_file);

    JSONValue res = formatJsonFromString(json_file_buffer);

    free(json_file_buffer);
    
    return res;
}


//setters

void setAsNumber(json_t json, char* name, double value){
    addJSONNumberItem(&(json->items), name, value);
}

void setAsString(json_t json, char* name, char* value){
    addJSONStringItem(&(json->items), name, value);
}

void setAsObject(json_t json, char* name, json_t value){
    addJSONObjectItem(&(json->items), name, value);
}

void setAsList(json_t json, char* name, json_list_t value){
    addJSONListItem(&(json->items), name, value);
}

//getters

double getAsNumber(json_t json, char* name){
    return getJSONItemByNameAsNumber(&(json->items), name);
}

char* getAsString(json_t json, char* name){
    return getJSONItemByNameAsString(&(json->items), name);
}

json_t getAsObject(json_t json, char* name){
    return getJSONItemByNameAsObject(&(json->items), name);
}

json_list_t getAsList(json_t json, char* name){
    return getJSONItemByNameAsList(&(json->items), name);
}


//other functions

void exportJsonWithDepth(json_t, int, FILE*);

void exportJsonValue(JSONValue value, int depth, FILE* file){

    switch (value.type)
    {
    case STRING:
        fprintf(file, "\"%s\"", value.data.stringvalue);
        break;
    case NUMBER:
        fprintf(file, "%g", value.data.numbervalue);
        break;
    case OBJECT:
        exportJsonWithDepth(value.data.objectvalue, depth+1, file);
        break;
    case LIST:
        fprintf(file, "[\n");
        for( int i = 0; i < length(value.data.listvalue); i++){

            if(i != 0){
                fprintf(file, ",\n");
            }

            for(int j = 0; j <= depth+1; j++){
                fprintf(file, "\t");
            }

            exportJsonValue(get(value.data.listvalue, i), depth+1, file);
        }
        fprintf(file, "\n");
        for(int j = 0; j <= depth; j++){
                fprintf(file, "\t");
        }
        fprintf(file, "]");
    default:
        break;
    }
}
void exportJsonItem(JSONItem* item, int depth, FILE* file){
    fprintf(file, "\"%s\": ", item->name);
    exportJsonValue(item->value, depth, file);
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

void exportJsonWithDepth(json_t json, int depth, FILE* file){
    fprintf(file, "{\n");

    exportJsonItemListFirst(json->items, depth, file);

    for( int i = 0; i< depth; i++){
        fprintf(file, "\t");
    }
    fprintf(file, "}");
}

void exportJson(json_t json, const char* filename){
    FILE* file = fopen(filename, "w");

    if( file != NULL ){
        exportJsonWithDepth(json, 0, file);
    }
}

void printJson(json_t json){
    exportJsonWithDepth(json, 0, stdout);
    fprintf(stdout, "\n");
}

char* jsonToString(json_t json){
    char* string = NULL;

    return string;
}


//Destructor

void freeJsonList(json_list_t list);

void freeJsonValue(JSONValue value){
    switch (value.type)
    {
    case STRING:

        if( value.data.stringvalue != NULL ){
            free( value.data.stringvalue );
        }

        break;

    case OBJECT:
        freeJson(value.data.objectvalue);
        break;
    case LIST:
        freeJsonList(value.data.listvalue);
        break;
    default:
        break;
    }
}

void freeJsonList(json_list_t list){

    if( list == NULL ){
        return;
    }

    for( int i = 0; i < length(list); i++){
        freeJsonValue( get(list, i) );
    }

    //despues de liberar todo tenemos que liberar data
    free(list->_data);

    list->_data = NULL;
    list->_reserved = 0;
    list->_size = 0;

    free(list);

}

void freeJsonItem(JSONItem* item){
    if( item == NULL ){
        return;
    }

    if( item -> name != NULL){
        free(item->name);
    }

    freeJsonValue((item->value));
}


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

    freeJsonItem(&(list->item));
}

void freeJson(json_t json){
    freeJsonItemList(json->items);
    free(json->items);
    json->items = NULL;
    free(json);
}


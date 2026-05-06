#include<JSON.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

// CREATION AND FORMATTING

json_t formatJsonFromString(const char*); // pre declaration
int createValueFromString(char* value, JSONValue*);


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

const char* read_string(const char* buffer, char** string){
    if( *buffer != '"'){
        printf("La string no comienza por '\"'\n");
        *string = NULL;
        return NULL; // ha ocurrido un error, debería de haber una coma
    }

    buffer++;

    const char* cpy = buffer;

    while( *buffer != 0 && *buffer != '"'){
        buffer++;
    }

    if( *buffer != '"'){
        printf("La string no terminar por '\"'\n");
        *string = NULL;
        return NULL; // ha ocurrido un error, debería de haber una coma
    }

    *string = strndup(cpy, (buffer-cpy));

    buffer++;

    buffer = skip_whitespaces(buffer);

    return buffer;

}

const char* read_object(const char* buffer, char** string){

    if( buffer == NULL || *buffer != '{'){
        return NULL;
    }

    const char* cpy = buffer;

    buffer++;

    int nested = 0;

    while( *buffer != 0 && (*buffer != '}' || nested != 0) ){

        if( *buffer == '{' ){
            nested++;
        }

        if( *buffer == '}'){
            nested--;
        }

        buffer++;
    }

    if( *buffer != '}' || nested != 0){
        return NULL; // ha ocurrido un error, el formato es incorrecto
    }

    buffer++;

    *string = strndup(cpy, (buffer-cpy));

    return buffer;

}

const char* read_list(const char* buffer, char** string){

    if( buffer == NULL || *buffer != '['){
        return NULL;
    }

    const char* cpy = buffer;

    buffer++;

    int nested = 0;

    while( *buffer != 0 && (*buffer != ']' || nested != 0) ){
        if( *buffer == '[' ){
            nested++;
        }

        if( *buffer == ']'){
            nested--;
        }

        buffer++;
    }

    if( *buffer != ']' || nested != 0){
        return NULL; // ha ocurrido un error, el formato es incorrecto
    }

    buffer++;

    *string = strndup(cpy, (buffer-cpy));

    return buffer;

}

const char* read_number(const char* buffer, char** string){

    if( buffer == NULL || *buffer < '0' || *buffer > '9'){
        return NULL;
    }

    const char* cpy = buffer;

    int dot_found = 0;

    while( ( *buffer >= '0' && *buffer <= '9' ) || ( *buffer == '.' && dot_found == 0 )){
        if( *buffer == '.' ){
            dot_found = 1;
        }
        buffer++;
    }

    *string = strndup(cpy, (buffer-cpy));

    return buffer;
    

}

const char* read_value(const char* buffer, JSONValue* out_value){

    if( buffer == NULL ){
        return NULL;
    }

    buffer = skip_whitespaces(buffer);

    char* value;

    switch (*buffer)
    {

    case '[':

        buffer = read_list(buffer, &value);

        if( buffer == NULL ){
            printf("Error leyendo lista\n");
            return NULL;
        }

        out_value->type = LIST;

        break;

    case '{':
    
        buffer = read_object(buffer, &value);

        if( buffer == NULL ){
            printf("Error leyendo objeto\n");
            return NULL;
        }

        out_value->type = OBJECT;

        break;

    case '"':
        
        buffer = read_string(buffer, &value);

        if( buffer == NULL ){
            return NULL;
        }

        out_value->type = STRING;

        break;

    default:
        // aqui ya tema numeros
        if( *buffer < '0' || *buffer > '9'){
            printf("tipo no reconocido\n");
            return NULL;
        }

        buffer = read_number(buffer, &value);

        if(buffer == NULL){
            return NULL;
        }

        out_value->type = NUMBER;

        break;
    }

    buffer = skip_whitespaces(buffer);

    createValueFromString(value, out_value);

    free(value);

    return buffer;

}

const char* nextKeyValuePair(const char* string, JSONItem* item){
    
    if( string == NULL ){
        return NULL;
    }

    string = skip_whitespaces(string);

    char* name;

    string = read_string(string, &name);

    if( string == NULL ){
        printf("El error proviene de haber leido incorrectamente el nombre\n");
        return NULL;
    }

    item->name = name;

    string = skip_whitespaces(string);

    if( *string != ':'){
        printf("Formato incorrecto intentando encontrar el caracter ':'\n");
        return NULL;
    }

    string++;

    string = read_value(string, &(item->value));
    
    if( string == NULL ){
        printf("Error intentando leer el value con nombre %s\n", name);
        return NULL;
    }

    return string;

}

json_list_t formatJsonListFromString(const char* value){
    
    if( value == NULL || *value != '['){
        return NULL;
    }

    value++;

    value = skip_whitespaces(value);

    json_list_t list = createJsonList();

    JSONValue j_value;

    int hasNext = 1;

    while( *value != 0 && *value != ']' && hasNext != 0){

        hasNext = 0;

        value = read_value(value, &j_value);

        if( value == NULL ){
            return NULL;
        }

        if(*value == ','){
            hasNext = 1;
            value++;
        }

        append(list, j_value);

        if( hasNext != 0 ){
            value = skip_whitespaces(value);
        }

    }

    if( *value != ']' || hasNext != 0 ){
        return NULL;
    }

    return list;
    

}

int createValueFromString(char* str_value, JSONValue* out_value){
    
    switch(out_value->type){
        case STRING:
            out_value->data.stringvalue = strdup(str_value);
            break;
        case NUMBER:
            out_value->data.numbervalue = atof(str_value);
        break;
        case OBJECT:
            out_value->data.objectvalue = formatJsonFromString(str_value);
        break;
        case LIST:
            out_value->data.listvalue = formatJsonListFromString(str_value);
        default:
        break;
    }

    return 1;
}

json_t formatJsonFromString(const char* json_as_string){

    if(json_as_string == NULL){
        return NULL;
    }

    JSON* json_object = createJsonEmpty();
    JSONItem item;
    int hasNext = 1;

    json_as_string = skip_whitespaces(json_as_string);

    if(*json_as_string != '{'){
        printf("El json no comienza correctamente\n");
        return NULL;
    }

    json_as_string++;

    json_as_string = skip_whitespaces(json_as_string);

    while( *json_as_string != 0 && *json_as_string != '}' && hasNext != 0 ){

        hasNext = 0;

        json_as_string = nextKeyValuePair(json_as_string, &item);

        if( json_as_string == NULL ){ // si ha habido cualquier problema liberamos

            freeJson(json_object);

            printf("Ha ocurrido un error intentando leer un item\n");

            return NULL;

        }

        if(*json_as_string == ','){
            hasNext = 1;
            json_as_string++;
        }

        addJSONItem(&(json_object->items), item); 

        if( hasNext != 0 ){
            json_as_string = skip_whitespaces(json_as_string);
        }

    }

    if( *json_as_string == 0 || hasNext == 1 ){ // algo ha salido mal, deberiamos acabar siempre en }

        freeJson(json_object);

        printf("El json no termina correctamente \n");

        return NULL;
    }

    return json_object;
}




// loaders

json_t createJsonFromString(const char* json_string){
    return formatJsonFromString(json_string);
}

json_t loadJsonFromFile(char* filename){

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

    json_t res = formatJsonFromString(json_file_buffer);

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
            printf("Freeing value: %s\n", value.data.stringvalue);
            free( value.data.stringvalue );
        }

        break;

    case OBJECT:
        printf("Freeing nested json: \n");
        freeJson(value.data.objectvalue);
        break;
    case LIST:
        printf("Freeing nested list: \n");
        freeJsonList(value.data.listvalue);
        break;
    default:
        printf("Freeing value: it is a number or smth \n");
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
    printf("Justo justo antes de liberar datos lista %p\n", list->_data);
    free(list->_data);
    printf("Justo justo despues de liberar datos lista\n");

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
        printf("Freeing name %s\n", item->name);
        free(item->name);
        printf("Name fred\n");
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


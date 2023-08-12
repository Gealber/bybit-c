#ifndef COMMON_H
#define COMMON_H

#include <curl/curl.h>
#include "cJSON.h"


typedef enum
{
    FALSE = 0,
    TRUE
} bool;

const char *bool_to_string(bool val);

// response will store in memory the response from the API
typedef struct
{
    char *chunk;
    size_t size;
} ResponseJSON;

typedef struct Node
{
    void *val;
    struct Node *next;
} Node;

void add_query(CURL *hnd_url, char *key, char *val);
size_t write_json(char *contents, size_t size, size_t nmemb, void *userdata);
char *extract_string_field(const cJSON *json, char *field_name);
int extract_int_field(const cJSON *json, char *field_name);
bool extract_bool_field(const cJSON *json, char *field_name);
void add_string_field(cJSON *const json, const char *field_name, char *field);
void add_int_field(cJSON *const json, const char *field_name, int field);
void add_bool_field(cJSON *const json, const char *field_name, bool field);
void add_list_item(Node **list, void *val);

// generate signature
int64_t timestamp();
unsigned long long utc_timestamp();

// clean list
void free_list(Node **list, void free_val_callback(void *));
void clean_string(char* str);

#endif
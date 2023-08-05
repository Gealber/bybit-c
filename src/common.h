#ifndef COMMON_H
#define COMMON_H

#include <curl/curl.h>
#include "cJSON.h"

// response will store in memory the response from the API
typedef struct {
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
void add_list_item(Node **list, void *val);
void free_list(Node **list, void free_val_callback(void *));

#endif
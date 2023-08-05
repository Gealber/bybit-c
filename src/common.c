#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "common.h"


void add_list_item(Node **list, void *val)
{
    Node *new_node = malloc(sizeof(Node));
    new_node->val = val;
    new_node->next = (*list);
    (*list) = new_node;
}

void free_list(Node **list, void free_val_callback(void *))
{
    Node *cur = *list;
    Node *next ;
    while (cur != NULL)
    {
        next = cur->next;
        free_val_callback(cur->val);
        free(cur);
        cur = next;
    }

    *list = NULL;
}

// add query will set the query only if the key or value provided are not empty
void add_query(CURL *hnd_url, char *key, char *val)
{
    if (key == NULL || (strlen(key) == 0))
        return;

    if (val == NULL || (strlen(val) == 0))
        return;

    size_t query_size = strlen(key) + strlen(val) + 1;
    char query[query_size];
    sprintf(query, "%s=%s", key, val);
    curl_url_set(hnd_url, CURLUPART_QUERY, query, CURLU_APPENDQUERY | CURLU_URLENCODE);
}

// write memory write response to memory
size_t write_json(char *contents, size_t size, size_t nmemb, void *userdata)
{
    size_t real_size = size * nmemb;
    ResponseJSON *mem = (ResponseJSON *)userdata;

    char *ptr = realloc(mem->chunk, mem->size + real_size + 1);
    if (!ptr) {
        printf("err: not enough memory realloc returned NULL\n");

        return 0;
    }

    mem->chunk = ptr;
    memcpy(&(mem->chunk[mem->size]), contents, real_size);
    mem->size += real_size;
    mem->chunk[mem->size] = 0; // null termination character

    return real_size;
}

char *extract_string_field(const cJSON *json, char *field_name)
{
    cJSON *item = cJSON_GetObjectItemCaseSensitive(json, field_name);
    if(cJSON_IsString(item) && (item->valuestring != NULL)) {
        return item->valuestring;
    }

    return "";
}
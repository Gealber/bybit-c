#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
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
    Node *next;
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
    if (!ptr)
    {
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
    if (cJSON_IsString(item) && (item->valuestring != NULL))
    {
        return item->valuestring;
    }

    return "";
}

int extract_int_field(const cJSON *json, char *field_name)
{
    cJSON *item = cJSON_GetObjectItemCaseSensitive(json, field_name);
    return item->valueint;
}

bool extract_bool_field(const cJSON *json, char *field_name)
{
    cJSON *item = cJSON_GetObjectItemCaseSensitive(json, field_name);
    return cJSON_IsTrue(item) ? TRUE : FALSE;
}

void add_string_field(cJSON *const json, const char *field_name, char *field)
{
    if (field)
    {
        cJSON_AddStringToObject(json, field_name, field);
    }
}

void add_int_field(cJSON *const json, const char *field_name, int field)
{
    cJSON_AddNumberToObject(json, field_name, field);
}

void add_bool_field(cJSON *const json, const char *field_name, bool field)
{
    cJSON_AddStringToObject(json, field_name, bool_to_string(field));
}

const char *bool_to_string(bool val)
{
    if (val)
        return "true";

    return "false";
}

int64_t timestamp()
{
    return time(NULL) * 1000;
}

unsigned long long utc_timestamp()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);

    unsigned long long milliseconds = (unsigned long long)(tv.tv_sec) * 1000 + (unsigned long long)(tv.tv_usec) / 1000;

    return milliseconds;
}

void clean_string(char *str)
{
    int i, j = 0;
    for (i = 0; str[i] != '\0'; i++)
    {
        if (str[i] != ' ' && str[i] != '\t' && str[i] != '\n' && str[i] != '\r')
        {
            str[j] = str[i];
            j++;
        }
    }
    str[j] = '\0';
}
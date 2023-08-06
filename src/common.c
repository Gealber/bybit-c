#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <curl/curl.h>
#include <openssl/evp.h>
#include <openssl/hmac.h>
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

char *gen_signature(const char *api_key, const char *api_secret, int64_t ts, char *params)
{
    int recv_window = 5000;
    // assuming a max of 13 digits for timestamp
    // 4 for recv windows
    size_t rule_size = 13 + strlen(api_key) + 4 + strlen(params) + 1;
    char rule[rule_size];
    sprintf(rule, "%ld%s%d%s", ts, api_key, recv_window, params);

    int keylen = strlen(api_secret);
    const unsigned char *data = (const unsigned char *)rule;
    int datalen = strlen((char *)data);
    unsigned char *result = NULL;
    unsigned int resultlen = -1;

    result = hmac_sha256((const void *)api_secret, keylen, data, datalen, result, &resultlen);

    char *hex = calloc(resultlen * 2 + 1, sizeof(char));
    char *ptr = &hex[0];

    int i = 0;
    for (i = 0; i < resultlen; i++)
    {
        ptr += sprintf(ptr, "%02X", result[i]);
    }

    return hex;
}

unsigned char *hmac_sha256(const void *key, int keylen, const unsigned char *data, int datalen, unsigned char *result, unsigned int *resultlen)
{
    return HMAC(EVP_sha256(), key, keylen, data, datalen, result, resultlen);
}

int64_t timestamp()
{
    return time(NULL) * 1000;
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
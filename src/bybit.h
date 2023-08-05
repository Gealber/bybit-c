#ifndef BYBIT_H
#define BYBIT_H

#include <time.h>
#include <curl/curl.h>
#include "cJSON.h"
#include "request.h"
#include "response.h"

typedef struct
{
    char *api_key;
    char *api_secret;
} Client;

typedef struct
{
    int ret_code;
    char *ret_msg;
    void *result;
    void *ret_ext_info;
    time_t time;
} Response;

typedef struct
{
    char *locale;
    char *type;
    char *tag;
    int page;
    int limit;
} AnnouncementQueryParams;

typedef struct
{
    char *title;
    char *description;
    struct AnnouncementType *type;
    char **tags;
    char *url;
    time_t date_timestamp;
    time_t start_date_timestamp;
    time_t end_date_timestamp;
} Announcement;

typedef struct
{
    char *title;
    char *key;
} AnnouncementType;

Client *new(char *api_key, char *api_secret);
CURL *http_client();
APIResponse *get_ticker(TickersQueryParams *query);
APIResponse *get_time_server();
APIResponse *get_kline(KlineQueryParams *query);
APIResponse *get_mark_price_kline(KlineQueryParams *query);
APIResponse *get_order_book(OrderBookQuery *query);

#endif
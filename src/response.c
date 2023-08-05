/*
This file contains the parsing of responses
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "request.h"
#include "response.h"
#include "common.h"
#include "cJSON.h"

TickerResponse *parse_ticker_response(char *text)
{
    TickerResponse *resp =  malloc(sizeof(TickerResponse));
    if (!resp)
        return NULL;
    resp->list = NULL;

    const cJSON *result = NULL;
    const cJSON *list = NULL;
    const cJSON *list_item = NULL;

    if (strlen(text) == 0)
        return NULL;
    cJSON *json = cJSON_Parse(text);
    if(!json) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            fprintf(stderr, "Error before: %s\n", error_ptr);
            fprintf(stderr, "Error text: %s\n", text);
        }
    }

    // cursor of the list
    result = cJSON_GetObjectItemCaseSensitive(json, "result");
    if(cJSON_IsObject(result))
    {
        char *category_str = extract_string_field(result, "category");
        if (strlen(category_str) != 0)
            resp->category = strdup(category_str);

        list = cJSON_GetObjectItemCaseSensitive(result, "list");
        cJSON_ArrayForEach(list_item, list)
        {
            Ticker *ticker = build_ticker(list_item);
            add_list_item(&resp->list, ticker);
        }
    }

    cJSON_Delete(json);

    return resp;
}

TimeServerResponse *parse_ts_response(char *text)
{
    TimeServerResponse *resp =  malloc(sizeof(TimeServerResponse));
    if (!resp)
        return NULL;

    const cJSON *result = NULL;

    if (strlen(text) == 0)
        return NULL;
    cJSON *json = cJSON_Parse(text);
    if(!json) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            fprintf(stderr, "Error before: %s\n", error_ptr);
            fprintf(stderr, "Error text: %s\n", text);
        }
    }

    result = cJSON_GetObjectItemCaseSensitive(json, "result");
    if(cJSON_IsObject(result))
    {
        char *time_second = extract_string_field(result, "timeSecond");
        if (strlen(time_second) != 0)
            resp->time_second = strdup(time_second);
        char *time_nano = extract_string_field(result, "timeNano");
        if (strlen(time_nano) != 0)
            resp->time_nano = strdup(time_nano);

    }

    cJSON_Delete(json);

    return resp;
}

KlineResponse *parse_kline_response(char *text)
{
    KlineResponse *resp =  malloc(sizeof(KlineResponse));
    if (!resp)
        return NULL;
    resp->list = NULL;

    const cJSON *result = NULL;
    const cJSON *list = NULL;
    const cJSON *list_item = NULL;

    if (strlen(text) == 0)
        return NULL;
    cJSON *json = cJSON_Parse(text);
    if(!json) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            fprintf(stderr, "Error before: %s\n", error_ptr);
            fprintf(stderr, "Error text: %s\n", text);
        }
    }

    // cursor of the list
    result = cJSON_GetObjectItemCaseSensitive(json, "result");
    if(cJSON_IsObject(result))
    {
        char *symbol_str = extract_string_field(result, "symbol");
        if (strlen(symbol_str) != 0)
            resp->symbol = strdup(symbol_str);
        char *category_str = extract_string_field(result, "category");
        if (strlen(category_str) != 0)
            resp->category = strdup(category_str);

        list = cJSON_GetObjectItemCaseSensitive(result, "list");
        cJSON_ArrayForEach(list_item, list)
        {
            Kline *kline = build_kline(list_item);
            add_list_item(&resp->list, kline);
        }
    }

    cJSON_Delete(json);

    return resp;
}

KlineResponse *parse_price_kline_response(char *text)
{
    KlineResponse *resp =  malloc(sizeof(KlineResponse));
    if (!resp)
        return NULL;
    resp->list = NULL;

    const cJSON *result = NULL;
    const cJSON *list = NULL;
    const cJSON *list_item = NULL;

    if (strlen(text) == 0)
        return NULL;
    cJSON *json = cJSON_Parse(text);
    if(!json) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            fprintf(stderr, "Error before: %s\n", error_ptr);
            fprintf(stderr, "Error text: %s\n", text);
        }
    }

    // cursor of the list
    result = cJSON_GetObjectItemCaseSensitive(json, "result");
    if(cJSON_IsObject(result))
    {
        char *symbol_str = extract_string_field(result, "symbol");
        if (strlen(symbol_str) != 0)
            resp->symbol = strdup(symbol_str);
        char *category_str = extract_string_field(result, "category");
        if (strlen(category_str) != 0)
            resp->category = strdup(category_str);

        list = cJSON_GetObjectItemCaseSensitive(result, "list");
        cJSON_ArrayForEach(list_item, list)
        {
            Kline *kline = build_price_kline(list_item);
            add_list_item(&resp->list, kline);
        }
    }

    cJSON_Delete(json);

    return resp;
}

OrderBookResponse *parse_order_book_response(char *text)
{
    OrderBookResponse *resp =  malloc(sizeof(OrderBookResponse));
    if (!resp)
        return NULL;
    resp->bids = NULL;
    resp->asks = NULL;

    const cJSON *result = NULL;
    const cJSON *a = NULL;
    const cJSON *a_item = NULL;
    const cJSON *b = NULL;
    const cJSON *b_item = NULL;

    if (strlen(text) == 0)
        return NULL;
    cJSON *json = cJSON_Parse(text);
    if(!json) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            fprintf(stderr, "Error before: %s\n", error_ptr);
            fprintf(stderr, "Error text: %s\n", text);
        }
    }

    // cursor of the list
    result = cJSON_GetObjectItemCaseSensitive(json, "result");
    if(cJSON_IsObject(result))
    {
        char *symbol_str = extract_string_field(result, "s");
        if (strlen(symbol_str) != 0)
            resp->symbol = strdup(symbol_str);
        char *ts_str = extract_string_field(result, "ts");
        if (strlen(ts_str) != 0)
            resp->ts = strdup(ts_str);
        char *u_str = extract_string_field(result, "u");
        if (strlen(u_str) != 0)
            resp->update_id = strdup(u_str);

        a = cJSON_GetObjectItemCaseSensitive(result, "a");
        cJSON_ArrayForEach(a_item, a)
        {
            OrderB *order = build_orderb(a_item);
            add_list_item(&resp->asks, order);
        }

        b = cJSON_GetObjectItemCaseSensitive(result, "b");
        cJSON_ArrayForEach(b_item, b)
        {
            OrderB *order = build_orderb(b_item);
            add_list_item(&resp->bids, order);
        }
    }

    cJSON_Delete(json);

    return resp;
}

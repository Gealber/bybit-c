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

APIResponse *parse_api_response(char *text, void *result_parsing_cb(const cJSON *))
{
    APIResponse *resp = malloc(sizeof(APIResponse));
    if (!resp)
        return NULL;
    resp->result = NULL;
    resp->ret_code = 0;
    resp->time = 0;
    resp->ret_msg = 0;

    const cJSON *ret_code = NULL;

    if (!text || strlen(text) == 0) {
        free(resp);
        return NULL;
    }
    cJSON *json = cJSON_Parse(text);
    if (!json)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            fprintf(stderr, "Error before: %s\n", error_ptr);
            fprintf(stderr, "Error text: %s\n", text);
        }
    }

    ret_code = cJSON_GetObjectItemCaseSensitive(json, "retCode");
    if (cJSON_IsNumber(ret_code))
    {
        resp->ret_code = ret_code->valueint;
    }

    char *ret_msg_str = extract_string_field(json, "retMsg");
    resp->ret_msg = strdup(ret_msg_str);

    resp->result = result_parsing_cb(json);

    cJSON_Delete(json);

    return resp;
}

void *parse_ticker_response_cb(const cJSON *json)
{
    return parse_ticker_response(json);
}

TickerResponse *parse_ticker_response(const cJSON *json)
{
    TickerResponse *resp = malloc(sizeof(TickerResponse));
    if (!resp)
        return NULL;

    resp->list = NULL;
    resp->category = 0;

    const cJSON *result = NULL;
    const cJSON *list = NULL;
    const cJSON *list_item = NULL;

    // cursor of the list
    result = cJSON_GetObjectItemCaseSensitive(json, "result");
    if (cJSON_IsObject(result))
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

    return resp;
}

void *parse_ts_response_cb(const cJSON *json)
{
    return parse_ts_response(json);
}

TimeServerResponse *parse_ts_response(const cJSON *json)
{
    TimeServerResponse *resp = malloc(sizeof(TimeServerResponse));
    if (!resp)
        return NULL;

    resp->time_nano = 0;
    resp->time_second = 0;

    const cJSON *result = NULL;

    result = cJSON_GetObjectItemCaseSensitive(json, "result");
    if (cJSON_IsObject(result))
    {
        char *time_second = extract_string_field(result, "timeSecond");
        if (strlen(time_second) != 0)
            resp->time_second = strdup(time_second);
        char *time_nano = extract_string_field(result, "timeNano");
        if (strlen(time_nano) != 0)
            resp->time_nano = strdup(time_nano);
    }

    return resp;
}

void *parse_kline_response_cb(const cJSON *json)
{
    return parse_kline_response(json);
}

KlineResponse *parse_kline_response(const cJSON *json)
{
    KlineResponse *resp = malloc(sizeof(KlineResponse));
    if (!resp)
        return NULL;
    resp->list = NULL;
    resp->category = 0;
    resp->symbol = 0;

    const cJSON *result = NULL;
    const cJSON *list = NULL;
    const cJSON *list_item = NULL;

    // cursor of the list
    result = cJSON_GetObjectItemCaseSensitive(json, "result");
    if (cJSON_IsObject(result))
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

    return resp;
}

void *parse_price_kline_response_cb(const cJSON *json)
{
    return parse_price_kline_response(json);
}

KlineResponse *parse_price_kline_response(const cJSON *json)
{
    KlineResponse *resp = malloc(sizeof(KlineResponse));
    if (!resp)
        return NULL;
    resp->list = NULL;
    resp->category = 0;
    resp->symbol = 0;

    const cJSON *result = NULL;
    const cJSON *list = NULL;
    const cJSON *list_item = NULL;

    // cursor of the list
    result = cJSON_GetObjectItemCaseSensitive(json, "result");
    if (cJSON_IsObject(result))
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

    return resp;
}

void *parse_order_book_response_cb(const cJSON *json)
{
    return parse_order_book_response(json);
}

OrderBookResponse *parse_order_book_response(const cJSON *json)
{
    OrderBookResponse *resp = malloc(sizeof(OrderBookResponse));
    if (!resp)
        return NULL;
    resp->bids = NULL;
    resp->asks = NULL;
    resp->symbol = 0;
    resp->ts = 0;
    resp->update_id = 0;

    const cJSON *result = NULL;
    const cJSON *a = NULL;
    const cJSON *a_item = NULL;
    const cJSON *b = NULL;
    const cJSON *b_item = NULL;

    // cursor of the list
    result = cJSON_GetObjectItemCaseSensitive(json, "result");
    if (cJSON_IsObject(result))
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

    return resp;
}

void *parse_order_response_cb(const cJSON *json)
{
    return parse_order_response(json);
}

OrderResponse *parse_order_response(const cJSON *json)
{
    OrderResponse *resp = malloc(sizeof(OrderResponse));
    if (!resp)
        return NULL;
    resp->order_id = NULL;
    resp->order_link_id = NULL;

    const cJSON *result = NULL;

    // cursor of the list
    result = cJSON_GetObjectItemCaseSensitive(json, "result");
    if (cJSON_IsObject(result))
    {
        char *order_id_str = extract_string_field(result, "orderId");
        if (strlen(order_id_str) != 0)
            resp->order_id = strdup(order_id_str);
        char *order_link_id_str = extract_string_field(result, "orderLinkId");
        if (strlen(order_link_id_str) != 0)
            resp->order_link_id = strdup(order_link_id_str);
    }

    return resp;
}

void *parse_cancel_all_orders_response_cb(const cJSON *json)
{
    return parse_cancel_all_orders_response(json);
}

CancelAllOrdersResponse *parse_cancel_all_orders_response(const cJSON *json)
{
    CancelAllOrdersResponse *resp = malloc(sizeof(CancelAllOrdersResponse));
    if (!resp)
        return NULL;
    resp->list = NULL;

    const cJSON *result = NULL;
    const cJSON *list = NULL;
    const cJSON *list_item = NULL;

    // cursor of the list
    result = cJSON_GetObjectItemCaseSensitive(json, "result");
    if (cJSON_IsObject(result))
    {
        list = cJSON_GetObjectItemCaseSensitive(result, "list");
        cJSON_ArrayForEach(list_item, list)
        {
            CancelledOrder *cancelled_order = build_cancelled_order(list_item);
            add_list_item(&resp->list, cancelled_order);
        }
    }

    return resp;
}

void *parse_open_orders_response_cb(const cJSON *json)
{
    return parse_open_orders_response(json);
}

OpenOrdersResponse *parse_open_orders_response(const cJSON *json)
{
    OpenOrdersResponse *resp = malloc(sizeof(OpenOrdersResponse));
    if (!resp)
        return NULL;
    // initialise response
    resp->category = NULL;
    resp->next_page_cursor = NULL;
    resp->list = NULL;

    const cJSON *result = NULL;
    const cJSON *list = NULL;
    const cJSON *list_item = NULL;

    // cursor of the list
    result = cJSON_GetObjectItemCaseSensitive(json, "result");
    if (cJSON_IsObject(result))
    {
        char *category_str = extract_string_field(result, "category");
        if (strlen(category_str) != 0)
            resp->category = strdup(category_str);
        char *next_page_cursor = extract_string_field(result, "nextPageCursor");
        if (strlen(next_page_cursor) != 0)
            resp->next_page_cursor = strdup(next_page_cursor);

        list = cJSON_GetObjectItemCaseSensitive(result, "list");
        cJSON_ArrayForEach(list_item, list)
        {
            OpenOrder *open_order = build_open_order_arr(list_item);
            add_list_item(&resp->list, open_order);
        }
    }

    return resp;
}

void *parse_order_history_response_cb(const cJSON *json)
{
    return parse_order_history_response(json);
}

OrderHistoryResponse *parse_order_history_response(const cJSON *json)
{
    OrderHistoryResponse *resp = malloc(sizeof(OrderHistoryResponse));
    if (!resp)
        return NULL;
    // initialise response
    resp->category = NULL;
    resp->list = NULL;

    const cJSON *result = NULL;
    const cJSON *list = NULL;
    const cJSON *list_item = NULL;

    // cursor of the list
    result = cJSON_GetObjectItemCaseSensitive(json, "result");
    if (cJSON_IsObject(result))
    {
        char *category_str = extract_string_field(result, "category");
        if (strlen(category_str) != 0)
            resp->category = strdup(category_str);

        list = cJSON_GetObjectItemCaseSensitive(result, "list");
        cJSON_ArrayForEach(list_item, list)
        {
            OpenOrder *open_order = build_open_order_obj(list_item);
            add_list_item(&resp->list, open_order);
        }
    }

    return resp;
}

void free_api_response(APIResponse *api_resp)
{
    if (api_resp->ret_msg)
        free(api_resp->ret_msg);
    if (api_resp->result)
        free(api_resp->result);
    if (api_resp)
        free(api_resp);
    api_resp = NULL;
}
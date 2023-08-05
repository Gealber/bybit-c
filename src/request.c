/*
This file contains the building of requests and free of structs used in requests
*/

#include <string.h>
#include <stdlib.h>
#include "common.h"
#include "request.h"

// build ticker from response
Ticker *build_ticker(const cJSON *list_item)
{
    Ticker *ticker = malloc(sizeof(Ticker));
    if (!ticker)
        return NULL;

    // symbol
    char *symbol_str = extract_string_field(list_item, "symbol");
    ticker->symbol = strdup(symbol_str);
    // bid1Price
    char *bid1_price_str = extract_string_field(list_item, "bid1Price");
    ticker->bid1_price = strdup(bid1_price_str);
    // bid1Size
    char *bid1_size_str = extract_string_field(list_item, "bid1Size");
    ticker->bid1_size = strdup(bid1_size_str);
    // ask1Price
    char *ask1_price_str = extract_string_field(list_item, "ask1Price");
    ticker->ask1_price = strdup(ask1_price_str);
    // ask1Size
    char *ask1_size_str = extract_string_field(list_item, "ask1Size");
    ticker->ask1_size = strdup(ask1_size_str);
    // lastPrice
    char *last_price_str = extract_string_field(list_item, "lastPrice");
    ticker->last_price = strdup(last_price_str);
    // prevPrice24h
    char *prev_price_str = extract_string_field(list_item, "prevPrice24h");
    ticker->prev_price_24h = strdup(prev_price_str);
    // price24hPcnt
    char *price_24h_str = extract_string_field(list_item, "price24hPcnt");
    ticker->price_24h_pcnt = strdup(price_24h_str);
    // highPrice24h
    char *high_price24h_str = extract_string_field(list_item, "highPrice24h");
    ticker->high_price_24h = strdup(high_price24h_str);
    // lowPrice24h
    char *low_price24h_str = extract_string_field(list_item, "lowPrice24h");
    ticker->low_price_24h = strdup(low_price24h_str);
    // turnover24h
    char *turnover_24h_str = extract_string_field(list_item, "turnover24h");
    ticker->turn_over_24h = strdup(turnover_24h_str);
    // volume24h
    char *volume_24h_str = extract_string_field(list_item, "volume24h");
    ticker->volume_24 = strdup(volume_24h_str);
    // usdIndexPrice
    char *usd_index_str = extract_string_field(list_item, "usdIndexPrice");
    ticker->usd_index_price = strdup(usd_index_str);

    return ticker;
}

TickersQueryParams *build_ticker_query(char *category, char *symbol, char *base_coin, char *exp_date)
{
    TickersQueryParams *params = malloc(sizeof(TickersQueryParams));
    if (!params)
        return NULL;
    
    params->category = category;
    params->symbol = symbol;
    params->base_coin = base_coin;
    params->exp_date = exp_date;

    params->_queries = (Node *)calloc(1, sizeof(Node));

    // build _queries list

    if (category != NULL && strlen(category) != 0) {
        _queryElement *category_query = create_query_element("category", category);
        add_list_item(&params->_queries, category_query);
    }
    if (symbol != NULL && strlen(symbol) != 0) {
        _queryElement *symbol_query = create_query_element("symbol", symbol);
        add_list_item(&params->_queries, symbol_query);
    }
    if (base_coin != NULL && strlen(base_coin) != 0) {
        _queryElement *base_coin_query = create_query_element("baseCoin", base_coin);
        add_list_item(&params->_queries, base_coin_query);
    }
    if (exp_date != NULL && strlen(exp_date) != 0) {
        _queryElement *exp_date_query = create_query_element("expDate", exp_date);
        add_list_item(&params->_queries, exp_date_query);
    }

    return params;
}

KlineQueryParams *build_kline_query(char *category, char *symbol, char *interval, char *start, char *end, char *limit)
{
    KlineQueryParams *params = malloc(sizeof(KlineQueryParams));
    if (!params)
        return NULL;
    
    params->category = category;
    params->symbol = symbol;
    params->interval = interval;
    params->start = start;
    params->end = end;
    params->limit = limit;

    params->_queries = (Node *)calloc(1, sizeof(Node));

    // build _queries list
    if (category != NULL && strlen(category) != 0) {
        _queryElement *category_query = create_query_element("category", category);
        add_list_item(&params->_queries, category_query);
    }
    if (symbol != NULL && strlen(symbol) != 0) {
        _queryElement *symbol_query = create_query_element("symbol", symbol);
        add_list_item(&params->_queries, symbol_query);
    }
    if (interval && strlen(interval) != 0) {
        _queryElement *interval_query = create_query_element("interval", interval);
        add_list_item(&params->_queries, interval_query);
    }
    if (start && strlen(start) != 0) {
        _queryElement *start_query = create_query_element("start", start);
        add_list_item(&params->_queries, start_query);
    }
    if (end && strlen(end) != 0) {
        _queryElement *end_query = create_query_element("end", end);
        add_list_item(&params->_queries, end_query);
    }
    if (limit != NULL && strlen(limit) != 0) {
        _queryElement *limit_query = create_query_element("limit", limit);
        add_list_item(&params->_queries, limit_query);
    }

    return params;
}

OrderBookQuery *build_order_book_query(char *category, char *symbol, char *limit)
{
    OrderBookQuery *params = malloc(sizeof(OrderBookQuery));
    if (!params)
        return NULL;
    
    params->category = category;
    params->symbol = symbol;
    params->limit = limit;

    params->_queries = (Node *)calloc(1, sizeof(Node));

    // build _queries list
    if (category != NULL && strlen(category) != 0) {
        _queryElement *category_query = create_query_element("category", category);
        add_list_item(&params->_queries, category_query);
    }
    if (symbol != NULL && strlen(symbol) != 0) {
        _queryElement *symbol_query = create_query_element("symbol", symbol);
        add_list_item(&params->_queries, symbol_query);
    }
    if (limit != NULL && strlen(limit) != 0) {
        _queryElement *limit_query = create_query_element("limit", limit);
        add_list_item(&params->_queries, limit_query);
    }

    return params;
}

_queryElement *create_query_element(char *key, char *val)
{
    _queryElement *elem = malloc(sizeof(_queryElement));
    elem->key = strdup(key);
    elem->val = strdup(val);
    
    return elem;
}

void free_query_element(_queryElement *elem)
{
    if (elem) {
        free(elem->key);
        free(elem->val);
        free(elem);
    }

    elem = NULL;
}

void free_query_element_callback(void *elem)
{
    free_query_element((_queryElement *)elem);
}

void free_ticker(Ticker *ticker)
{
    free(ticker->symbol); 
    free(ticker->bid1_price); 
    free(ticker->bid1_size); 
    free(ticker->ask1_price); 
    free(ticker->ask1_size); 
    free(ticker->last_price); 
    free(ticker->prev_price_24h); 
    free(ticker->price_24h_pcnt); 
    free(ticker->high_price_24h); 
    free(ticker->low_price_24h); 
    free(ticker->turn_over_24h); 
    free(ticker->volume_24); 
    free(ticker->usd_index_price); 
    free(ticker);
    ticker = NULL;
}

// build kline from response
Kline *build_kline(const cJSON *list_item)
{
    Kline *kline = malloc(sizeof(Kline));
    if (!kline)
        return NULL;

    int size = cJSON_GetArraySize(list_item);
    if (size != 7)
        return NULL;

    cJSON *start_time = cJSON_GetArrayItem(list_item, 0);
    if(cJSON_IsString(start_time) && (start_time->valuestring != NULL))
        kline->start_time = strdup(start_time->valuestring);

    cJSON *open_price = cJSON_GetArrayItem(list_item, 1);
    if(cJSON_IsString(open_price) && (open_price->valuestring != NULL))
        kline->open_price = strdup(open_price->valuestring);

    cJSON *high_price = cJSON_GetArrayItem(list_item, 2);
    if(cJSON_IsString(high_price) && (high_price->valuestring != NULL))
        kline->high_price = strdup(high_price->valuestring);

    cJSON *low_price = cJSON_GetArrayItem(list_item, 3);
    if(cJSON_IsString(low_price) && (low_price->valuestring != NULL))
        kline->low_price = strdup(low_price->valuestring);

    cJSON *close_price = cJSON_GetArrayItem(list_item, 4);
    if(cJSON_IsString(close_price) && (close_price->valuestring != NULL))
        kline->close_price = strdup(close_price->valuestring);

    cJSON *volume = cJSON_GetArrayItem(list_item, 5);
    if(cJSON_IsString(volume) && (volume->valuestring != NULL))
        kline->volume = strdup(volume->valuestring);

    cJSON *turn_over = cJSON_GetArrayItem(list_item, 6);
    if(cJSON_IsString(turn_over) && (turn_over->valuestring != NULL))
        kline->turnover = strdup(turn_over->valuestring);

    return kline;
}

// build kline from response
Kline *build_price_kline(const cJSON *list_item)
{
    Kline *kline = malloc(sizeof(Kline));
    if (!kline)
        return NULL;

    int size = cJSON_GetArraySize(list_item);
    if (size != 5)
        return NULL;

    cJSON *start_time = cJSON_GetArrayItem(list_item, 0);
    if(cJSON_IsString(start_time) && (start_time->valuestring != NULL))
        kline->start_time = strdup(start_time->valuestring);

    cJSON *open_price = cJSON_GetArrayItem(list_item, 1);
    if(cJSON_IsString(open_price) && (open_price->valuestring != NULL))
        kline->open_price = strdup(open_price->valuestring);

    cJSON *high_price = cJSON_GetArrayItem(list_item, 2);
    if(cJSON_IsString(high_price) && (high_price->valuestring != NULL))
        kline->high_price = strdup(high_price->valuestring);

    cJSON *low_price = cJSON_GetArrayItem(list_item, 3);
    if(cJSON_IsString(low_price) && (low_price->valuestring != NULL))
        kline->low_price = strdup(low_price->valuestring);

    cJSON *close_price = cJSON_GetArrayItem(list_item, 4);
    if(cJSON_IsString(close_price) && (close_price->valuestring != NULL))
        kline->close_price = strdup(close_price->valuestring);

    return kline;
}

// build order from response
OrderB *build_orderb(const cJSON *list_item)
{
    OrderB *order = malloc(sizeof(OrderB));
    if (!order)
        return NULL;

    int size = cJSON_GetArraySize(list_item);
    if (size != 2)
        return NULL;

    cJSON *price = cJSON_GetArrayItem(list_item, 0);
    if(cJSON_IsString(price) && (price->valuestring != NULL))
        order->price = strdup(price->valuestring);

    cJSON *order_size = cJSON_GetArrayItem(list_item, 1);
    if(cJSON_IsString(order_size) && (order_size->valuestring != NULL))
        order->size = strdup(order_size->valuestring);

    return order;
}

void free_kline(Kline *kline)
{
    free(kline->start_time); 
    free(kline->open_price); 
    free(kline->high_price); 
    free(kline->low_price);
    free(kline->close_price);
    free(kline->volume); 
    free(kline->turnover); 
    free(kline);
    kline = NULL;
}

void free_price_kline(Kline *kline)
{
    free(kline->start_time); 
    free(kline->open_price); 
    free(kline->high_price); 
    free(kline->low_price);
    free(kline->close_price);
    free(kline);
    kline = NULL;
}

void free_order(OrderB *order)
{
    free(order->price); 
    free(order->size); 
    free(order);
    order = NULL;
}
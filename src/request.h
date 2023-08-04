#ifndef REQUEST_H
#define REQUEST_H

#include "cJSON.h"

typedef struct
{
    // category is required
    char *category;
    char *symbol;
    char *base_coin;
    char *exp_date;
} TickersQueryParams;

typedef struct
{
    char *symbol;
    char *bid1_price;
    char *bid1_size;
    char *ask1_price;
    char *ask1_size;
    char *last_price;
    char *prev_price_24h;
    char *price_24h_pcnt;
    char *high_price_24h;
    char *low_price_24h;
    char *turn_over_24h;
    char *volume_24;
    char *usd_index_price;
} Ticker;

typedef struct
{
    char *category;
    char *symbol;
    char *interval;
    char *start;
    char *end;
    char *limit;
} KlineQueryParams;

// the elements of this array will be the one specified int KlineFields
typedef struct
{
    char *start_time;
    char *open_price;
    char *high_price;
    char *low_price;
    char *close_price;
    char *volume;
    char *turnover;
} Kline;

// OrderBookQuery represents a query for order book
typedef struct
{
    char *category;
    char *symbol;
    char *limit;
} OrderBookQuery;

// OrderB represents an order in the order book.
typedef struct
{
    char *price; // price of the order
    char *size;  // size of the order
} OrderB;

Ticker *build_ticker(const cJSON *list_item);
void free_ticker(Ticker *ticker);
Kline *build_kline(const cJSON *list_item);
Kline *build_price_kline(const cJSON *list_item);
OrderB *build_orderb(const cJSON *list_item);
void free_kline(Kline *kline);
void free_price_kline(Kline *kline);
void free_order(OrderB *order);

#endif
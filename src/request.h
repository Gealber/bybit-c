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

Ticker *build_ticker(const cJSON *list_item);
void free_ticker(Ticker *ticker);

#endif
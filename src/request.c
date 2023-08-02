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

// void free_ts
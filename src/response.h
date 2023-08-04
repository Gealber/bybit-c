
#ifndef RESPONSE_H
#define RESPONSE_H

typedef struct TickerResponse
{
    char *category;
    struct Node *list;
} TickerResponse;

typedef struct TimeServerResponse
{
    char *time_second;
    char *time_nano;
} TimeServerResponse;

typedef struct KlineResponse
{
    char *category;
    char *symbol;
    struct Node *list;
} KlineResponse;

typedef struct OrderBookResponse
{
    char *symbol;
    struct Node *bids; // Bid, buyer. Sort by price desc
    struct Node *asks; // Ask, seller. Order by price asc
    char *ts;          // timestamp
    char *update_id;           // update id
} OrderBookResponse;

TickerResponse *parse_ticker_response(char *text);
TimeServerResponse *parse_ts_response(char *text);
KlineResponse *parse_kline_response(char *text);
KlineResponse *parse_price_kline_response(char *text);
OrderBookResponse *parse_order_book_response(char *text);

#endif
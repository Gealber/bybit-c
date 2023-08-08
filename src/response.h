
#ifndef RESPONSE_H
#define RESPONSE_H

typedef struct APIResponse
{
    int ret_code;
    char *ret_msg;
    void *result;
    int time;
} APIResponse;

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

typedef struct OrderResponse
{
    char *order_id;
    char *order_link_id;
} OrderResponse;

typedef struct OpenOrdersResponse
{
    char *category;
    char *next_page_cursor;
    struct Node *list;
} OpenOrdersResponse;

// detailed parsing of respond result
TickerResponse *parse_ticker_response(const cJSON *json);
TimeServerResponse *parse_ts_response(const cJSON *json);
KlineResponse *parse_kline_response(const cJSON *json);
KlineResponse *parse_price_kline_response(const cJSON *json);
OrderBookResponse *parse_order_book_response(const cJSON *json);
OrderResponse *parse_order_response(const cJSON *json);
OpenOrdersResponse *parse_open_orders_response(const cJSON *json);

// general parsing api response
APIResponse *parse_api_response(char *text, void *result_parsing_cb(const cJSON *));

// parsing callbacks
void *parse_ticker_response_cb(const cJSON *json);
void *parse_ts_response_cb(const cJSON *json);
void *parse_order_book_response_cb(const cJSON *json);
void *parse_price_kline_response_cb(const cJSON *json);
void *parse_kline_response_cb(const cJSON *json);
void *parse_order_response_cb(const cJSON *json);
void *parse_open_orders_response_cb(const cJSON *json);

// cleaning responses
void free_api_response(APIResponse *api_resp);

#endif
#ifndef REQUEST_H
#define REQUEST_H

#include "cJSON.h"
#include "common.h"

typedef struct
{
    char *key;
    char *val;
} _queryElement;

typedef struct
{
    // category is required
    char *category;
    char *symbol;
    char *base_coin;
    char *exp_date;
    struct Node *_queries;
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
    struct Node *_queries;
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
    struct Node *_queries;
} OrderBookQuery;

// OrderB represents an order in the order book.
typedef struct
{
    char *price; // price of the order
    char *size;  // size of the order
} OrderB;

typedef struct
{
    char *category;
    char *symbol;
    int is_leverage;
    char *side;
    char *order_type;
    char *qty;
    char *price;
    int trigger_direction;
    char *order_filter;
    char *trigger_price;
    char *trigger_by;
    char *order_lv;
    char *time_in_force;
    int position_idx;
    char *order_link_id;
    char *take_profit;
    char *stop_loss;
    char *tp_trigger_by;
    char *sl_trigger_by;
    bool reduce_only;
    bool close_on_trigger;
    char *smp_type;
    bool mmp;
    char *tpsl_mode;
    char *tp_limit_price;
    char *sl_limit_price;
    char *tp_order_type;
    char *sl_order_type;
} OrderRequest;

typedef struct
{
    char *category;
    char *symbol;
    char *order_id;
    char *order_link_id;
    char *order_lv;
    char *trigger_price;
    char *qty;
    char *price;
    char *take_profit;
    char *stop_loss;
    char *tp_trigger_by;
    char *sl_trigger_by;
    char *trigger_by;
    char *tp_limit_price;
    char *sl_limit_price;
} AmendOrderRequest;

typedef struct
{
    char *category;
    char *symbol;
    char *order_id;
    char *order_link_id;
    char *order_filter;
} CancelOrderRequest;

typedef struct
{
    char *category;
    char *symbol;
    char *settle_coin;
    char *order_id;
    char *order_link_id;
    int open_only;
    char *order_filter;
    int limit;
    char *cursor;
    struct Node *_queries;
} OpenOrdersQuery;

typedef struct
{
    char *order_id;
    char *order_link_id;
    char *block_trade_id;
    char *symbol;
    char *price;
    char *qty;
    char *side;
    char *is_leverage;
    int position_idx;
    char *order_status;
    char *cancel_type;
    char *reject_reason;
    char *avg_price;
    char *leaves_qty;
    char *leaves_value;
    char *cum_exec_qty;
    char *cum_exec_value;
    char *cum_exec_fee;
    char *time_in_force;
    char *order_type;
    char *stop_order_type;
    char *order_lv;
    char *trigger_price;
    char *take_profit;
    char *stop_loss;
    char *tpsl_mode;
    char *tp_limit_price;
    char *sl_limit_price;
    char *tp_trigger_by;
    char *sl_trigger_by;
    int trigger_direction;
    char *trigger_by;
    char *last_price_on_created;
    bool reduce_only;
    bool close_on_trigger;
    char *place_type;
    char *smp_type;
    int smp_group;
    char *smp_order_id;
    char *created_time;
    char *updated_time;
} OpenOrder;


// building structs from response
Ticker *build_ticker(const cJSON *list_item);
void free_ticker(Ticker *ticker);
Kline *build_kline(const cJSON *list_item);
Kline *build_price_kline(const cJSON *list_item);
OrderB *build_orderb(const cJSON *list_item);
OpenOrder *build_open_order(const cJSON *list_item);

// query structs
TickersQueryParams *build_ticker_query(char *category, char *symbol, char *base_coin, char *exp_date);
KlineQueryParams *build_kline_query(char *category, char *symbol, char *interval, char *start, char *end, char *limit);
OrderBookQuery *build_order_book_query(char *category, char *symbol, char *limit);
OpenOrdersQuery *build_open_orders_query(char *category, char *symbol, char *settle_coin, char *order_id, char *order_link_id, int open_only, char *order_filter, int limit, char *cursor);
_queryElement *create_query_element(char *key, char *val);

// struct to json
// initialize an empty OrderRequest struct
OrderRequest *init_order_request();
char *order_request_tojson(OrderRequest *order_request);
AmendOrderRequest *init_amend_order_request();
char *amend_order_request_tojson(AmendOrderRequest *amend_order_request);
CancelOrderRequest *init_cancel_order_request();
char *cancel_order_request_tojson(CancelOrderRequest *cancel_order_request);

// methods for releasing memory
void free_kline(Kline *kline);
void free_price_kline(Kline *kline);
void free_orderb(OrderB *order);
void free_open_order(OpenOrder *open_order);
// realeasing memory for queries
void free_query_element(_queryElement *elem);
void free_query_element_cb(void *elem);
// realeasing body json
void free_order_request(OrderRequest *order_request);

#endif
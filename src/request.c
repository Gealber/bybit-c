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

    if (category != NULL && strlen(category) != 0)
    {
        _queryElement *category_query = create_query_element("category", category);
        add_list_item(&params->_queries, category_query);
    }
    if (symbol != NULL && strlen(symbol) != 0)
    {
        _queryElement *symbol_query = create_query_element("symbol", symbol);
        add_list_item(&params->_queries, symbol_query);
    }
    if (base_coin != NULL && strlen(base_coin) != 0)
    {
        _queryElement *base_coin_query = create_query_element("baseCoin", base_coin);
        add_list_item(&params->_queries, base_coin_query);
    }
    if (exp_date != NULL && strlen(exp_date) != 0)
    {
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
    if (category != NULL && strlen(category) != 0)
    {
        _queryElement *category_query = create_query_element("category", category);
        add_list_item(&params->_queries, category_query);
    }
    if (symbol != NULL && strlen(symbol) != 0)
    {
        _queryElement *symbol_query = create_query_element("symbol", symbol);
        add_list_item(&params->_queries, symbol_query);
    }
    if (interval && strlen(interval) != 0)
    {
        _queryElement *interval_query = create_query_element("interval", interval);
        add_list_item(&params->_queries, interval_query);
    }
    if (start && strlen(start) != 0)
    {
        _queryElement *start_query = create_query_element("start", start);
        add_list_item(&params->_queries, start_query);
    }
    if (end && strlen(end) != 0)
    {
        _queryElement *end_query = create_query_element("end", end);
        add_list_item(&params->_queries, end_query);
    }
    if (limit != NULL && strlen(limit) != 0)
    {
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
    if (category != NULL && strlen(category) != 0)
    {
        _queryElement *category_query = create_query_element("category", category);
        add_list_item(&params->_queries, category_query);
    }
    if (symbol != NULL && strlen(symbol) != 0)
    {
        _queryElement *symbol_query = create_query_element("symbol", symbol);
        add_list_item(&params->_queries, symbol_query);
    }
    if (limit != NULL && strlen(limit) != 0)
    {
        _queryElement *limit_query = create_query_element("limit", limit);
        add_list_item(&params->_queries, limit_query);
    }

    return params;
}

OpenOrdersQuery *build_open_orders_query(char *category, char *symbol, char *settle_coin, char *order_id, char *order_link_id, int open_only, char *order_filter, int limit, char *cursor)
{
    OpenOrdersQuery *params = malloc(sizeof(OpenOrdersQuery));
    if (!params)
        return NULL;

    params->category = category;
    params->symbol = symbol;
    params->settle_coin = settle_coin;
    params->order_id = order_id;
    params->order_link_id = order_link_id;
    params->open_only = open_only;
    params->order_filter = order_filter;
    params->limit = limit;
    params->cursor = cursor;

    params->_queries = (Node *)calloc(1, sizeof(Node));

    // build _queries list
    if (category != NULL && strlen(category) != 0)
    {
        _queryElement *category_query = create_query_element("category", category);
        add_list_item(&params->_queries, category_query);
    }
    if (symbol != NULL && strlen(symbol) != 0)
    {
        _queryElement *symbol_query = create_query_element("symbol", symbol);
        add_list_item(&params->_queries, symbol_query);
    }
    if (settle_coin != NULL && strlen(settle_coin) != 0)
    {
        _queryElement *settle_coin_query = create_query_element("settleCoin", settle_coin);
        add_list_item(&params->_queries, settle_coin_query);
    }
    if (order_id != NULL && strlen(order_id) != 0)
    {
        _queryElement *order_id_query = create_query_element("orderId", order_id);
        add_list_item(&params->_queries, order_id_query);
    }
    if (order_link_id != NULL && strlen(order_link_id) != 0)
    {
        _queryElement *order_link_id_query = create_query_element("orderLinkId", order_link_id);
        add_list_item(&params->_queries, order_link_id_query);
    }
    _queryElement *open_only_query = create_query_element("openOnly", (char *)bool_to_string(open_only));
    add_list_item(&params->_queries, open_only_query);
    if (order_filter != NULL && strlen(order_filter) != 0)
    {
        _queryElement *order_filter_query = create_query_element("orderFilter", order_filter);
        add_list_item(&params->_queries, order_filter_query);
    }
    if (limit >= 1 && limit <= 50)
    {
        char limit_str[3];
        sprintf(limit_str, "%d", limit);
        _queryElement *limit_query = create_query_element("limit", limit_str);
        add_list_item(&params->_queries, limit_query);
    }
    if (cursor != NULL && strlen(cursor) != 0)
    {
        _queryElement *cursor_query = create_query_element("cursor", cursor);
        add_list_item(&params->_queries, cursor_query);
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
    if (cJSON_IsString(start_time) && (start_time->valuestring != NULL))
        kline->start_time = strdup(start_time->valuestring);

    cJSON *open_price = cJSON_GetArrayItem(list_item, 1);
    if (cJSON_IsString(open_price) && (open_price->valuestring != NULL))
        kline->open_price = strdup(open_price->valuestring);

    cJSON *high_price = cJSON_GetArrayItem(list_item, 2);
    if (cJSON_IsString(high_price) && (high_price->valuestring != NULL))
        kline->high_price = strdup(high_price->valuestring);

    cJSON *low_price = cJSON_GetArrayItem(list_item, 3);
    if (cJSON_IsString(low_price) && (low_price->valuestring != NULL))
        kline->low_price = strdup(low_price->valuestring);

    cJSON *close_price = cJSON_GetArrayItem(list_item, 4);
    if (cJSON_IsString(close_price) && (close_price->valuestring != NULL))
        kline->close_price = strdup(close_price->valuestring);

    cJSON *volume = cJSON_GetArrayItem(list_item, 5);
    if (cJSON_IsString(volume) && (volume->valuestring != NULL))
        kline->volume = strdup(volume->valuestring);

    cJSON *turn_over = cJSON_GetArrayItem(list_item, 6);
    if (cJSON_IsString(turn_over) && (turn_over->valuestring != NULL))
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
    if (cJSON_IsString(start_time) && (start_time->valuestring != NULL))
        kline->start_time = strdup(start_time->valuestring);

    cJSON *open_price = cJSON_GetArrayItem(list_item, 1);
    if (cJSON_IsString(open_price) && (open_price->valuestring != NULL))
        kline->open_price = strdup(open_price->valuestring);

    cJSON *high_price = cJSON_GetArrayItem(list_item, 2);
    if (cJSON_IsString(high_price) && (high_price->valuestring != NULL))
        kline->high_price = strdup(high_price->valuestring);

    cJSON *low_price = cJSON_GetArrayItem(list_item, 3);
    if (cJSON_IsString(low_price) && (low_price->valuestring != NULL))
        kline->low_price = strdup(low_price->valuestring);

    cJSON *close_price = cJSON_GetArrayItem(list_item, 4);
    if (cJSON_IsString(close_price) && (close_price->valuestring != NULL))
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
    if (cJSON_IsString(price) && (price->valuestring != NULL))
        order->price = strdup(price->valuestring);

    cJSON *order_size = cJSON_GetArrayItem(list_item, 1);
    if (cJSON_IsString(order_size) && (order_size->valuestring != NULL))
        order->size = strdup(order_size->valuestring);

    return order;
}

// build order from response
CancelledOrder *build_cancelled_order(const cJSON *list_item)
{
    CancelledOrder *order = malloc(sizeof(CancelledOrder));
    if (!order)
        return NULL;

    int size = cJSON_GetArraySize(list_item);
    if (size != 2)
        return NULL;

    cJSON *order_id = cJSON_GetArrayItem(list_item, 0);
    if (cJSON_IsString(order_id) && (order_id->valuestring != NULL))
        order->order_id = strdup(order_id->valuestring);

    cJSON *order_link_id = cJSON_GetArrayItem(list_item, 1);
    if (cJSON_IsString(order_link_id) && (order_link_id->valuestring != NULL))
        order->order_link_id = strdup(order_link_id->valuestring);

    return order;
}

// build open order from response
OpenOrder *build_open_order(const cJSON *list_item)
{
    OpenOrder *order = malloc(sizeof(OpenOrder));
    if (!order)
        return NULL;

    int size = cJSON_GetArraySize(list_item);
    if (size != 41)
        return NULL;

    cJSON *order_id = cJSON_GetArrayItem(list_item, 0);
    if (cJSON_IsString(order_id) && (order_id->valuestring != NULL))
        order->order_id = strdup(order_id->valuestring);
    cJSON *order_link_id = cJSON_GetArrayItem(list_item, 1);
    if (cJSON_IsString(order_link_id) && (order_link_id->valuestring != NULL))
        order->order_link_id = strdup(order_link_id->valuestring);
    cJSON *block_trade_id = cJSON_GetArrayItem(list_item, 2);
    if (cJSON_IsString(block_trade_id) && (block_trade_id->valuestring != NULL))
        order->block_trade_id = strdup(block_trade_id->valuestring);
    cJSON *symbol = cJSON_GetArrayItem(list_item, 3);
    if (cJSON_IsString(symbol) && (symbol->valuestring != NULL))
        order->symbol = strdup(symbol->valuestring);
    cJSON *price = cJSON_GetArrayItem(list_item, 4);
    if (cJSON_IsString(price) && (price->valuestring != NULL))
        order->price = strdup(price->valuestring);
    cJSON *qty = cJSON_GetArrayItem(list_item, 5);
    if (cJSON_IsString(qty) && (qty->valuestring != NULL))
        order->qty = strdup(qty->valuestring);
    cJSON *side = cJSON_GetArrayItem(list_item, 6);
    if (cJSON_IsString(side) && (side->valuestring != NULL))
        order->side = strdup(side->valuestring);
    cJSON *is_leverage = cJSON_GetArrayItem(list_item, 7);
    if (cJSON_IsString(is_leverage) && (is_leverage->valuestring != NULL))
        order->is_leverage = strdup(is_leverage->valuestring);
    cJSON *position_idx = cJSON_GetArrayItem(list_item, 8);
    if (cJSON_IsNumber(position_idx))
        order->position_idx = position_idx->valueint;
    cJSON *order_status = cJSON_GetArrayItem(list_item, 9);
    if (cJSON_IsString(order_status) && (order_status->valuestring != NULL))
        order->order_status = strdup(order_status->valuestring);
    cJSON *cancel_type = cJSON_GetArrayItem(list_item, 10);
    if (cJSON_IsString(cancel_type) && (cancel_type->valuestring != NULL))
        order->cancel_type = strdup(cancel_type->valuestring);
    cJSON *reject_reason = cJSON_GetArrayItem(list_item, 11);
    if (cJSON_IsString(reject_reason) && (reject_reason->valuestring != NULL))
        order->reject_reason = strdup(reject_reason->valuestring);
    cJSON *avg_price = cJSON_GetArrayItem(list_item, 12);
    if (cJSON_IsString(avg_price) && (avg_price->valuestring != NULL))
        order->avg_price = strdup(avg_price->valuestring);
    cJSON *leaves_qty = cJSON_GetArrayItem(list_item, 13);
    if (cJSON_IsString(leaves_qty) && (leaves_qty->valuestring != NULL))
        order->leaves_qty = strdup(leaves_qty->valuestring);
    cJSON *leaves_value = cJSON_GetArrayItem(list_item, 14);
    if (cJSON_IsString(leaves_value) && (leaves_value->valuestring != NULL))
        order->leaves_value = strdup(leaves_value->valuestring);
    cJSON *cum_exec_qty = cJSON_GetArrayItem(list_item, 15);
    if (cJSON_IsString(cum_exec_qty) && (cum_exec_qty->valuestring != NULL))
        order->cum_exec_qty = strdup(cum_exec_qty->valuestring);
    cJSON *cum_exec_value = cJSON_GetArrayItem(list_item, 16);
    if (cJSON_IsString(cum_exec_value) && (cum_exec_value->valuestring != NULL))
        order->cum_exec_value = strdup(cum_exec_value->valuestring);
    cJSON *cum_exec_fee = cJSON_GetArrayItem(list_item, 17);
    if (cJSON_IsString(cum_exec_fee) && (cum_exec_fee->valuestring != NULL))
        order->cum_exec_fee = strdup(cum_exec_fee->valuestring);
    cJSON *time_in_force = cJSON_GetArrayItem(list_item, 18);
    if (cJSON_IsString(time_in_force) && (time_in_force->valuestring != NULL))
        order->time_in_force = strdup(time_in_force->valuestring);
    cJSON *order_type = cJSON_GetArrayItem(list_item, 19);
    if (cJSON_IsString(order_type) && (order_type->valuestring != NULL))
        order->order_type = strdup(order_type->valuestring);
    cJSON *stop_order_type = cJSON_GetArrayItem(list_item, 20);
    if (cJSON_IsString(stop_order_type) && (stop_order_type->valuestring != NULL))
        order->stop_order_type = strdup(stop_order_type->valuestring);
    cJSON *order_lv = cJSON_GetArrayItem(list_item, 21);
    if (cJSON_IsString(order_lv) && (order_lv->valuestring != NULL))
        order->order_lv = strdup(order_lv->valuestring);
    cJSON *trigger_price = cJSON_GetArrayItem(list_item, 22);
    if (cJSON_IsString(trigger_price) && (trigger_price->valuestring != NULL))
        order->trigger_price = strdup(trigger_price->valuestring);
    cJSON *take_profit = cJSON_GetArrayItem(list_item, 23);
    if (cJSON_IsString(take_profit) && (take_profit->valuestring != NULL))
        order->take_profit = strdup(take_profit->valuestring);
    cJSON *stop_loss = cJSON_GetArrayItem(list_item, 24);
    if (cJSON_IsString(stop_loss) && (stop_loss->valuestring != NULL))
        order->stop_loss = strdup(stop_loss->valuestring);
    cJSON *tpsl_mode = cJSON_GetArrayItem(list_item, 25);
    if (cJSON_IsString(tpsl_mode) && (tpsl_mode->valuestring != NULL))
        order->tpsl_mode = strdup(tpsl_mode->valuestring);
    cJSON *tp_limit_price = cJSON_GetArrayItem(list_item, 26);
    if (cJSON_IsString(tp_limit_price) && (tp_limit_price->valuestring != NULL))
        order->tp_limit_price = strdup(tp_limit_price->valuestring);
    cJSON *sl_limit_price = cJSON_GetArrayItem(list_item, 27);
    if (cJSON_IsString(sl_limit_price) && (sl_limit_price->valuestring != NULL))
        order->sl_limit_price = strdup(sl_limit_price->valuestring);
    cJSON *tp_trigger_by = cJSON_GetArrayItem(list_item, 28);
    if (cJSON_IsString(tp_trigger_by) && (tp_trigger_by->valuestring != NULL))
        order->tp_trigger_by = strdup(tp_trigger_by->valuestring);
    cJSON *sl_trigger_by = cJSON_GetArrayItem(list_item, 29);
    if (cJSON_IsString(sl_trigger_by) && (sl_trigger_by->valuestring != NULL))
        order->sl_trigger_by = strdup(sl_trigger_by->valuestring);
    cJSON *trigger_direction = cJSON_GetArrayItem(list_item, 30);
    if (cJSON_IsNumber(trigger_direction))
        order->trigger_direction = trigger_direction->valueint;
    cJSON *trigger_by = cJSON_GetArrayItem(list_item, 31);
    if (cJSON_IsString(trigger_by) && (trigger_by->valuestring != NULL))
        order->trigger_by = strdup(trigger_by->valuestring);
    cJSON *last_price_on_created = cJSON_GetArrayItem(list_item, 32);
    if (cJSON_IsString(last_price_on_created) && (last_price_on_created->valuestring != NULL))
        order->last_price_on_created = strdup(last_price_on_created->valuestring);
    cJSON *reduce_only = cJSON_GetArrayItem(list_item, 33);
    if (cJSON_IsBool(reduce_only))
    {
        if (strncmp(reduce_only->valuestring, "true", 4))
        {
            order->reduce_only = 1;
        }
        else
        {
            order->reduce_only = 0;
        }
    }
    cJSON *close_on_trigger = cJSON_GetArrayItem(list_item, 34);
    if (cJSON_IsBool(close_on_trigger))
    {
        if (strncmp(close_on_trigger->valuestring, "true", 4))
        {
            order->close_on_trigger = 1;
        }
        else
        {
            order->close_on_trigger = 0;
        }
    }
    cJSON *place_type = cJSON_GetArrayItem(list_item, 35);
    if (cJSON_IsString(place_type) && (place_type->valuestring != NULL))
        order->place_type = strdup(place_type->valuestring);
    cJSON *smp_type = cJSON_GetArrayItem(list_item, 36);
    if (cJSON_IsString(smp_type) && (smp_type->valuestring != NULL))
        order->smp_type = strdup(smp_type->valuestring);
    cJSON *smp_group = cJSON_GetArrayItem(list_item, 37);
    if (cJSON_IsNumber(smp_group))
        order->smp_group = smp_group->valueint;
    cJSON *smp_order_id = cJSON_GetArrayItem(list_item, 38);
    if (cJSON_IsString(smp_order_id) && (smp_order_id->valuestring != NULL))
        order->smp_order_id = strdup(smp_order_id->valuestring);
    cJSON *created_time = cJSON_GetArrayItem(list_item, 39);
    if (cJSON_IsString(created_time) && (created_time->valuestring != NULL))
        order->created_time = strdup(created_time->valuestring);
    cJSON *updated_time = cJSON_GetArrayItem(list_item, 40);
    if (cJSON_IsString(updated_time) && (updated_time->valuestring != NULL))
        order->updated_time = strdup(updated_time->valuestring);

    return order;
}

OrderRequest *init_order_request()
{
    OrderRequest *order = calloc(1, sizeof(OrderRequest));
    if (!order)
        return NULL;

    order->category = NULL;
    order->symbol = NULL;
    order->side = NULL;
    order->order_type = NULL;
    order->qty = NULL;
    order->price = NULL;
    order->order_filter = NULL;
    order->trigger_price = NULL;
    order->trigger_by = NULL;
    order->order_lv = NULL;
    order->time_in_force = NULL;
    order->order_link_id = NULL;
    order->take_profit = NULL;
    order->stop_loss = NULL;
    order->tp_trigger_by = NULL;
    order->sl_trigger_by = NULL;
    order->smp_type = NULL;
    order->tpsl_mode = NULL;
    order->tp_limit_price = NULL;
    order->sl_limit_price = NULL;
    order->tp_order_type = NULL;

    return order;
}

CancelAllOrders *init_cancel_all_orders_request()
{
    CancelAllOrders *request = calloc(1, sizeof(CancelAllOrders));
    if (!request)
        return NULL;

    request->category = NULL;
    request->symbol = NULL;
    request->base_coin = NULL;
    request->settle_coin = NULL;
    request->order_filter = NULL;

    return request;
}

// order_request_tojson returns the json string of the struct
char *order_request_tojson(OrderRequest *order_request)
{
    char *buff = NULL;

    cJSON *json = cJSON_CreateObject();
    if (!json)
        return NULL;

    add_string_field(json, "category", order_request->category);
    add_string_field(json, "symbol", order_request->symbol);
    add_int_field(json, "isLeverage", order_request->is_leverage);
    add_string_field(json, "side", order_request->side);
    add_string_field(json, "orderType", order_request->order_type);
    add_string_field(json, "qty", order_request->qty);
    add_string_field(json, "price", order_request->price);
    add_int_field(json, "triggerDirection", order_request->trigger_direction);
    add_string_field(json, "orderFilter", order_request->order_filter);
    add_string_field(json, "triggerPrice", order_request->trigger_price);
    add_string_field(json, "triggerBy", order_request->trigger_by);
    add_string_field(json, "orderLv", order_request->order_lv);
    add_string_field(json, "timeInForce", order_request->time_in_force);
    add_int_field(json, "positionIdx", order_request->position_idx);
    add_string_field(json, "orderLinkId", order_request->order_link_id);
    add_string_field(json, "takeProfit", order_request->take_profit);
    add_string_field(json, "stopLoss", order_request->stop_loss);
    add_string_field(json, "tpTriggerBy", order_request->tp_trigger_by);
    add_string_field(json, "slTriggerBy", order_request->sl_trigger_by);
    add_bool_field(json, "reduceOnly", order_request->reduce_only);
    add_bool_field(json, "closeOnTrigger", order_request->close_on_trigger);
    add_string_field(json, "smpType", order_request->smp_type);
    add_bool_field(json, "mmp", order_request->mmp);
    add_string_field(json, "tpslMode", order_request->tpsl_mode);
    add_string_field(json, "tpLimitPrice", order_request->tp_limit_price);
    add_string_field(json, "slLimitPrice", order_request->sl_limit_price);
    add_string_field(json, "tpOrderType", order_request->tp_order_type);
    add_string_field(json, "slOrderType", order_request->sl_order_type);

    buff = cJSON_Print(json);
    clean_string(buff);

    cJSON_Delete(json);

    return buff;
}

AmendOrderRequest *init_amend_order_request()
{
    AmendOrderRequest *amend = calloc(1, sizeof(AmendOrderRequest));
    if (!amend)
        return NULL;

    amend->category = NULL;
    amend->symbol = NULL;
    amend->order_id = NULL;
    amend->order_link_id = NULL;
    amend->order_lv = NULL;
    amend->trigger_price = NULL;
    amend->qty = NULL;
    amend->price = NULL;
    amend->take_profit = NULL;
    amend->stop_loss = NULL;
    amend->tp_trigger_by = NULL;
    amend->sl_trigger_by = NULL;
    amend->trigger_by = NULL;
    amend->tp_limit_price = NULL;
    amend->sl_limit_price = NULL;

    return amend;
}

char *amend_order_request_tojson(AmendOrderRequest *amend_order_request)
{
    char *buff = NULL;

    cJSON *json = cJSON_CreateObject();
    if (!json)
        return NULL;

    add_string_field(json, "category", amend_order_request->category);
    add_string_field(json, "symbol", amend_order_request->symbol);
    add_string_field(json, "orderId", amend_order_request->order_id);
    add_string_field(json, "orderLinkId", amend_order_request->order_link_id);
    add_string_field(json, "orderLv", amend_order_request->order_lv);
    add_string_field(json, "triggerPrice", amend_order_request->trigger_price);
    add_string_field(json, "qty", amend_order_request->qty);
    add_string_field(json, "price", amend_order_request->price);
    add_string_field(json, "takeProfit", amend_order_request->take_profit);
    add_string_field(json, "stopLoss", amend_order_request->stop_loss);
    add_string_field(json, "triggerBy", amend_order_request->trigger_by);
    add_string_field(json, "tpTriggerBy", amend_order_request->tp_trigger_by);
    add_string_field(json, "slTriggerBy", amend_order_request->sl_trigger_by);
    add_string_field(json, "tpLimitPrice", amend_order_request->tp_limit_price);
    add_string_field(json, "slLimitPrice", amend_order_request->sl_limit_price);

    buff = cJSON_Print(json);
    clean_string(buff);

    cJSON_Delete(json);

    return buff;
}

CancelOrderRequest *init_cancel_order_request()
{
    CancelOrderRequest *cancel = calloc(1, sizeof(CancelOrderRequest));
    if (!cancel)
        return NULL;

    cancel->category = NULL;
    cancel->symbol = NULL;
    cancel->order_id = NULL;
    cancel->order_link_id = NULL;

    return cancel;
}

char *cancel_order_request_tojson(CancelOrderRequest *cancel_order_request)
{
    char *buff = NULL;

    cJSON *json = cJSON_CreateObject();
    if (!json)
        return NULL;

    add_string_field(json, "category", cancel_order_request->category);
    add_string_field(json, "symbol", cancel_order_request->symbol);
    add_string_field(json, "orderId", cancel_order_request->order_id);
    add_string_field(json, "orderLinkId", cancel_order_request->order_link_id);

    buff = cJSON_Print(json);
    clean_string(buff);

    cJSON_Delete(json);

    return buff;
}

char *cancel_all_orders_request_tojson(CancelAllOrders *cancel_all_orders_request)
{
    char *buff = NULL;

    cJSON *json = cJSON_CreateObject();
    if (!json)
        return NULL;

    add_string_field(json, "category", cancel_all_orders_request->category);
    add_string_field(json, "symbol", cancel_all_orders_request->symbol);
    add_string_field(json, "baseCoin", cancel_all_orders_request->base_coin);
    add_string_field(json, "settleCoin", cancel_all_orders_request->settle_coin);
    add_string_field(json, "orderFilter", cancel_all_orders_request->order_filter);

    buff = cJSON_Print(json);
    clean_string(buff);

    cJSON_Delete(json);

    return buff;
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

void free_orderb(OrderB *order)
{
    free(order->price);
    free(order->size);
    free(order);
    order = NULL;
}

void free_open_order(OpenOrder *open_order)
{
    free(open_order->order_id);
    free(open_order->order_link_id);
    free(open_order->block_trade_id);
    free(open_order->symbol);
    free(open_order->price);
    free(open_order->qty);
    free(open_order->side);
    free(open_order->is_leverage);
    free(open_order->order_status);
    free(open_order->cancel_type);
    free(open_order->reject_reason);
    free(open_order->avg_price);
    free(open_order->leaves_qty);
    free(open_order->leaves_value);
    free(open_order->cum_exec_qty);
    free(open_order->cum_exec_value);
    free(open_order->cum_exec_fee);
    free(open_order->time_in_force);
    free(open_order->order_type);
    free(open_order->stop_order_type);
    free(open_order->order_lv);
    free(open_order->trigger_price);
    free(open_order->take_profit);
    free(open_order->stop_loss);
    free(open_order->tpsl_mode);
    free(open_order->tp_limit_price);
    free(open_order->sl_limit_price);
    free(open_order->tp_trigger_by);
    free(open_order->sl_trigger_by);
    free(open_order->trigger_by);
    free(open_order->last_price_on_created);
    free(open_order->place_type);
    free(open_order->smp_type);
    free(open_order->smp_order_id);
    free(open_order->created_time);
    free(open_order->updated_time);
}

void free_query_element(_queryElement *elem)
{
    if (elem)
    {
        free(elem->key);
        free(elem->val);
        free(elem);
    }

    elem = NULL;
}

void free_query_element_cb(void *elem)
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

void free_cancelled_order(CancelledOrder *cancelled_order)
{
    free(cancelled_order->order_id);
    free(cancelled_order->order_link_id);
    free(cancelled_order);
    cancelled_order = NULL;
}

// free_order_request free memory for order requests
void free_order_request(OrderRequest *order_request)
{
    if (order_request->category)
        free(order_request->category);
    if (order_request->symbol)
        free(order_request->symbol);
    if (order_request->side)
        free(order_request->side);
    if (order_request->order_type)
        free(order_request->order_type);
    if (order_request->qty)
        free(order_request->qty);
    if (order_request->price)
        free(order_request->price);
    if (order_request->order_filter)
        free(order_request->order_filter);
    if (order_request->trigger_price)
        free(order_request->trigger_price);
    if (order_request->trigger_by)
        free(order_request->trigger_by);
    if (order_request->order_lv)
        free(order_request->order_lv);
    if (order_request->time_in_force)
        free(order_request->time_in_force);
    if (order_request->order_link_id)
        free(order_request->order_link_id);
    if (order_request->take_profit)
        free(order_request->take_profit);
    if (order_request->stop_loss)
        free(order_request->stop_loss);
    if (order_request->tp_trigger_by)
        free(order_request->tp_trigger_by);
    if (order_request->sl_trigger_by)
        free(order_request->sl_trigger_by);
    if (order_request->smp_type)
        free(order_request->smp_type);
    if (order_request->tpsl_mode)
        free(order_request->tpsl_mode);
    if (order_request->tp_limit_price)
        free(order_request->tp_limit_price);
    if (order_request->sl_limit_price)
        free(order_request->sl_limit_price);
    if (order_request->tp_order_type)
        free(order_request->tp_order_type);
    if (order_request->sl_order_type)
        free(order_request->sl_order_type);
}
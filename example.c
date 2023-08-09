#include <stdio.h>
#include <stdlib.h>
#include "src/bybit.h"
#include "src/common.h"
#include "src/request.h"
#include "src/response.h"

void place_cancel_all_orders();
void place_cancel_order();
void amend_order();
void place_order();
void retrieve_ticker();
void retrieve_time_server();
void retrieve_kline();
void retrieve_open_orders();
void retrieve_price_kline();
void retrieve_order_book();
void print_ticker(Ticker *ticker);
void print_kline(Kline *kline);
void print_price_kline(Kline *kline);
void print_order_book(OrderB *order);
void print_open_order(OpenOrder *order);

int main(int argc, char *argv[])
{
	/*Uncomment these lines to run examples*/
	// retrieve_time_server();
	// place_order();
	// place_cancel_order();
	// amend_order();
	// retrieve_ticker();
	// retrieve_kline();
	// retrieve_price_kline();
	// retrieve_order_book();
	// retrieve_open_orders();
	place_cancel_all_orders();
	return 0;
}

void place_cancel_all_orders()
{
	// Client *clt = new("<API_KEY>", "<API_SECRET>");
	Client *clt = new("wLRb7YfUhzBYjcs8gY", "kKve5Z13n8t16tV7dooSKfs9robjXX2H6eQD");
	if (!clt)
		return ;

	// allocate memory and initialize order request
	CancelAllOrders *cancel_all_orders_request = init_cancel_all_orders_request();
	if (!cancel_all_orders_request)
		return ;

	cancel_all_orders_request->category = "spot";
	cancel_all_orders_request->symbol = "BTCUSDT";
	cancel_all_orders_request->base_coin = "BTC";
	
	APIResponse *api_resp = post_cancel_all_orders(clt, cancel_all_orders_request);
	if (!api_resp)
		goto end;

	if (api_resp->ret_code != 0)
	{
		printf("response code: %d with message: %s\n", api_resp->ret_code, api_resp->ret_msg);
		goto end;
	}

	if (!api_resp->result) {
		printf("RESULT IS NULL\n");
		goto end;
	}

	CancelAllOrdersResponse *resp = (CancelAllOrdersResponse *)api_resp->result;
	if (!resp)
		goto end;

	struct Node *cur = resp->list;
	if (cur == NULL)
		goto end;

	int i = 0;
	while (cur != NULL)
	{
		CancelledOrder *order = (CancelledOrder *)cur->val;
		printf("-------------------> %d\n", i);
		printf("order_id: %s\n", order->order_id);
		printf("order_link_id: %s\n", order->order_link_id);
		cur = cur->next;
		i++;
	}

	Node *tmp = NULL;
	cur = resp->list;
	while (cur != NULL)
	{
		free_cancelled_order(cur->val);
		tmp = cur;
		cur = cur->next;
		free(tmp);
	}

end:
	if (cancel_all_orders_request) free(cancel_all_orders_request);
	if (api_resp) free_api_response(api_resp);
	if (clt) free(clt);

	return ;
}

void retrieve_open_orders()
{
	Client *clt = new("<API_KEY>", "<API_SECRET>");
	if (!clt)
		return ;

	// allocate memory and initialize order request
	OpenOrdersQuery *open_orders_query = build_open_orders_query("spot", "", "", "", "", 0, "", 10, "");
	if (!open_orders_query)
		return ;

	APIResponse *api_resp = get_open_orders(clt, open_orders_query);
	if (!api_resp)
		goto end;

	if (api_resp->ret_code != 0)
	{
		printf("response code: %d with message: %s\n", api_resp->ret_code, api_resp->ret_msg);
		goto end;
	}

	if (!api_resp->result) {
		printf("RESULT IS NULL\n");
		goto end;
	}

	OpenOrdersResponse *resp = (OpenOrdersResponse *)api_resp->result;
	if (!resp)
		goto end;

	struct Node *cur = resp->list;
	if (cur == NULL)
		goto end;

	int i = 0;
	while (cur != NULL)
	{
		OpenOrder *order = (OpenOrder *)cur->val;
		printf("-------------------> %d\n", i);
		print_open_order(order);
		cur = cur->next;
		i++;
	}

	Node *tmp = NULL;
	cur = resp->list;
	while (cur != NULL)
	{
		free_open_order(cur->val);
		tmp = cur;
		cur = cur->next;
		free(tmp);
	}

	free(resp->category);
	free(resp->next_page_cursor);
	

end:
	if (api_resp) free_api_response(api_resp);
	if (clt) free(clt);
	if (open_orders_query)
	{
		free_list(&open_orders_query->_queries, free_query_element_cb);
		free(open_orders_query);
	}

	return ;
}

void place_cancel_order()
{
	Client *clt = new("<API_KEY>", "<API_SECRET>");
	if (!clt)
		return ;

	// allocate memory and initialize order request
	CancelOrderRequest *cancel_order_request = init_cancel_order_request();
	if (!cancel_order_request)
		return ;

	cancel_order_request->category = "spot";
	cancel_order_request->symbol = "BTCUSDT";
    cancel_order_request->order_id = "1482027329061466624";
    cancel_order_request->order_link_id = 0;
	
	APIResponse *api_resp = post_cancel_order(clt, cancel_order_request);
	if (!api_resp)
		goto end;

	if (api_resp->ret_code != 0)
	{
		printf("response code: %d with message: %s\n", api_resp->ret_code, api_resp->ret_msg);
		goto end;
	}

	if (!api_resp->result) {
		printf("RESULT IS NULL\n");
		goto end;
	}

	OrderResponse *resp = (OrderResponse *)api_resp->result;
	if (!resp)
		goto end;

	printf("ORDER ID: %s\n", resp->order_id);
	printf("ORDER LINK ID: %s\n", resp->order_link_id);
	
	if (resp->order_id) free(resp->order_id);
	if (resp->order_link_id) free(resp->order_link_id);

end:
	if (cancel_order_request) free(cancel_order_request);
	if (api_resp) free_api_response(api_resp);
	if (clt) free(clt);

	return ;
}

void amend_order()
{
	Client *clt = new("<API_KEY>", "<API_SECRET>");
	if (!clt)
		return ;

	// allocate memory and initialize order request
	AmendOrderRequest *amend_order_request = init_amend_order_request();
	if (!amend_order_request)
		return ;

	amend_order_request->category = "linear";
	amend_order_request->symbol = "BTCUSDT";
	
	APIResponse *api_resp = post_amend_order(clt, amend_order_request);
	if (!api_resp)
		goto end;

	if (api_resp->ret_code != 0)
	{
		printf("response code: %d with message: %s\n", api_resp->ret_code, api_resp->ret_msg);
		goto end;
	}

	if (!api_resp->result) {
		printf("RESULT IS NULL\n");
		goto end;
	}

	OrderResponse *resp = (OrderResponse *)api_resp->result;
	if (!resp)
		goto end;

	printf("ORDER ID: %s\n", resp->order_id);
	printf("ORDER LINK ID: %s\n", resp->order_link_id);
	
	if (resp->order_id) free(resp->order_id);
	if (resp->order_link_id) free(resp->order_link_id);

end:
	if (amend_order_request) free(amend_order_request);
	if (api_resp) free_api_response(api_resp);
	if (clt) free(clt);

	return ;
}

void place_order()
{
	Client *clt = new("<API_KEY>", "<API_SECRET>");
	if (!clt)
		return ;

	// allocate memory and initialize order request
	OrderRequest *order_request = init_order_request();
	if (!order_request)
		return ;

	order_request->category = "spot";
	order_request->symbol = "BTCUSDT";
	order_request->side = "Buy";
	order_request->order_type = "Limit";
	order_request->qty = "0.2";
	order_request->price = "27000";
	
	APIResponse *api_resp = post_order(clt, order_request);
	if (!api_resp)
		goto end;

	if (api_resp->ret_code != 0)
	{
		printf("response code: %d with message: %s\n", api_resp->ret_code, api_resp->ret_msg);
		goto end;
	}

	if (!api_resp->result) {
		printf("RESULT IS NULL\n");
		goto end;
	}

	OrderResponse *resp = (OrderResponse *)api_resp->result;
	if (!resp)
		goto end;

	printf("ORDER ID: %s\n", resp->order_id);
	printf("ORDER LINK ID: %s\n", resp->order_link_id);
	
	if (resp->order_id) free(resp->order_id);
	if (resp->order_link_id) free(resp->order_link_id);

end:
	if (order_request) free(order_request);
	if (api_resp) free_api_response(api_resp);
	if (clt) free(clt);

	return ;
}

// retrieve ticker example
void retrieve_ticker()
{
	TickersQueryParams *query = build_ticker_query("spot", "TONUSDT", "TON", "");
	if (!query) {
		printf("QUERY IS NULL\n");
		return;
	}

	APIResponse *api_resp = get_ticker(query);
	if (!api_resp) {
		printf("RESP IS NULL\n");
		goto end;
	}

	if (api_resp->ret_code != 0)
	{
		printf("response code: %d with message: %s\n", api_resp->ret_code, api_resp->ret_msg);
		goto end;
	}

	if (!api_resp->result) {
		printf("RESULT IS NULL\n");
		goto end;
	}

	TickerResponse *resp = (TickerResponse *)api_resp->result;
	struct Node *cur = resp->list;
	if (cur == NULL) {
		printf("CUR IS NULL\n");
		goto end;
	}

	while (cur != NULL)
	{
		Ticker *ticker = (Ticker *)cur->val;
		print_ticker(ticker);
		cur = cur->next;
	}

	// free response
	Node *tmp = NULL;
	cur = resp->list;
	while (cur != NULL)
	{
		free_ticker(cur->val);
		tmp = cur;
		cur = cur->next;
		free(tmp);
	}

	if (resp->category) free(resp->category);

end:
	free_api_response(api_resp);
	if (query)
	{
		free_list(&query->_queries, free_query_element_cb);
		free(query);
	}
}

// retrieve price kline example
void retrieve_price_kline()
{
	KlineQueryParams *query = build_kline_query("linear", "BTCUSDT", "60", "", "", "100");
	if (!query)
		return;

	APIResponse *api_resp = get_mark_price_kline(query);
	if (!api_resp)
		goto end;

	if (api_resp->ret_code != 0)
	{
		printf("response code: %d with message: %s\n", api_resp->ret_code, api_resp->ret_msg);
		goto end;
	}

	if (!api_resp->result) {
		printf("RESULT IS NULL\n");
		goto end;
	}

	KlineResponse *resp = (KlineResponse *)api_resp->result;

	struct Node *cur = resp->list;
	if (cur == NULL)
		goto end;

	int i = 0;
	while (cur != NULL)
	{
		Kline *kline = (Kline *)cur->val;
		printf("-------------------> %d\n", i);
		print_price_kline(kline);
		cur = cur->next;
		i++;
	}

	// free response
	Node *tmp = NULL;
	cur = resp->list;
	while (cur != NULL)
	{
		free_price_kline(cur->val);
		tmp = cur;
		cur = cur->next;
		free(tmp);
	}

	if (resp->category) free(resp->category);
	if (resp->symbol) free(resp->symbol);
end:
	free_api_response(api_resp);
	if (query)
	{
		free_list(&query->_queries, free_query_element_cb);
		free(query);
	}
}

// retrieve kline example
void retrieve_kline()
{
	KlineQueryParams *query = build_kline_query("spot", "TONUSDT", "60", "", "", "100");
	if (!query)
		return;

	APIResponse *api_resp = get_kline(query);
	if (!api_resp)
		goto end;

	if (api_resp->ret_code != 0)
	{
		printf("response code: %d with message: %s\n", api_resp->ret_code, api_resp->ret_msg);
		goto end;
	}

	KlineResponse *resp = (KlineResponse *)api_resp->result;

	struct Node *cur = resp->list;
	if (cur == NULL)
		goto end;

	int i = 0;
	while (cur != NULL)
	{
		Kline *kline = (Kline *)cur->val;
		printf("-------------------> %d\n", i);
		print_kline(kline);
		cur = cur->next;
		i++;
	}

	Node *tmp = NULL;
	cur = resp->list;
	while (cur != NULL)
	{
		free_kline(cur->val);
		tmp = cur;
		cur = cur->next;
		free(tmp);
	}

	// free response
	if (resp->category) free(resp->category);
	if (resp->symbol) free(resp->symbol);
	
end:
	free_api_response(api_resp);
	if (query)
	{
		free_list(&query->_queries, free_query_element_cb);
		free(query);
	}
}

// retrieve order book example
void retrieve_order_book()
{
	OrderBookQuery *query = build_order_book_query("spot", "TONUSDT", "50");
	if (!query)
		return;

	APIResponse *api_resp = get_order_book(query);
	if (!api_resp) {
		printf("RESP IS NULL\n");
		goto end;
	}

	if (api_resp->ret_code != 0)
	{
		printf("response code: %d with message: %s\n", api_resp->ret_code, api_resp->ret_msg);
		goto end;
	}

	if (!api_resp->result) {
		printf("RESULT IS NULL\n");
		goto end;
	}

	OrderBookResponse *resp = (OrderBookResponse *)api_resp->result;

	struct Node *cur = resp->asks;
	if (cur == NULL)
		goto end;

	printf("ASKS: \n");
	int i = 0;
	while (cur != NULL)
	{
		OrderB *order = (OrderB *)cur->val;
		printf("-------------------> %d\n", i);
		print_order_book(order);
		cur = cur->next;
		i++;
	}

	printf("BIDS: \n");
	i = 0;
	cur = resp->bids;
	while (cur != NULL)
	{
		OrderB *order = (OrderB *)cur->val;
		printf("-------------------> %d\n", i);
		print_order_book(order);
		cur = cur->next;
		i++;
	}

	// free response
	Node *tmp = NULL;
	cur = resp->asks;
	while (cur != NULL)
	{
		free_orderb(cur->val);
		tmp = cur;
		cur = cur->next;
		free(tmp);
	}

	cur = resp->bids;
	while (cur != NULL)
	{
		free_orderb(cur->val);
		tmp = cur;
		cur = cur->next;
		free(tmp);
	}

	if (resp->symbol) free(resp->symbol);
	if (resp->ts) free(resp->ts);
	if (resp->update_id) free(resp->update_id);

end:
	free_api_response(api_resp);
	if (query)
	{
		free_list(&query->_queries, free_query_element_cb);
		free(query);
	}
}

// retrieve time server example
void retrieve_time_server()
{
	APIResponse *api_resp = get_time_server();
	if (!api_resp)
		return;

	if (api_resp->ret_code != 0)
	{
		printf("response code: %d with message: %s\n", api_resp->ret_code, api_resp->ret_msg);
		goto end;
	}

	TimeServerResponse *resp = (TimeServerResponse *)api_resp->result;
	if (!resp)
		goto end;
	

	printf("time_second: %s\n", resp->time_second);
	printf("time_nano: %s\n", resp->time_nano);

	if (resp->time_second) free(resp->time_second);
	if (resp->time_nano) free(resp->time_nano);
end:
	free_api_response(api_resp);
}

void print_ticker(Ticker *ticker)
{
	// symbol
	printf("symbol %s\n", ticker->symbol);
	// bid1Price
	printf("bid1_price %s\n", ticker->bid1_price);
	// bid1Size
	printf("bid1_size %s\n", ticker->bid1_size);
	// ask1Price
	printf("ask1_price %s\n", ticker->ask1_price);
	// ask1Size
	printf("ask1_size %s\n", ticker->ask1_size);
	// lastPrice
	printf("last_price %s\n", ticker->last_price);
	// prevPrice24h
	printf("prev_price_24h %s\n", ticker->prev_price_24h);
	// price24hPcnt
	printf("price_24h_pcnt %s\n", ticker->price_24h_pcnt);
	// highPrice24h
	printf("high_price_24h %s\n", ticker->high_price_24h);
	// lowPrice24h
	printf("low_price_24h %s\n", ticker->low_price_24h);
	// turnover24h
	printf("turn_over_24h %s\n", ticker->turn_over_24h);
	// volume24h
	printf("volume_24 %s\n", ticker->volume_24);
	// usdIndexPrice
	printf("usd_index_price %s\n", ticker->usd_index_price);
}

void print_kline(Kline *kline)
{
	printf("start time: %s\n", kline->start_time);
	printf("open price: %s\n", kline->open_price);
	printf("high price: %s\n", kline->high_price);
	printf("low price: %s\n", kline->low_price);
	printf("close price: %s\n", kline->close_price);
	printf("volume: %s\n", kline->volume);
	printf("turnover: %s\n", kline->turnover);
}

void print_price_kline(Kline *kline)
{
	printf("start time: %s\n", kline->start_time);
	printf("open price: %s\n", kline->open_price);
	printf("high price: %s\n", kline->high_price);
	printf("low price: %s\n", kline->low_price);
	printf("close price: %s\n", kline->close_price);
}

void print_order_book(OrderB *order)
{
	printf("price: %s\n", order->price);
	printf("size: %s\n", order->size);
}

void print_open_order(OpenOrder *order)
{
	printf("order_id: %s\n", order->order_id);
	printf("block_trade_id: %s\n", order->block_trade_id);
	printf("symbol: %s\n", order->symbol);
	printf("price: %s\n", order->price);
	printf("qty: %s\n", order->qty);
	printf("side: %s\n", order->side);
	printf("is_leverage: %s\n", order->is_leverage);
	printf("position_idx: %d\n", order->position_idx);
	printf("order_status: %s\n", order->order_status);
	printf("cancel_type: %s\n", order->cancel_type);
	printf("reject_reason: %s\n", order->reject_reason);
	printf("avg_price: %s\n", order->avg_price);
	printf("leaves_qty: %s\n", order->leaves_qty);
	printf("leaves_value: %s\n", order->leaves_value);
	printf("cum_exec_qty: %s\n", order->cum_exec_qty);
	printf("cum_exec_value: %s\n", order->cum_exec_value);
	printf("cum_exec_fee: %s\n", order->cum_exec_fee);
	printf("time_in_force: %s\n", order->time_in_force);
	printf("order_type: %s\n", order->order_type);
	printf("stop_order_type: %s\n", order->stop_order_type);
	printf("order_lv: %s\n", order->order_lv);
	printf("trigger_price: %s\n", order->trigger_price);
	printf("take_profit: %s\n", order->take_profit);
	printf("stop_loss: %s\n", order->stop_loss);
	printf("tpsl_mode: %s\n", order->tpsl_mode);
	printf("tp_limit_price: %s\n", order->tp_limit_price);
	printf("sl_limit_price: %s\n", order->sl_limit_price);
	printf("tp_trigger_by: %s\n", order->tp_trigger_by);
	printf("sl_trigger_by: %s\n", order->sl_trigger_by);
	printf("trigger_direction: %d\n", order->trigger_direction);
	printf("trigger_by: %s\n", order->trigger_by);
	printf("last_price_on_created: %s\n", order->last_price_on_created);
	printf("reduce_only: %s\n", bool_to_string(order->reduce_only));
	printf("close_on_trigger: %s\n", bool_to_string(order->close_on_trigger));
	printf("place_type: %s\n", order->place_type);
	printf("smp_type: %s\n", order->smp_type);
	printf("smp_group: %d\n", order->smp_group);
	printf("smp_order_id: %s\n", order->smp_order_id);
	printf("created_time: %s\n", order->created_time);
	printf("updated_time: %s\n", order->updated_time);
}
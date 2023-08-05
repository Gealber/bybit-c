#include <stdio.h>
#include <stdlib.h>
#include "bybit.h"
#include "common.h"
#include "request.h"
#include "response.h"

void retrieve_ticker();
void retrieve_time_server();
void retrieve_kline();
void retrieve_price_kline();
void retrieve_order_book();
void print_ticker(Ticker *ticker);
void print_kline(Kline *kline);
void print_price_kline(Kline *kline);
void print_order_book(OrderB *order);

int main(int argc, char *argv[])
{
	/*Uncomment these lines to run examples*/
	// retrieve_ticker();
	// retrieve_time_server();
	// retrieve_kline();
	// retrieve_price_kline();
	retrieve_order_book();
	return 0;
}

// retrieve price kline example
void retrieve_price_kline()
{
	KlineQueryParams *query = build_kline_query("linear", "BTCUSDT", "60", "", "", "100");
	if (!query)
		return;

	KlineResponse *resp = get_mark_price_kline(query);
	if (!resp)
		goto end;

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
	free(resp->category);
	free(resp->symbol);
	Node *tmp = NULL;
	cur = resp->list;
	while (cur != NULL)
	{
		free_price_kline(cur->val);
		tmp = cur;
		cur = cur->next;
		free(tmp);
	}

end:
	if (resp)
		free(resp);
	if (query)
	{
		free_list(&query->_queries, free_query_element_callback);
		free(query);
	}
}

// retrieve kline example
void retrieve_kline()
{
	KlineQueryParams *query = build_kline_query("spot", "TONUSDT", "60", "", "", "100");
	if (!query)
		return;

	KlineResponse *resp = get_kline(query);
	if (!resp)
		goto end;

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

	// free response
	free(resp->category);
	free(resp->symbol);
	Node *tmp = NULL;
	cur = resp->list;
	while (cur != NULL)
	{
		free_kline(cur->val);
		tmp = cur;
		cur = cur->next;
		free(tmp);
	}

end:
	if (resp)
		free(resp);
	if (query)
	{
		free_list(&query->_queries, free_query_element_callback);
		free(query);
	}
}

// retrieve order book example
void retrieve_order_book()
{
	OrderBookQuery *query = build_order_book_query("spot", "TONUSDT", "50");
	if (!query)
		return;

	OrderBookResponse *resp = get_order_book(query);
	if (!resp)
		goto end;

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
	free(resp->symbol);
	Node *tmp = NULL;
	cur = resp->asks;
	while (cur != NULL)
	{
		free_order(cur->val);
		tmp = cur;
		cur = cur->next;
		free(tmp);
	}

	cur = resp->bids;
	while (cur != NULL)
	{
		free_order(cur->val);
		tmp = cur;
		cur = cur->next;
		free(tmp);
	}

end:
	if (resp)
		free(resp);
	if (query)
	{
		free_list(&query->_queries, free_query_element_callback);
		free(query);
	}
}
// retrieve time server example
void retrieve_time_server()
{
	TimeServerResponse *resp = get_time_server();
	if (!resp)
		return;

	printf("time_second: %s\n", resp->time_second);
	printf("time_nano: %s\n", resp->time_nano);
	free(resp->time_second);
	free(resp->time_nano);
	free(resp);
}

// retrieve ticker example
void retrieve_ticker()
{
	TickersQueryParams *query = build_ticker_query("spot", "TONUSDT", "TON", "lala");
	if (!query)
		return;

	TickerResponse *resp = get_ticker(query);
	if (!resp)
		goto end;

	struct Node *cur = resp->list;
	if (cur == NULL)
		goto end;

	while (cur != NULL)
	{
		Ticker *ticker = (Ticker *)cur->val;
		print_ticker(ticker);
		cur = cur->next;
	}

	// free response
	free(resp->category);
	Node *tmp = NULL;
	cur = resp->list;
	while (cur != NULL)
	{
		free_ticker(cur->val);
		tmp = cur;
		cur = cur->next;
		free(tmp);
	}

end:
	if (resp)
		free(resp);
	if (query)
	{
		free_list(&query->_queries, free_query_element_callback);
		free(query);
	}
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

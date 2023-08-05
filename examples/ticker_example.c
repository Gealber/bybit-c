#include <stdio.h>
#include <stdlib.h>
#include "src/bybit.h"
#include "src/common.h"
#include "src/request.h"
#include "src/response.h"

void retrieve_ticker();
void print_ticker(Ticker *ticker);

int main(int argc, char *argv[])
{
	/*Uncomment these lines to run examples*/
	retrieve_ticker();
	return 0;
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

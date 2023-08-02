#include <stdio.h>
#include <stdlib.h>
#include "bybit.h"
#include "common.h"
#include "request.h"
#include "response.h"

void retrieve_ticker();
void retrieve_time_server();
void print_ticker(Ticker *ticker);

int main(int argc, char *argv[])
{
  // retrieve_ticker();
  retrieve_time_server();
  return 0;
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
}

// retrieve ticker example
void retrieve_ticker()
{
  TickersQueryParams *query = malloc(sizeof(TickersQueryParams));

  query->category = "spot";
  query->symbol = "TONUSDT";
  query->base_coin = "TON";
  query->exp_date = 0;

  TickerResponse *resp = get_ticker(query);
  if (!resp)
    goto end;

  struct Node *cur = resp->list;
  if (cur == NULL)
    printf("CUR IS NULL\n");

  while (cur != NULL) {
    Ticker *ticker = (Ticker *)cur->val;
    print_ticker(ticker);
    cur = cur->next;
  }

  // free response
  free(resp->category);
  Node *tmp = NULL;
  cur = resp->list;
  while (cur != NULL) {
    free_ticker(cur->val);
    tmp = cur;
    cur = cur->next;
    free(tmp);
  }
  free(resp);

end:
  free(query);
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

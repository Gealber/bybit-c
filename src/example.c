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
void print_ticker(Ticker *ticker);
void print_kline(Kline *kline);
void print_price_kline(Kline *kline);

int main(int argc, char *argv[])
{
  // retrieve_ticker();
  // retrieve_time_server();
  // retrieve_kline();
  retrieve_price_kline();
  return 0;
}

// retrieve price kline example
void retrieve_price_kline()
{
  KlineQueryParams *query = malloc(sizeof(KlineQueryParams));

  query->category = "linear";
  query->symbol = "BTCUSDT";
  query->interval = "60"; // klines for 1 hour interval
  query->start = "";
  query->end = "";
  query->limit = "100";

  KlineResponse *resp = get_mark_price_kline(query);
  if (!resp)
    goto end;

  struct Node *cur = resp->list;
  if (cur == NULL)
    printf("CUR IS NULL\n");

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
  free(resp);

end:
  free(query);
}
// retrieve kline example
void retrieve_kline()
{
  KlineQueryParams *query = malloc(sizeof(KlineQueryParams));

  query->category = "spot";
  query->symbol = "TONUSDT";
  query->interval = "60"; // klines for 1 hour interval
  query->start = "";
  query->end = "";
  query->limit = "100";

  KlineResponse *resp = get_kline(query);
  if (!resp)
    goto end;

  struct Node *cur = resp->list;
  if (cur == NULL)
    printf("CUR IS NULL\n");

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
  free(resp);

end:
  free(query);
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


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

TickerResponse *parse_ticker_response(char *text);
TimeServerResponse *parse_ts_response(char *text);
KlineResponse *parse_kline_response(char *text);

#endif
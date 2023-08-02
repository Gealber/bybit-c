
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

TickerResponse *parse_ticker_response(char *text);
TimeServerResponse *parse_ts_response(char *text);

#endif
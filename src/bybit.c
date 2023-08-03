#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "bybit.h"
#include "const.h"
#include "common.h"
#include "request.h"
#include "response.h"
#include "cJSON.h"

// New creates a new Client instance
// which will contain information about API KEY and API SECRET
Client *new(char *api_key, char *api_secret)
{
    Client *clt = malloc(sizeof(Client));
    if (clt == NULL)     
        return NULL;

    clt->api_key = api_key;
    clt->api_secret = api_secret;

    return clt;
}

CURL *http_client()
{
    CURL *hnd;

    hnd = curl_easy_init();

    curl_easy_setopt(hnd, CURLOPT_BUFFERSIZE, 1 << 19);
    curl_easy_setopt(hnd, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(hnd, CURLOPT_MAXREDIRS, 50L);
    curl_easy_setopt(hnd, CURLOPT_USERAGENT, USER_AGENT);
    curl_easy_setopt(hnd, CURLOPT_HTTP_VERSION, (long)CURL_HTTP_VERSION_2TLS);
    curl_easy_setopt(hnd, CURLOPT_FTP_SKIP_PASV_IP, 1L);
    curl_easy_setopt(hnd, CURLOPT_TCP_KEEPALIVE, 1L);
    curl_easy_setopt(hnd, CURLOPT_ACCEPT_ENCODING, "");
    // // VERBOSE MODE
    // curl_easy_setopt(hnd, CURLOPT_VERBOSE, 1L);

    return hnd;
}

TickerResponse *get_ticker(TickersQueryParams *query)
{
    // check for query
    if (query == NULL)
        return NULL;

    // category it's a required parameter
    if (query->category == NULL && (strlen(query->category) == 0))
        return NULL;
    
    char url[128];
    sprintf(url, "%s%s", DOMAIN_MAINNET, TICKERS_PATH);
    CURL *hnd_url = curl_url();
    CURL *hnd = http_client();
    curl_url_set(hnd_url, CURLUPART_URL, url, 0);

    // build queries
    add_query(hnd_url, "category", query->category);
    add_query(hnd_url, "baseCoin", query->base_coin);
    add_query(hnd_url, "symbol", query->symbol);
    add_query(hnd_url, "expDate", query->exp_date);

    // setting url
    curl_easy_setopt(hnd, CURLOPT_CURLU, hnd_url);

    // setting memory to store response
    ResponseJSON mem = {.chunk = malloc(0), .size = 0};

    curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, write_json);
    curl_easy_setopt(hnd, CURLOPT_WRITEDATA, (void *)&mem);

    // make http request asking for the tickers
    CURLcode ret = curl_easy_perform(hnd);
    if (ret != CURLE_OK)
        printf("ERROR CODE: %d\n", ret);

    TickerResponse *resp = parse_ticker_response(mem.chunk);

    curl_easy_cleanup(hnd);
    hnd = NULL;
    curl_url_cleanup(hnd_url);
    free(mem.chunk);

    return resp;
}

// get_time_server retrieve current time in bybit server
TimeServerResponse *get_time_server()
{
    char url[38];
    sprintf(url, "%s%s", DOMAIN_MAINNET, SERVER_TIME_PATH);
    CURL *hnd_url = curl_url();
    CURL *hnd = http_client();
    curl_url_set(hnd_url, CURLUPART_URL, url, 0);

    // setting url
    curl_easy_setopt(hnd, CURLOPT_CURLU, hnd_url);

    // setting memory to store response
    ResponseJSON mem = {.chunk = malloc(0), .size = 0};

    curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, write_json);
    curl_easy_setopt(hnd, CURLOPT_WRITEDATA, (void *)&mem);

    // make http request asking for the tickers
    CURLcode ret = curl_easy_perform(hnd);
    if (ret != CURLE_OK)
        printf("ERROR CODE: %d\n", ret);

    TimeServerResponse *resp = parse_ts_response(mem.chunk);

    curl_easy_cleanup(hnd);
    hnd = NULL;
    curl_url_cleanup(hnd_url);
    free(mem.chunk);

    return resp;
}

// query for historycal klines
KlineResponse *get_kline(KlineQueryParams *query)
{
    char url[128];
    sprintf(url, "%s%s", DOMAIN_MAINNET, KLINE_PATH);
    CURL *hnd_url = curl_url();
    CURL *hnd = http_client();
    curl_url_set(hnd_url, CURLUPART_URL, url, 0);

    // build queries
    add_query(hnd_url, "category", query->category);
    add_query(hnd_url, "symbol", query->symbol);
    add_query(hnd_url, "interval", query->interval);
    add_query(hnd_url, "start", query->start);
    add_query(hnd_url, "end", query->end);
    add_query(hnd_url, "limit", query->limit);

    // setting url
    curl_easy_setopt(hnd, CURLOPT_CURLU, hnd_url);

    // setting memory to store response
    ResponseJSON mem = {.chunk = malloc(0), .size = 0};

    curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, write_json);
    curl_easy_setopt(hnd, CURLOPT_WRITEDATA, (void *)&mem);

    // make http request asking for the tickers
    CURLcode ret = curl_easy_perform(hnd);
    if (ret != CURLE_OK)
        printf("ERROR CODE: %d\n", ret);

    KlineResponse *resp = parse_kline_response(mem.chunk);

    curl_easy_cleanup(hnd);
    hnd = NULL;
    curl_url_cleanup(hnd_url);
    free(mem.chunk);

    return resp;
}

KlineResponse *get_mark_price_kline(KlineQueryParams *query)
{
    char url[256];
    sprintf(url, "%s%s", DOMAIN_MAINNET, MARK_PRICE_KLINE_PATH);
    CURL *hnd_url = curl_url();
    CURL *hnd = http_client();
    curl_url_set(hnd_url, CURLUPART_URL, url, 0);

    // build queries
    add_query(hnd_url, "category", query->category);
    add_query(hnd_url, "symbol", query->symbol);
    add_query(hnd_url, "interval", query->interval);
    add_query(hnd_url, "start", query->start);
    add_query(hnd_url, "end", query->end);
    add_query(hnd_url, "limit", query->limit);

    // setting url
    curl_easy_setopt(hnd, CURLOPT_CURLU, hnd_url);

    // setting memory to store response
    ResponseJSON mem = {.chunk = malloc(0), .size = 0};

    curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, write_json);
    curl_easy_setopt(hnd, CURLOPT_WRITEDATA, (void *)&mem);

    // make http request asking for the tickers
    CURLcode ret = curl_easy_perform(hnd);
    if (ret != CURLE_OK)
        printf("ERROR CODE: %d\n", ret);

    KlineResponse *resp = parse_price_kline_response(mem.chunk);

    curl_easy_cleanup(hnd);
    hnd = NULL;
    curl_url_cleanup(hnd_url);
    free(mem.chunk);

    return resp;
}
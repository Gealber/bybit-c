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

    // not absolutely necessary, the default size 16KB is good enough at the moment
    // curl_easy_setopt(hnd, CURLOPT_BUFFERSIZE, 1 << 20);
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

// perform_get perform a GET http request storing its response
// in the mem passed
CURLcode perform_get(char *url, Node *queries, ResponseJSON *mem)
{
    CURL *hnd_url = curl_url();
    CURL *hnd = http_client();
    curl_url_set(hnd_url, CURLUPART_URL, url, 0);

    // build queries
    struct Node *cur = queries;
    while (cur != NULL)
    {
        _queryElement *query = (_queryElement *)cur->val;
        if (!query)
            break;
        add_query(hnd_url, query->key, query->val);
        cur = cur->next;
    }

    // setting url
    curl_easy_setopt(hnd, CURLOPT_CURLU, hnd_url);

    curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, write_json);
    curl_easy_setopt(hnd, CURLOPT_WRITEDATA, (void *)mem);

    // make http request asking for the tickers
    CURLcode ret = curl_easy_perform(hnd);

    // cleaning handler
    curl_easy_cleanup(hnd);
    hnd = NULL;
    curl_url_cleanup(hnd_url);

    return ret;
}

// perform_post perform a POST http request storing its response
// in the mem passed. Authenticated method will need to receive the client instance.
CURLcode perform_post(Client *clt, char *url, char *body, ResponseJSON *mem)
{
    CURL *hnd_url = curl_url();
    CURL *hnd = http_client();
    curl_url_set(hnd_url, CURLUPART_URL, url, CURLU_URLENCODE);

    // generating signature
    int64_t ts = timestamp();
    char *hex_signature = gen_signature(clt->api_key, clt->api_secret, ts, body);

    // headers
    char api_key_header[36];
    sprintf(api_key_header, "X-BAPI-API-KEY: %s", clt->api_key);
    char ts_header[48];
    sprintf(ts_header, "X-BAPI-TIMESTAMP: %ld", ts);
    char sign_header[13 + strlen(hex_signature) + 1];
    // printf("HEX SIGNATURE: %s SIZE: %ld\n", hex_signature, strlen(hex_signature));
    sprintf(sign_header, "X-BAPI-SIGN: %s", hex_signature);

    struct curl_slist *headers;
    headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, api_key_header);
    headers = curl_slist_append(headers, ts_header);
    headers = curl_slist_append(headers, sign_header);
    headers = curl_slist_append(headers, "X-BAPI-RECV-WINDOW: 6000");

    // setting url
    curl_easy_setopt(hnd, CURLOPT_CURLU, hnd_url);
    // setting memory to write response
    curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, write_json);
    curl_easy_setopt(hnd, CURLOPT_WRITEDATA, (void *)mem);
    // setting header application/json
    curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headers);
    // specifying json body
    curl_easy_setopt(hnd, CURLOPT_POSTFIELDS, body);
    curl_easy_setopt(hnd, CURLOPT_POSTFIELDSIZE_LARGE, (curl_off_t)strlen(body));

    // make http request asking for the tickers
    CURLcode ret = curl_easy_perform(hnd);

    // cleaning handler
    curl_easy_cleanup(hnd);
    hnd = NULL;
    // cleaning list of headers
    curl_slist_free_all(headers);
    headers = NULL;
    // cleaning url builder
    curl_url_cleanup(hnd_url);
    hnd_url = NULL;
    // we need to free signature as well
    free(hex_signature);

    return ret;
}

// get_ticker retrieve tickers from a given pair
APIResponse *get_ticker(TickersQueryParams *query)
{
    // check for query
    if (query == NULL)
        return NULL;

    // category it's a required parameter
    if (query->category == NULL && (strlen(query->category) == 0))
        return NULL;

    char url[128];
    sprintf(url, "%s%s", DOMAIN_MAINNET, TICKERS_PATH);

    // setting memory to store response
    ResponseJSON mem = {.chunk = malloc(0), .size = 0};

    CURLcode ret = perform_get(url, query->_queries, &mem);
    if (ret != CURLE_OK)
        return NULL;

    APIResponse *resp = parse_api_response(mem.chunk, parse_ticker_response_cb);

    free(mem.chunk);

    return resp;
}

// get_time_server retrieve current time in bybit server
APIResponse *get_time_server()
{
    char url[38];
    sprintf(url, "%s%s", DOMAIN_MAINNET, SERVER_TIME_PATH);

    // setting memory to store response
    ResponseJSON mem = {.chunk = malloc(0), .size = 0};

    CURLcode ret = perform_get(url, NULL, &mem);
    if (ret != CURLE_OK)
        return NULL;

    APIResponse *resp = NULL;
    if (mem.size != 0)
        resp = parse_api_response(mem.chunk, parse_ts_response_cb);
    else
        printf("NO RESPONSE FROM API\n");

    free(mem.chunk);

    return resp;
}

// query for historycal klines
APIResponse *get_kline(KlineQueryParams *query)
{
    char url[128];
    sprintf(url, "%s%s", DOMAIN_MAINNET, KLINE_PATH);

    // setting memory to store response
    ResponseJSON mem = {.chunk = malloc(0), .size = 0};

    CURLcode ret = perform_get(url, query->_queries, &mem);
    if (ret != CURLE_OK)
        return NULL;

    APIResponse *resp = parse_api_response(mem.chunk, parse_kline_response_cb);

    free(mem.chunk);

    return resp;
}

APIResponse *get_mark_price_kline(KlineQueryParams *query)
{
    char url[256];
    sprintf(url, "%s%s", DOMAIN_MAINNET, MARK_PRICE_KLINE_PATH);

    // setting memory to store response
    ResponseJSON mem = {.chunk = malloc(0), .size = 0};

    CURLcode ret = perform_get(url, query->_queries, &mem);
    if (ret != CURLE_OK)
        return NULL;

    APIResponse *resp = parse_api_response(mem.chunk, parse_price_kline_response_cb);

    free(mem.chunk);

    return resp;
}

// get_order_book retrieve information about order book
APIResponse *get_order_book(OrderBookQuery *query)
{
    char url[128];
    sprintf(url, "%s%s", DOMAIN_MAINNET, ORDERBOOK_PATH);

    // setting memory to store response
    ResponseJSON mem = {.chunk = malloc(0), .size = 0};

    CURLcode ret = perform_get(url, query->_queries, &mem);
    if (ret != CURLE_OK)
        return NULL;

    APIResponse *resp = parse_api_response(mem.chunk, parse_order_book_response_cb);

    free(mem.chunk);

    return resp;
}

APIResponse *post_order(Client *clt, OrderRequest *order_request)
{
    char url[46];
    sprintf(url, "%s%s", DOMAIN_TESTNET, PLACE_ORDER_PATH);

    // setting memory to store response
    ResponseJSON mem = {.chunk = malloc(0), .size = 0};

    char *body = order_request_tojson(order_request);

    CURLcode ret = perform_post(clt, url, body, &mem);
    if (ret != CURLE_OK)
        return NULL;

    APIResponse *resp = NULL;
    if (mem.size != 0)
        resp = parse_api_response(mem.chunk, parse_order_response_cb);

    if (mem.chunk)
        free(mem.chunk);
    free(body);

    return resp;
}
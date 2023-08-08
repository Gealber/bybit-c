#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <openssl/evp.h>
#include <openssl/hmac.h>
#include "bybit.h"
#include "common.h"
#include "request.h"
#include "response.h"
#include "cJSON.h"

// ENDPOINTS AND DOMAIN
const char *DOMAIN_TESTNET = "https://api-testnet.bybit.com/v5";
const char *DOMAIN_MAINNET = "https://api.bybit.com/v5";
const char *ANNOUNCEMENT_PATH = "/announcements/index";
const char *TICKERS_PATH = "/market/tickers";
const char *SERVER_TIME_PATH = "/market/time";
const char *KLINE_PATH = "/market/kline";
const char *MARK_PRICE_KLINE_PATH = "/market/mark-price-kline";
const char *ORDERBOOK_PATH = "/market/orderbook";
const char *PLACE_ORDER_PATH = "/order/create";
const char *AMEND_ORDER_PATH = "/order/amend";
const char *CANCEL_ORDER_PATH = "/order/cancel";
const char *OPEN_ORDER_REALTIME_PATH = "/order/realtime";

// USER AGENT
const char *USER_AGENT = "bybit-c";

const int DEFAULT_RECV_WINDOW = 7000;

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
    // VERBOSE MODE
    curl_easy_setopt(hnd, CURLOPT_VERBOSE, 1L);

    return hnd;
}

struct curl_slist *auth_headers(Client *clt, char *params)
{
    // generating signature
    int64_t ts = timestamp();
    char *hex_signature = gen_signature(clt->api_key, clt->api_secret, ts, params);

    // headers
    char api_key_header[36];
    sprintf(api_key_header, "X-BAPI-API-KEY: %s", clt->api_key);
    char ts_header[48];
    sprintf(ts_header, "X-BAPI-TIMESTAMP: %ld", ts);
    char sign_header[13 + strlen(hex_signature) + 1];
    sprintf(sign_header, "X-BAPI-SIGN: %s", hex_signature);
    char recv_window[32];
    sprintf(recv_window, "X-BAPI-RECV-WINDOW: %d", DEFAULT_RECV_WINDOW);

    struct curl_slist *headers;
    headers = NULL;
    headers = curl_slist_append(headers, api_key_header);
    headers = curl_slist_append(headers, ts_header);
    headers = curl_slist_append(headers, sign_header);
    headers = curl_slist_append(headers, recv_window);

    free(hex_signature);
    return headers;
}

// perform_get perform a GET http request storing its response
// in the mem passed
CURLcode perform_get(Client *clt, char *url, Node *queries, ResponseJSON *mem)
{
    CURL *hnd_url = curl_url();
    CURL *hnd = http_client();
    curl_url_set(hnd_url, CURLUPART_URL, url, 0);

    // build queries
    size_t params_size = 1;
    int cur_pos = 0;
    char *params = calloc(1, sizeof(char));
    params[0] = '\0';
    struct Node *cur = queries;
    while (cur != NULL)
    {
        _queryElement *query = (_queryElement *)cur->val;
        if (!query)
            break;
        add_query(hnd_url, query->key, query->val);
        cur = cur->next;
        params_size += strlen(query->key) + strlen(query->val) + 2; // 2 for = and for &
        params = realloc(params, params_size*sizeof(char));
        sprintf(&params[cur_pos], "%s=%s&", query->key, query->val);
        cur_pos = params_size-1;
    }

    if (params)
        params[strlen(params)-1] = '\0';

    // setting auth headers in case is needed
    struct curl_slist *headers;
    headers = NULL;
    if (clt) {
        headers = auth_headers(clt, params);
        curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headers);
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
    // cleaning list of headers
    if (headers) {
        curl_slist_free_all(headers);
        headers = NULL;
    }
    // clearing url
    curl_url_cleanup(hnd_url);
    free(params);

    return ret;
}

// perform_post perform a POST http request storing its response
// in the mem passed. Authenticated method will need to receive the client instance.
CURLcode perform_post(Client *clt, char *url, char *body, ResponseJSON *mem)
{
    CURL *hnd_url = curl_url();
    CURL *hnd = http_client();
    curl_url_set(hnd_url, CURLUPART_URL, url, CURLU_URLENCODE);

    // auth headers
    struct curl_slist *headers;
    headers = auth_headers(clt, body);
    headers = curl_slist_append(headers, "Content-Type: application/json");

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

    CURLcode ret = perform_get(NULL, url, query->_queries, &mem);
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

    CURLcode ret = perform_get(NULL, url, NULL, &mem);
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

    CURLcode ret = perform_get(NULL, url, query->_queries, &mem);
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

    CURLcode ret = perform_get(NULL, url, query->_queries, &mem);
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

    CURLcode ret = perform_get(NULL, url, query->_queries, &mem);
    if (ret != CURLE_OK)
        return NULL;

    APIResponse *resp = parse_api_response(mem.chunk, parse_order_book_response_cb);

    free(mem.chunk);

    return resp;
}

APIResponse *get_open_orders(Client *clt, OpenOrdersQuery *query)
{
    char url[128];
    sprintf(url, "%s%s", DOMAIN_TESTNET, OPEN_ORDER_REALTIME_PATH);

    // setting memory to store response
    ResponseJSON mem = {.chunk = malloc(0), .size = 0};

    CURLcode ret = perform_get(clt, url, query->_queries, &mem);
    if (ret != CURLE_OK)
        return NULL;

    APIResponse *resp = parse_api_response(mem.chunk, parse_open_orders_response_cb);

    free(mem.chunk);

    return resp;
}

// post_order place an order in the exchange
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

// amend_order 
APIResponse *post_amend_order(Client *clt, AmendOrderRequest *amend_order_request)
{
    char url[46];
    sprintf(url, "%s%s", DOMAIN_TESTNET, AMEND_ORDER_PATH);

    // setting memory to store response
    ResponseJSON mem = {.chunk = malloc(0), .size = 0};

    char *body = amend_order_request_tojson(amend_order_request);

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

// cancel_order 
APIResponse *post_cancel_order(Client *clt, CancelOrderRequest *cancel_order_request)
{
    char url[46];
    sprintf(url, "%s%s", DOMAIN_TESTNET, CANCEL_ORDER_PATH);

    // setting memory to store response
    ResponseJSON mem = {.chunk = malloc(0), .size = 0};

    char *body = cancel_order_request_tojson(cancel_order_request);

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

/* UTILS */
char *gen_signature(const char *api_key, const char *api_secret, int64_t ts, char *params)
{
    // assuming a max of 13 digits for timestamp
    // 4 for recv windows
    size_t rule_size = 13 + strlen(api_key) + 4 + strlen(params) + 1;
    char rule[rule_size];
    sprintf(rule, "%ld%s%d%s", ts, api_key, DEFAULT_RECV_WINDOW, params);

    int keylen = strlen(api_secret);
    const unsigned char *data = (const unsigned char *)rule;
    int datalen = strlen((char *)data);
    unsigned char *result = NULL;
    unsigned int resultlen = -1;

    result = hmac_sha256((const void *)api_secret, keylen, data, datalen, result, &resultlen);

    char *hex = calloc(resultlen * 2 + 1, sizeof(char));
    char *ptr = &hex[0];

    int i = 0;
    for (i = 0; i < resultlen; i++)
    {
        ptr += sprintf(ptr, "%02X", result[i]);
    }

    return hex;
}

unsigned char *hmac_sha256(const void *key, int keylen, const unsigned char *data, int datalen, unsigned char *result, unsigned int *resultlen)
{
    return HMAC(EVP_sha256(), key, keylen, data, datalen, result, resultlen);
}

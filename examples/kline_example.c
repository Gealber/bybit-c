#include <stdio.h>
#include <stdlib.h>
#include "src/bybit.h"
#include "src/common.h"
#include "src/request.h"
#include "src/response.h"

void retrieve_kline();
void retrieve_price_kline();
void print_kline(Kline *kline);
void print_price_kline(Kline *kline);

int main(int argc, char *argv[])
{
    /*Uncomment these lines to run examples*/
    retrieve_kline();
    retrieve_price_kline();
    return 0;
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

    if (!api_resp->result)
    {
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

    if (resp->category)
        free(resp->category);
    if (resp->symbol)
        free(resp->symbol);
end:
    free_api_response(api_resp);
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
    if (resp->category)
        free(resp->category);
    if (resp->symbol)
        free(resp->symbol);

end:
    free_api_response(api_resp);
    if (query)
    {
        free_list(&query->_queries, free_query_element_callback);
        free(query);
    }
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
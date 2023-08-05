#include <stdio.h>
#include <stdlib.h>
#include "src/bybit.h"
#include "src/common.h"
#include "src/request.h"
#include "src/response.h"

void retrieve_order_book();
void print_order_book(OrderB *order);

int main(int argc, char *argv[])
{
    /*Uncomment these lines to run examples*/
    retrieve_order_book();
    return 0;
}

// retrieve order book example
void retrieve_order_book()
{
    OrderBookQuery *query = build_order_book_query("spot", "TONUSDT", "50");
    if (!query)
        return;

    APIResponse *api_resp = get_order_book(query);
    if (!api_resp)
    {
        printf("RESP IS NULL\n");
        goto end;
    }

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

    OrderBookResponse *resp = (OrderBookResponse *)api_resp->result;

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

    if (resp->symbol)
        free(resp->symbol);
    if (resp->ts)
        free(resp->ts);
    if (resp->update_id)
        free(resp->update_id);

end:
    free_api_response(api_resp);
    if (query)
    {
        free_list(&query->_queries, free_query_element_callback);
        free(query);
    }
}

// retrieve time server example
void retrieve_time_server()
{
    APIResponse *api_resp = get_time_server();
    if (!api_resp)
        return;

    if (api_resp->ret_code != 0)
    {
        printf("response code: %d with message: %s\n", api_resp->ret_code, api_resp->ret_msg);
        goto end;
    }

    TimeServerResponse *resp = (TimeServerResponse *)api_resp->result;
    if (!resp)
        goto end;

    printf("time_second: %s\n", resp->time_second);
    printf("time_nano: %s\n", resp->time_nano);

    if (resp->time_second)
        free(resp->time_second);
    if (resp->time_nano)
        free(resp->time_nano);
end:
    free_api_response(api_resp);
}

void print_order_book(OrderB *order)
{
    printf("price: %s\n", order->price);
    printf("size: %s\n", order->size);
}

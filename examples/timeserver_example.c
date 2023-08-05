#include <stdio.h>
#include <stdlib.h>
#include "src/bybit.h"
#include "src/common.h"
#include "src/request.h"
#include "src/response.h"

void retrieve_time_server();

int main(int argc, char *argv[])
{
    /*Uncomment these lines to run examples*/
    retrieve_time_server();
    return 0;
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
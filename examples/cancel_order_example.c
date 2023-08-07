#include <stdio.h>
#include <stdlib.h>
#include "src/bybit.h"
#include "src/common.h"
#include "src/request.h"
#include "src/response.h"

void place_cancel_order();

int main()
{
    place_cancel_order();
}

void place_cancel_order()
{
	Client *clt = new("<API_KEY>", "<API_SECRET>");
	if (!clt)
		return ;

	// allocate memory and initialize order request
	CancelOrderRequest *cancel_order_request = init_cancel_order_request();
	if (!cancel_order_request)
		return ;

	cancel_order_request->category = "spot";
	cancel_order_request->symbol = "BTCUSDT";
    cancel_order_request->order_id = "c6f055d9-7f21-4079-913d-e6523a9cfffa";
    cancel_order_request->order_link_id = 0;
	
	APIResponse *api_resp = post_cancel_order(clt, cancel_order_request);
	if (!api_resp)
		goto end;

	if (api_resp->ret_code != 0)
	{
		printf("response code: %d with message: %s\n", api_resp->ret_code, api_resp->ret_msg);
		goto end;
	}

	if (!api_resp->result) {
		printf("RESULT IS NULL\n");
		goto end;
	}

	OrderResponse *resp = (OrderResponse *)api_resp->result;
	if (!resp)
		goto end;

	printf("ORDER ID: %s\n", resp->order_id);
	printf("ORDER LINK ID: %s\n", resp->order_link_id);
	
	if (resp->order_id) free(resp->order_id);
	if (resp->order_link_id) free(resp->order_link_id);

end:
	if (cancel_order_request) free(cancel_order_request);
	if (api_resp) free_api_response(api_resp);
	if (clt) free(clt);

	return ;
}
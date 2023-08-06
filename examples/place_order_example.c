#include <stdio.h>
#include <stdlib.h>
#include "src/bybit.h"
#include "src/common.h"
#include "src/request.h"
#include "src/response.h"

void place_order();

int main()
{
    place_order();
}

void place_order()
{
	Client *clt = new("<API_KEY>", "<API_SECRET>");
	if (!clt)
		return ;

	// allocate memory and initialize order request
	OrderRequest *order_request = init_order_request();
	if (!order_request)
		return ;

	order_request->category = "spot";
	order_request->symbol = "BTCUSDT";
	order_request->side = "Buy";
	order_request->order_type = "Market";
	order_request->qty = "10000";
	
	APIResponse *api_resp = post_order(clt, order_request);
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
	if (order_request) free(order_request);
	if (api_resp) free_api_response(api_resp);
	if (clt) free(clt);

	return ;
}

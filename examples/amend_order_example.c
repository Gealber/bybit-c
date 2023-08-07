#include <stdio.h>
#include <stdlib.h>
#include "src/bybit.h"
#include "src/common.h"
#include "src/request.h"
#include "src/response.h"

void place_amend_order();

int main()
{
    place_amend_order();
}

void place_amend_order()
{
	Client *clt = new("<API_KEY>", "<API_SECRET>");
	if (!clt)
		return ;

	// allocate memory and initialize order request
	AmendOrderRequest *amend_order_request = init_amend_order_request();
	if (!amend_order_request)
		return ;

	amend_order_request->category = "linear";
	amend_order_request->symbol = "BTCUSDT";
	
	APIResponse *api_resp = post_amend_order(clt, amend_order_request);
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
	if (amend_order_request) free(amend_order_request);
	if (api_resp) free_api_response(api_resp);
	if (clt) free(clt);

	return ;
}
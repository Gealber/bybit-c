#include <stdio.h>
#include <stdlib.h>
#include "src/bybit.h"
#include "src/common.h"
#include "src/request.h"
#include "src/response.h"

void get_open_orders();
void print_open_order(OpenOrder *order);

int main(int argc, char *argv[])
{
    get_open_orders();
	return 0;
}

void get_open_orders()
{
	// Client *clt = new("<API_KEY>", "<API_SECRET>");
	Client *clt = new("wLRb7YfUhzBYjcs8gY", "kKve5Z13n8t16tV7dooSKfs9robjXX2H6eQD");
	if (!clt)
		return ;

	// allocate memory and initialize order request
	OpenOrdersQuery *open_orders_query = build_open_orders_query("spot", "", "", "", "", 0, "", "", "");
	if (!open_orders_query)
		return ;

	APIResponse *api_resp = get_open_order(clt, open_orders_query);
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

	OpenOrdersResponse *resp = (OpenOrdersResponse *)api_resp->result;
	if (!resp)
		goto end;

	struct Node *cur = resp->list;
	if (cur == NULL)
		goto end;

	int i = 0;
	while (cur != NULL)
	{
		OpenOrder *order = (OpenOrder *)cur->val;
		printf("-------------------> %d\n", i);
		print_open_order(order);
		cur = cur->next;
		i++;
	}

	Node *tmp = NULL;
	cur = resp->list;
	while (cur != NULL)
	{
		free_open_order(cur->val);
		tmp = cur;
		cur = cur->next;
		free(tmp);
	}

	free(resp->category);
	free(resp->next_page_cursor);
	

end:
	if (api_resp) free_api_response(api_resp);
	if (clt) free(clt);

	return ;
}


void print_open_order(OpenOrder *order)
{
	printf("order_id: %s\n", order->order_id);
	printf("block_trade_id: %s\n", order->block_trade_id);
	printf("symbol: %s\n", order->symbol);
	printf("price: %s\n", order->price);
	printf("qty: %s\n", order->qty);
	printf("side: %s\n", order->side);
	printf("is_leverage: %s\n", order->is_leverage);
	printf("position_idx: %s\n", order->position_idx);
	printf("order_status: %s\n", order->order_status);
	printf("cancel_type: %s\n", order->cancel_type);
	printf("reject_reason: %s\n", order->reject_reason);
	printf("avg_price: %s\n", order->avg_price);
	printf("leaves_qty: %s\n", order->leaves_qty);
	printf("leaves_value: %s\n", order->leaves_value);
	printf("cum_exec_qty: %s\n", order->cum_exec_qty);
	printf("cum_exec_value: %s\n", order->cum_exec_value);
	printf("cum_exec_fee: %s\n", order->cum_exec_fee);
	printf("time_in_force: %s\n", order->time_in_force);
	printf("order_type: %s\n", order->order_type);
	printf("stop_order_type: %s\n", order->stop_order_type);
	printf("order_lv: %s\n", order->order_lv);
	printf("trigger_price: %s\n", order->trigger_price);
	printf("take_profit: %s\n", order->take_profit);
	printf("stop_loss: %s\n", order->stop_loss);
	printf("tpsl_mode: %s\n", order->tpsl_mode);
	printf("tp_limit_price: %s\n", order->tp_limit_price);
	printf("sl_limit_price: %s\n", order->sl_limit_price);
	printf("tp_trigger_by: %s\n", order->tp_trigger_by);
	printf("sl_trigger_by: %s\n", order->sl_trigger_by);
	printf("trigger_direction: %d\n", order->trigger_direction);
	printf("trigger_by: %s\n", order->trigger_by);
	printf("last_price_on_created: %s\n", order->last_price_on_created);
	printf("reduce_only: %s\n", bool_to_string(order->reduce_only));
	printf("close_on_trigger: %s\n", bool_to_string(order->close_on_trigger));
	printf("place_type: %s\n", order->place_type);
	printf("smp_type: %s\n", order->smp_type);
	printf("smp_group: %d\n", order->smp_group);
	printf("smp_order_id: %s\n", order->smp_order_id);
	printf("created_time: %s\n", order->created_time);
	printf("updated_time: %s\n", order->updated_time);
}
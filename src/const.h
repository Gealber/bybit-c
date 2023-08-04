#ifndef CONST_H
#define CONST_H

const char *DOMAIN_TESTNET = "https://api-testnet.bybit.com/v5";
const char *DOMAIN_MAINNET = "https://api.bybit.com/v5";
const char *ANNOUNCEMENT_PATH = "/announcements/index";
const char *TICKERS_PATH = "/market/tickers";
const char *SERVER_TIME_PATH = "/market/time";
const char *KLINE_PATH = "/market/kline";
const char *MARK_PRICE_KLINE_PATH = "/market/mark-price-kline";
const char *ORDERBOOK_PATH = "/market/orderbook";

const char *USER_AGENT = "bybit-c";

const int BUFFER_SIZE = 32;

#endif
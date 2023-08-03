# bybit-c

Implementation in C of [Bybit API](https://bybit-exchange.github.io/docs/). This implementation will contains the code for V5 of the API, previous versions won't be supported. This is a work in progress, so I don't recommend you to rely on it at the moment. This is basically a prototype at the moment.

## Third parties libraries used

1. [libcurl](https://curl.se/libcurl/), without this library would be literally a pain in the ass to do this job.
2. [cJSON](https://github.com/DaveGamble/cJSON), to parse json.

## Endpoints supported

1. [Get Bybit Server Time](https://bybit-exchange.github.io/docs/v5/market/time)
2. [Get Tickers](https://bybit-exchange.github.io/docs/v5/market/tickers)
3. [Get Kline](https://bybit-exchange.github.io/docs/v5/market/kline)
4. [Get Mark Price Kline](https://bybit-exchange.github.io/docs/v5/market/mark-kline)

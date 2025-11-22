#pragma once

#include "massive/core/config.hpp"
#include "massive/core/http_transport.hpp"
#include "massive/core/json.hpp"
#include "massive/exceptions.hpp"
#include "massive/rest/models.hpp"
#include "massive/rest/models/benzinga.hpp"
#include "massive/rest/models/conversion.hpp"
#include "massive/rest/models/economy.hpp"
#include "massive/rest/models/etf.hpp"
#include "massive/rest/models/financials.hpp"
#include "massive/rest/models/futures.hpp"
#include "massive/rest/models/indicators.hpp"
#include "massive/rest/models/reference.hpp"
#include "massive/rest/models/snapshot.hpp"
#include "massive/rest/models/summaries.hpp"
#include "massive/rest/models/tmx.hpp"
#include "massive/rest/models/vx.hpp"
#include "massive/rest/pagination.hpp"
#include "massive/rest/request_options.hpp"

#include <chrono>
#include <functional>
#include <iterator>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace massive::rest {

class RESTClient {
public:
    RESTClient(core::ClientConfig config, std::shared_ptr<core::IHttpTransport> transport);

    // Aggregates (Bars) - All methods
    std::vector<Agg> list_aggs(const std::string &ticker, int multiplier,
                               const std::string &timespan, const std::string &from,
                               const std::string &to, std::optional<bool> adjusted = std::nullopt,
                               std::optional<std::string> sort = std::nullopt,
                               std::optional<int> limit = std::nullopt);

    std::vector<Agg> get_aggs(const std::string &ticker, int multiplier,
                              const std::string &timespan, const std::string &from,
                              const std::string &to, std::optional<bool> adjusted = std::nullopt,
                              std::optional<std::string> sort = std::nullopt,
                              std::optional<int> limit = std::nullopt);

    std::vector<GroupedDailyAgg> get_grouped_daily_aggs(const std::string &date,
                                                        std::optional<bool> adjusted = std::nullopt,
                                                        const std::string &locale = "us",
                                                        const std::string &market_type = "stocks",
                                                        bool include_otc = false);

    DailyOpenCloseAgg get_daily_open_close_agg(const std::string &ticker, const std::string &date,
                                               std::optional<bool> adjusted = std::nullopt);

    PreviousCloseAgg get_previous_close_agg(const std::string &ticker,
                                            std::optional<bool> adjusted = std::nullopt);

    // Trades
    std::vector<Trade> list_trades(const std::string &ticker,
                                   const std::optional<std::string> &timestamp = std::nullopt,
                                   const std::optional<std::string> &timestamp_lt = std::nullopt,
                                   const std::optional<std::string> &timestamp_lte = std::nullopt,
                                   const std::optional<std::string> &timestamp_gt = std::nullopt,
                                   const std::optional<std::string> &timestamp_gte = std::nullopt,
                                   std::optional<int> limit = std::nullopt,
                                   const std::optional<std::string> &sort = std::nullopt,
                                   const std::optional<std::string> &order = std::nullopt);

    LastTrade get_last_trade(const std::string &ticker);
    CryptoTrade get_last_crypto_trade(const std::string &from, const std::string &to);

    // Quotes
    std::vector<Quote> list_quotes(const std::string &ticker,
                                   const std::optional<std::string> &timestamp = std::nullopt,
                                   const std::optional<std::string> &timestamp_lt = std::nullopt,
                                   const std::optional<std::string> &timestamp_lte = std::nullopt,
                                   const std::optional<std::string> &timestamp_gt = std::nullopt,
                                   const std::optional<std::string> &timestamp_gte = std::nullopt,
                                   std::optional<int> limit = std::nullopt,
                                   const std::optional<std::string> &sort = std::nullopt,
                                   const std::optional<std::string> &order = std::nullopt);

    LastQuote get_last_quote(const std::string &ticker);
    LastForexQuote get_last_forex_quote(const std::string &from, const std::string &to);
    RealTimeCurrencyConversion
    get_real_time_currency_conversion(const std::string &from, const std::string &to,
                                      std::optional<double> amount = std::nullopt,
                                      std::optional<int> precision = std::nullopt);

    // Snapshots
    std::vector<TickerSnapshot> get_snapshot_all(SnapshotMarketType market_type,
                                                 const std::vector<std::string> &tickers = {},
                                                 bool include_otc = false);

    TickerSnapshot get_snapshot_ticker(SnapshotMarketType market_type, const std::string &ticker);

    std::vector<TickerSnapshot> get_snapshot_direction(SnapshotMarketType market_type,
                                                       Direction direction,
                                                       bool include_otc = false);

    std::vector<UniversalSnapshot>
    list_universal_snapshots(const std::optional<std::string> &type = std::nullopt,
                             const std::vector<std::string> &ticker_any_of = {},
                             const std::optional<std::string> &ticker_lt = std::nullopt,
                             const std::optional<std::string> &ticker_lte = std::nullopt,
                             const std::optional<std::string> &ticker_gt = std::nullopt,
                             const std::optional<std::string> &ticker_gte = std::nullopt,
                             std::optional<int> limit = 10,
                             const std::optional<std::string> &sort = std::nullopt,
                             const std::optional<std::string> &order = std::nullopt);

    std::vector<IndicesSnapshot>
    get_snapshot_indices(const std::vector<std::string> &ticker_any_of = {});

    // Snapshot - Options
    TickerSnapshot get_snapshot_option(const std::string &option_ticker);
    std::vector<TickerSnapshot>
    list_snapshot_options_chain(const std::string &underlying_ticker,
                                const std::optional<std::string> &expiration_date = std::nullopt,
                                const std::optional<std::string> &contract_type = std::nullopt);

    // Snapshot - Crypto Book
    SnapshotTickerFullBook get_snapshot_crypto_book(const std::string &ticker);

    // Reference Data - Markets
    std::vector<MarketHoliday> get_market_holidays();
    MarketStatus get_market_status();

    // Reference Data - Tickers
    std::vector<Ticker> list_tickers(const std::optional<std::string> &ticker = std::nullopt,
                                     const std::optional<std::string> &ticker_lt = std::nullopt,
                                     const std::optional<std::string> &ticker_lte = std::nullopt,
                                     const std::optional<std::string> &ticker_gt = std::nullopt,
                                     const std::optional<std::string> &ticker_gte = std::nullopt,
                                     const std::optional<std::string> &type = std::nullopt,
                                     const std::optional<std::string> &market = std::nullopt,
                                     const std::optional<std::string> &exchange = std::nullopt,
                                     std::optional<int> cusip = std::nullopt,
                                     std::optional<int> cik = std::nullopt,
                                     const std::optional<std::string> &date = std::nullopt,
                                     std::optional<bool> active = std::nullopt,
                                     const std::optional<std::string> &search = std::nullopt,
                                     std::optional<int> limit = 10,
                                     const std::optional<std::string> &sort = std::nullopt,
                                     const std::optional<std::string> &order = std::nullopt);

    TickerDetails get_ticker_details(const std::string &ticker,
                                     const std::optional<std::string> &date = std::nullopt);

    // Reference Data - Ticker News
    std::vector<TickerNews> list_ticker_news(const std::string &ticker,
                                             std::optional<int> limit = std::nullopt,
                                             const std::optional<std::string> &order = std::nullopt,
                                             const std::optional<std::string> &sort = std::nullopt);

    // Reference Data - Ticker Types
    std::vector<TickerTypes> get_ticker_types();

    // Reference Data - Related Companies
    std::vector<RelatedCompany> get_related_companies(const std::string &ticker);

    // Reference Data - Ticker Events
    TickerChangeResults get_ticker_events(const std::string &ticker,
                                          const std::optional<std::string> &types = std::nullopt);

    // Reference Data - Splits
    std::vector<Split> list_splits(const std::optional<std::string> &ticker = std::nullopt,
                                   std::optional<int> limit = std::nullopt);

    // Reference Data - Dividends
    std::vector<Dividend> list_dividends(const std::optional<std::string> &ticker = std::nullopt,
                                         std::optional<int> limit = std::nullopt);

    // Reference Data - Conditions
    std::vector<Condition> list_conditions(std::optional<int> limit = std::nullopt);

    // Reference Data - Exchanges
    std::vector<Exchange> get_exchanges();

    // Reference Data - Contracts
    OptionsContract get_options_contract(const std::string &ticker);
    std::vector<OptionsContract>
    list_options_contracts(const std::optional<std::string> &underlying_ticker = std::nullopt,
                           const std::optional<std::string> &underlying_ticker_lt = std::nullopt,
                           const std::optional<std::string> &underlying_ticker_lte = std::nullopt,
                           const std::optional<std::string> &underlying_ticker_gt = std::nullopt,
                           const std::optional<std::string> &underlying_ticker_gte = std::nullopt,
                           const std::optional<std::string> &contract_type = std::nullopt,
                           const std::optional<std::string> &expiration_date = std::nullopt,
                           const std::optional<std::string> &expiration_date_lt = std::nullopt,
                           const std::optional<std::string> &expiration_date_lte = std::nullopt,
                           const std::optional<std::string> &expiration_date_gt = std::nullopt,
                           const std::optional<std::string> &expiration_date_gte = std::nullopt,
                           const std::optional<std::string> &as_of = std::nullopt,
                           std::optional<double> strike_price = std::nullopt,
                           std::optional<double> strike_price_lt = std::nullopt,
                           std::optional<double> strike_price_lte = std::nullopt,
                           std::optional<double> strike_price_gt = std::nullopt,
                           std::optional<double> strike_price_gte = std::nullopt,
                           std::optional<bool> expired = std::nullopt,
                           std::optional<int> limit = std::nullopt,
                           const std::optional<std::string> &sort = std::nullopt,
                           const std::optional<std::string> &order = std::nullopt);

    // Reference Data - IPOs
    std::vector<IPO> list_ipos(const std::optional<std::string> &ticker = std::nullopt,
                               const std::optional<std::string> &us_code = std::nullopt,
                               const std::optional<std::string> &isin = std::nullopt,
                               const std::optional<std::string> &listing_date = std::nullopt,
                               const std::optional<std::string> &listing_date_lt = std::nullopt,
                               const std::optional<std::string> &listing_date_lte = std::nullopt,
                               const std::optional<std::string> &listing_date_gt = std::nullopt,
                               const std::optional<std::string> &listing_date_gte = std::nullopt,
                               const std::optional<std::string> &ipo_status = std::nullopt,
                               std::optional<int> limit = std::nullopt,
                               const std::optional<std::string> &sort = std::nullopt,
                               const std::optional<std::string> &order = std::nullopt);

    // Reference Data - Short Interest
    std::vector<ShortInterest>
    list_short_interest(const std::optional<std::string> &ticker = std::nullopt,
                        const std::optional<std::string> &days_to_cover = std::nullopt,
                        const std::optional<std::string> &days_to_cover_lt = std::nullopt,
                        const std::optional<std::string> &days_to_cover_lte = std::nullopt,
                        const std::optional<std::string> &days_to_cover_gt = std::nullopt,
                        const std::optional<std::string> &days_to_cover_gte = std::nullopt,
                        const std::optional<std::string> &settlement_date = std::nullopt,
                        const std::optional<std::string> &settlement_date_lt = std::nullopt,
                        const std::optional<std::string> &settlement_date_lte = std::nullopt,
                        const std::optional<std::string> &settlement_date_gt = std::nullopt,
                        const std::optional<std::string> &settlement_date_gte = std::nullopt,
                        const std::optional<std::string> &avg_daily_volume = std::nullopt,
                        const std::optional<std::string> &avg_daily_volume_lt = std::nullopt,
                        const std::optional<std::string> &avg_daily_volume_lte = std::nullopt,
                        const std::optional<std::string> &avg_daily_volume_gt = std::nullopt,
                        const std::optional<std::string> &avg_daily_volume_gte = std::nullopt,
                        std::optional<int> limit = std::nullopt,
                        const std::optional<std::string> &sort = std::nullopt,
                        const std::optional<std::string> &order = std::nullopt);

    // Reference Data - Short Volume
    std::vector<ShortVolume>
    list_short_volume(const std::optional<std::string> &ticker = std::nullopt,
                      const std::optional<std::string> &date = std::nullopt,
                      const std::optional<std::string> &date_lt = std::nullopt,
                      const std::optional<std::string> &date_lte = std::nullopt,
                      const std::optional<std::string> &date_gt = std::nullopt,
                      const std::optional<std::string> &date_gte = std::nullopt,
                      const std::optional<std::string> &short_volume_ratio = std::nullopt,
                      const std::optional<std::string> &short_volume_ratio_lt = std::nullopt,
                      const std::optional<std::string> &short_volume_ratio_lte = std::nullopt,
                      const std::optional<std::string> &short_volume_ratio_gt = std::nullopt,
                      const std::optional<std::string> &short_volume_ratio_gte = std::nullopt,
                      const std::optional<std::string> &total_volume = std::nullopt,
                      const std::optional<std::string> &total_volume_lt = std::nullopt,
                      const std::optional<std::string> &total_volume_lte = std::nullopt,
                      const std::optional<std::string> &total_volume_gt = std::nullopt,
                      const std::optional<std::string> &total_volume_gte = std::nullopt,
                      std::optional<int> limit = std::nullopt,
                      const std::optional<std::string> &sort = std::nullopt,
                      const std::optional<std::string> &order = std::nullopt);

    // Summaries
    std::vector<SummaryResult> get_summaries(const std::vector<std::string> &ticker_any_of = {});

    // Indicators
    SMAIndicatorResults get_sma(const std::string &ticker,
                                const std::optional<std::string> &timespan = std::nullopt,
                                std::optional<int> window = std::nullopt,
                                std::optional<bool> adjusted = std::nullopt,
                                std::optional<int> limit = std::nullopt);

    EMAIndicatorResults get_ema(const std::string &ticker,
                                const std::optional<std::string> &timespan = std::nullopt,
                                std::optional<int> window = std::nullopt,
                                std::optional<bool> adjusted = std::nullopt,
                                std::optional<int> limit = std::nullopt);

    RSIIndicatorResults get_rsi(const std::string &ticker,
                                const std::optional<std::string> &timespan = std::nullopt,
                                std::optional<int> window = std::nullopt,
                                std::optional<bool> adjusted = std::nullopt,
                                std::optional<int> limit = std::nullopt);

    MACDIndicatorResults get_macd(const std::string &ticker,
                                  const std::optional<std::string> &timespan = std::nullopt,
                                  std::optional<int> short_window = std::nullopt,
                                  std::optional<int> long_window = std::nullopt,
                                  std::optional<int> signal_window = std::nullopt,
                                  std::optional<bool> adjusted = std::nullopt,
                                  std::optional<int> limit = std::nullopt);

    // Futures
    std::vector<FuturesAgg>
    list_futures_aggregates(const std::string &ticker,
                            const std::optional<std::string> &resolution = std::nullopt,
                            std::optional<int> limit = std::nullopt);

    FuturesContract get_futures_contract_details(const std::string &ticker);
    std::vector<FuturesContract>
    list_futures_contracts(const std::optional<std::string> &product_code = std::nullopt,
                           std::optional<int> limit = std::nullopt);

    std::vector<FuturesQuote> list_futures_quotes(const std::string &ticker,
                                                  std::optional<int> limit = std::nullopt);

    std::vector<FuturesTrade> list_futures_trades(const std::string &ticker,
                                                  std::optional<int> limit = std::nullopt);

    // Futures - Additional Methods
    std::vector<FuturesProduct>
    list_futures_products(const std::optional<std::string> &name = std::nullopt,
                          const std::optional<std::string> &name_search = std::nullopt,
                          const std::optional<std::string> &as_of = std::nullopt,
                          const std::optional<std::string> &trading_venue = std::nullopt,
                          std::optional<int> limit = std::nullopt,
                          const std::optional<std::string> &sort = std::nullopt);
    FuturesProduct
    get_futures_product_details(const std::string &product_code,
                                const std::optional<std::string> &type = std::nullopt,
                                const std::optional<std::string> &as_of = std::nullopt);
    FuturesSchedule get_futures_schedule(const std::string &ticker);
    std::vector<FuturesSchedule>
    list_futures_schedules(const std::optional<std::string> &session_end_date = std::nullopt,
                           const std::optional<std::string> &trading_venue = std::nullopt,
                           std::optional<int> limit = std::nullopt,
                           const std::optional<std::string> &sort = std::nullopt);
    std::vector<FuturesSchedule> list_futures_schedules_by_product_code(
        const std::string &product_code,
        const std::optional<std::string> &session_end_date = std::nullopt,
        const std::optional<std::string> &session_end_date_lt = std::nullopt,
        const std::optional<std::string> &session_end_date_lte = std::nullopt,
        const std::optional<std::string> &session_end_date_gt = std::nullopt,
        const std::optional<std::string> &session_end_date_gte = std::nullopt,
        std::optional<int> limit = std::nullopt,
        const std::optional<std::string> &sort = std::nullopt);
    FuturesMarketStatus get_futures_market_status();
    std::vector<FuturesMarketStatus> list_futures_market_statuses(
        const std::optional<std::string> &product_code = std::nullopt,
        const std::optional<std::string> &product_code_any_of = std::nullopt,
        std::optional<int> limit = std::nullopt,
        const std::optional<std::string> &sort = std::nullopt);
    FuturesSnapshot get_futures_snapshot(const std::string &ticker);
    std::vector<FuturesExchange> list_futures_exchanges();

    // Financials - Balance Sheets
    std::vector<BalanceSheet>
    list_balance_sheets(const std::optional<std::string> &cik = std::nullopt,
                        const std::optional<std::string> &cik_any_of = std::nullopt,
                        const std::optional<std::string> &cik_gt = std::nullopt,
                        const std::optional<std::string> &cik_gte = std::nullopt,
                        const std::optional<std::string> &cik_lt = std::nullopt,
                        const std::optional<std::string> &cik_lte = std::nullopt,
                        const std::optional<std::string> &tickers = std::nullopt,
                        const std::optional<std::string> &tickers_all_of = std::nullopt,
                        const std::optional<std::string> &tickers_any_of = std::nullopt,
                        const std::optional<std::string> &period_end = std::nullopt,
                        const std::optional<std::string> &period_end_gt = std::nullopt,
                        const std::optional<std::string> &period_end_gte = std::nullopt,
                        const std::optional<std::string> &period_end_lt = std::nullopt,
                        const std::optional<std::string> &period_end_lte = std::nullopt,
                        const std::optional<std::string> &filing_date = std::nullopt,
                        const std::optional<std::string> &filing_date_gt = std::nullopt,
                        const std::optional<std::string> &filing_date_gte = std::nullopt,
                        const std::optional<std::string> &filing_date_lt = std::nullopt,
                        const std::optional<std::string> &filing_date_lte = std::nullopt,
                        std::optional<double> fiscal_year = std::nullopt,
                        std::optional<double> fiscal_year_gt = std::nullopt,
                        std::optional<double> fiscal_year_gte = std::nullopt,
                        std::optional<double> fiscal_year_lt = std::nullopt,
                        std::optional<double> fiscal_year_lte = std::nullopt,
                        std::optional<double> fiscal_quarter = std::nullopt,
                        std::optional<double> fiscal_quarter_gt = std::nullopt,
                        std::optional<double> fiscal_quarter_gte = std::nullopt,
                        std::optional<double> fiscal_quarter_lt = std::nullopt,
                        std::optional<double> fiscal_quarter_lte = std::nullopt,
                        const std::optional<std::string> &timeframe = std::nullopt,
                        const std::optional<std::string> &timeframe_any_of = std::nullopt,
                        const std::optional<std::string> &timeframe_gt = std::nullopt,
                        const std::optional<std::string> &timeframe_gte = std::nullopt,
                        const std::optional<std::string> &timeframe_lt = std::nullopt,
                        const std::optional<std::string> &timeframe_lte = std::nullopt,
                        std::optional<int> limit = std::nullopt,
                        const std::optional<std::string> &sort = std::nullopt);

    // Financials - Income Statements
    std::vector<IncomeStatement>
    list_income_statements(const std::optional<std::string> &cik = std::nullopt,
                           const std::optional<std::string> &cik_any_of = std::nullopt,
                           const std::optional<std::string> &cik_gt = std::nullopt,
                           const std::optional<std::string> &cik_gte = std::nullopt,
                           const std::optional<std::string> &cik_lt = std::nullopt,
                           const std::optional<std::string> &cik_lte = std::nullopt,
                           const std::optional<std::string> &tickers = std::nullopt,
                           const std::optional<std::string> &tickers_all_of = std::nullopt,
                           const std::optional<std::string> &tickers_any_of = std::nullopt,
                           const std::optional<std::string> &period_end = std::nullopt,
                           const std::optional<std::string> &period_end_gt = std::nullopt,
                           const std::optional<std::string> &period_end_gte = std::nullopt,
                           const std::optional<std::string> &period_end_lt = std::nullopt,
                           const std::optional<std::string> &period_end_lte = std::nullopt,
                           const std::optional<std::string> &filing_date = std::nullopt,
                           const std::optional<std::string> &filing_date_gt = std::nullopt,
                           const std::optional<std::string> &filing_date_gte = std::nullopt,
                           const std::optional<std::string> &filing_date_lt = std::nullopt,
                           const std::optional<std::string> &filing_date_lte = std::nullopt,
                           std::optional<double> fiscal_year = std::nullopt,
                           std::optional<double> fiscal_year_gt = std::nullopt,
                           std::optional<double> fiscal_year_gte = std::nullopt,
                           std::optional<double> fiscal_year_lt = std::nullopt,
                           std::optional<double> fiscal_year_lte = std::nullopt,
                           std::optional<double> fiscal_quarter = std::nullopt,
                           std::optional<double> fiscal_quarter_gt = std::nullopt,
                           std::optional<double> fiscal_quarter_gte = std::nullopt,
                           std::optional<double> fiscal_quarter_lt = std::nullopt,
                           std::optional<double> fiscal_quarter_lte = std::nullopt,
                           const std::optional<std::string> &timeframe = std::nullopt,
                           const std::optional<std::string> &timeframe_any_of = std::nullopt,
                           const std::optional<std::string> &timeframe_gt = std::nullopt,
                           const std::optional<std::string> &timeframe_gte = std::nullopt,
                           const std::optional<std::string> &timeframe_lt = std::nullopt,
                           const std::optional<std::string> &timeframe_lte = std::nullopt,
                           std::optional<int> limit = std::nullopt,
                           const std::optional<std::string> &sort = std::nullopt);

    // Financials - Cash Flow Statements
    std::vector<CashFlowStatement>
    list_cash_flow_statements(const std::optional<std::string> &cik = std::nullopt,
                              const std::optional<std::string> &cik_any_of = std::nullopt,
                              const std::optional<std::string> &cik_gt = std::nullopt,
                              const std::optional<std::string> &cik_gte = std::nullopt,
                              const std::optional<std::string> &cik_lt = std::nullopt,
                              const std::optional<std::string> &cik_lte = std::nullopt,
                              const std::optional<std::string> &period_end = std::nullopt,
                              const std::optional<std::string> &period_end_gt = std::nullopt,
                              const std::optional<std::string> &period_end_gte = std::nullopt,
                              const std::optional<std::string> &period_end_lt = std::nullopt,
                              const std::optional<std::string> &period_end_lte = std::nullopt,
                              const std::optional<std::string> &filing_date = std::nullopt,
                              const std::optional<std::string> &filing_date_gt = std::nullopt,
                              const std::optional<std::string> &filing_date_gte = std::nullopt,
                              const std::optional<std::string> &filing_date_lt = std::nullopt,
                              const std::optional<std::string> &filing_date_lte = std::nullopt,
                              const std::optional<std::string> &tickers = std::nullopt,
                              const std::optional<std::string> &tickers_all_of = std::nullopt,
                              const std::optional<std::string> &tickers_any_of = std::nullopt,
                              std::optional<double> fiscal_year = std::nullopt,
                              std::optional<double> fiscal_year_gt = std::nullopt,
                              std::optional<double> fiscal_year_gte = std::nullopt,
                              std::optional<double> fiscal_year_lt = std::nullopt,
                              std::optional<double> fiscal_year_lte = std::nullopt,
                              std::optional<double> fiscal_quarter = std::nullopt,
                              std::optional<double> fiscal_quarter_gt = std::nullopt,
                              std::optional<double> fiscal_quarter_gte = std::nullopt,
                              std::optional<double> fiscal_quarter_lt = std::nullopt,
                              std::optional<double> fiscal_quarter_lte = std::nullopt,
                              const std::optional<std::string> &timeframe = std::nullopt,
                              const std::optional<std::string> &timeframe_any_of = std::nullopt,
                              const std::optional<std::string> &timeframe_gt = std::nullopt,
                              const std::optional<std::string> &timeframe_gte = std::nullopt,
                              const std::optional<std::string> &timeframe_lt = std::nullopt,
                              const std::optional<std::string> &timeframe_lte = std::nullopt,
                              std::optional<int> limit = std::nullopt,
                              const std::optional<std::string> &sort = std::nullopt);

    // Financials - Ratios
    std::vector<FinancialRatios> list_financial_ratios(
        const std::optional<std::string> &ticker = std::nullopt,
        const std::optional<std::string> &ticker_any_of = std::nullopt,
        const std::optional<std::string> &ticker_gt = std::nullopt,
        const std::optional<std::string> &ticker_gte = std::nullopt,
        const std::optional<std::string> &ticker_lt = std::nullopt,
        const std::optional<std::string> &ticker_lte = std::nullopt,
        const std::optional<std::string> &cik = std::nullopt,
        const std::optional<std::string> &cik_any_of = std::nullopt,
        const std::optional<std::string> &cik_gt = std::nullopt,
        const std::optional<std::string> &cik_gte = std::nullopt,
        const std::optional<std::string> &cik_lt = std::nullopt,
        const std::optional<std::string> &cik_lte = std::nullopt,
        std::optional<double> price = std::nullopt, std::optional<double> price_gt = std::nullopt,
        std::optional<double> price_gte = std::nullopt,
        std::optional<double> price_lt = std::nullopt,
        std::optional<double> price_lte = std::nullopt,
        std::optional<double> average_volume = std::nullopt,
        std::optional<double> average_volume_gt = std::nullopt,
        std::optional<double> average_volume_gte = std::nullopt,
        std::optional<double> average_volume_lt = std::nullopt,
        std::optional<double> average_volume_lte = std::nullopt,
        std::optional<double> market_cap = std::nullopt,
        std::optional<double> market_cap_gt = std::nullopt,
        std::optional<double> market_cap_gte = std::nullopt,
        std::optional<double> market_cap_lt = std::nullopt,
        std::optional<double> market_cap_lte = std::nullopt,
        std::optional<double> earnings_per_share = std::nullopt,
        std::optional<double> earnings_per_share_gt = std::nullopt,
        std::optional<double> earnings_per_share_gte = std::nullopt,
        std::optional<double> earnings_per_share_lt = std::nullopt,
        std::optional<double> earnings_per_share_lte = std::nullopt,
        std::optional<double> price_to_earnings = std::nullopt,
        std::optional<double> price_to_earnings_gt = std::nullopt,
        std::optional<double> price_to_earnings_gte = std::nullopt,
        std::optional<double> price_to_earnings_lt = std::nullopt,
        std::optional<double> price_to_earnings_lte = std::nullopt,
        std::optional<double> price_to_book = std::nullopt,
        std::optional<double> price_to_book_gt = std::nullopt,
        std::optional<double> price_to_book_gte = std::nullopt,
        std::optional<double> price_to_book_lt = std::nullopt,
        std::optional<double> price_to_book_lte = std::nullopt,
        std::optional<double> price_to_sales = std::nullopt,
        std::optional<double> price_to_sales_gt = std::nullopt,
        std::optional<double> price_to_sales_gte = std::nullopt,
        std::optional<double> price_to_sales_lt = std::nullopt,
        std::optional<double> price_to_sales_lte = std::nullopt,
        std::optional<double> price_to_cash_flow = std::nullopt,
        std::optional<double> price_to_cash_flow_gt = std::nullopt,
        std::optional<double> price_to_cash_flow_gte = std::nullopt,
        std::optional<double> price_to_cash_flow_lt = std::nullopt,
        std::optional<double> price_to_cash_flow_lte = std::nullopt,
        std::optional<double> price_to_free_cash_flow = std::nullopt,
        std::optional<double> price_to_free_cash_flow_gt = std::nullopt,
        std::optional<double> price_to_free_cash_flow_gte = std::nullopt,
        std::optional<double> price_to_free_cash_flow_lt = std::nullopt,
        std::optional<double> price_to_free_cash_flow_lte = std::nullopt,
        std::optional<double> dividend_yield = std::nullopt,
        std::optional<double> dividend_yield_gt = std::nullopt,
        std::optional<double> dividend_yield_gte = std::nullopt,
        std::optional<double> dividend_yield_lt = std::nullopt,
        std::optional<double> dividend_yield_lte = std::nullopt,
        std::optional<double> return_on_assets = std::nullopt,
        std::optional<double> return_on_assets_gt = std::nullopt,
        std::optional<double> return_on_assets_gte = std::nullopt,
        std::optional<double> return_on_assets_lt = std::nullopt,
        std::optional<double> return_on_assets_lte = std::nullopt,
        std::optional<double> return_on_equity = std::nullopt,
        std::optional<double> return_on_equity_gt = std::nullopt,
        std::optional<double> return_on_equity_gte = std::nullopt,
        std::optional<double> return_on_equity_lt = std::nullopt,
        std::optional<double> return_on_equity_lte = std::nullopt,
        std::optional<double> debt_to_equity = std::nullopt,
        std::optional<double> debt_to_equity_gt = std::nullopt,
        std::optional<double> debt_to_equity_gte = std::nullopt,
        std::optional<double> debt_to_equity_lt = std::nullopt,
        std::optional<double> debt_to_equity_lte = std::nullopt,
        std::optional<double> current = std::nullopt,
        std::optional<double> current_gt = std::nullopt,
        std::optional<double> current_gte = std::nullopt,
        std::optional<double> current_lt = std::nullopt,
        std::optional<double> current_lte = std::nullopt,
        std::optional<double> quick = std::nullopt, std::optional<double> quick_gt = std::nullopt,
        std::optional<double> quick_gte = std::nullopt,
        std::optional<double> quick_lt = std::nullopt,
        std::optional<double> quick_lte = std::nullopt, std::optional<double> cash = std::nullopt,
        std::optional<double> cash_gt = std::nullopt, std::optional<double> cash_gte = std::nullopt,
        std::optional<double> cash_lt = std::nullopt, std::optional<double> cash_lte = std::nullopt,
        std::optional<double> ev_to_sales = std::nullopt,
        std::optional<double> ev_to_sales_gt = std::nullopt,
        std::optional<double> ev_to_sales_gte = std::nullopt,
        std::optional<double> ev_to_sales_lt = std::nullopt,
        std::optional<double> ev_to_sales_lte = std::nullopt,
        std::optional<double> ev_to_ebitda = std::nullopt,
        std::optional<double> ev_to_ebitda_gt = std::nullopt,
        std::optional<double> ev_to_ebitda_gte = std::nullopt,
        std::optional<double> ev_to_ebitda_lt = std::nullopt,
        std::optional<double> ev_to_ebitda_lte = std::nullopt,
        std::optional<double> enterprise_value = std::nullopt,
        std::optional<double> enterprise_value_gt = std::nullopt,
        std::optional<double> enterprise_value_gte = std::nullopt,
        std::optional<double> enterprise_value_lt = std::nullopt,
        std::optional<double> enterprise_value_lte = std::nullopt,
        std::optional<double> free_cash_flow = std::nullopt,
        std::optional<double> free_cash_flow_gt = std::nullopt,
        std::optional<double> free_cash_flow_gte = std::nullopt,
        std::optional<double> free_cash_flow_lt = std::nullopt,
        std::optional<double> free_cash_flow_lte = std::nullopt,
        std::optional<int> limit = std::nullopt,
        const std::optional<std::string> &sort = std::nullopt);

    // Benzinga
    std::vector<BenzingaNews>
    list_benzinga_news(const std::optional<std::string> &ticker = std::nullopt,
                       const std::optional<std::vector<std::string>> &tickers_any_of = std::nullopt,
                       std::optional<int> limit = std::nullopt,
                       const std::optional<std::string> &published_utc_gte = std::nullopt,
                       const std::optional<std::string> &published_utc_lte = std::nullopt,
                       const std::optional<std::string> &sort = std::nullopt,
                       const std::optional<std::string> &order = std::nullopt);

    std::vector<BenzingaEvent> list_benzinga_events(
        const std::optional<std::string> &ticker = std::nullopt,
        const std::optional<std::vector<std::string>> &tickers_any_of = std::nullopt,
        std::optional<int> limit = std::nullopt,
        const std::optional<std::string> &start_date_gte = std::nullopt,
        const std::optional<std::string> &start_date_lte = std::nullopt,
        const std::optional<std::string> &types = std::nullopt,
        const std::optional<std::string> &sort = std::nullopt,
        const std::optional<std::string> &order = std::nullopt);

    std::vector<BenzingaPressRelease> list_benzinga_press_releases(
        const std::optional<std::string> &ticker = std::nullopt,
        const std::optional<std::vector<std::string>> &tickers_any_of = std::nullopt,
        std::optional<int> limit = std::nullopt,
        const std::optional<std::string> &published_utc_gte = std::nullopt,
        const std::optional<std::string> &published_utc_lte = std::nullopt,
        const std::optional<std::string> &sort = std::nullopt,
        const std::optional<std::string> &order = std::nullopt);

    std::vector<BenzingaAnalystRating> list_benzinga_analyst_ratings(
        const std::optional<std::string> &ticker = std::nullopt,
        const std::optional<std::vector<std::string>> &tickers_any_of = std::nullopt,
        std::optional<int> limit = std::nullopt,
        const std::optional<std::string> &published_utc_gte = std::nullopt,
        const std::optional<std::string> &published_utc_lte = std::nullopt,
        const std::optional<std::string> &sort = std::nullopt,
        const std::optional<std::string> &order = std::nullopt);

    // Benzinga - Additional Methods
    std::vector<BenzingaAnalystInsight>
    list_benzinga_analyst_insights(const std::optional<std::string> &ticker = std::nullopt,
                                   const std::optional<std::string> &date = std::nullopt,
                                   const std::optional<std::string> &firm = std::nullopt,
                                   std::optional<int> limit = std::nullopt,
                                   const std::optional<std::string> &sort = std::nullopt);

    std::vector<BenzingaAnalyst>
    list_benzinga_analysts(const std::optional<std::string> &benzinga_firm_id = std::nullopt,
                           const std::optional<std::string> &firm_name = std::nullopt,
                           const std::optional<std::string> &full_name = std::nullopt,
                           std::optional<int> limit = std::nullopt,
                           const std::optional<std::string> &sort = std::nullopt);

    std::vector<BenzingaConsensusRating>
    list_benzinga_consensus_ratings(const std::string &ticker,
                                    const std::optional<std::string> &date = std::nullopt,
                                    const std::optional<std::string> &date_gte = std::nullopt,
                                    const std::optional<std::string> &date_lte = std::nullopt,
                                    std::optional<int> limit = std::nullopt);

    std::vector<BenzingaEarning>
    list_benzinga_earnings(const std::optional<std::string> &ticker = std::nullopt,
                           const std::optional<std::string> &date = std::nullopt,
                           const std::optional<std::string> &date_gte = std::nullopt,
                           const std::optional<std::string> &date_lte = std::nullopt,
                           std::optional<int> limit = std::nullopt,
                           const std::optional<std::string> &sort = std::nullopt);

    std::vector<BenzingaFirm>
    list_benzinga_firms(const std::optional<std::string> &benzinga_id = std::nullopt,
                        const std::optional<std::string> &name = std::nullopt,
                        std::optional<int> limit = std::nullopt,
                        const std::optional<std::string> &sort = std::nullopt);

    std::vector<BenzingaGuidance>
    list_benzinga_guidance(const std::optional<std::string> &ticker = std::nullopt,
                           const std::optional<std::string> &date = std::nullopt,
                           const std::optional<std::string> &date_gte = std::nullopt,
                           const std::optional<std::string> &date_lte = std::nullopt,
                           std::optional<int> limit = std::nullopt,
                           const std::optional<std::string> &sort = std::nullopt);

    std::vector<BenzingaNews> list_benzinga_news_v2(
        const std::optional<std::string> &ticker = std::nullopt,
        const std::optional<std::vector<std::string>> &tickers_any_of = std::nullopt,
        std::optional<int> limit = std::nullopt,
        const std::optional<std::string> &published_utc_gte = std::nullopt,
        const std::optional<std::string> &published_utc_lte = std::nullopt,
        const std::optional<std::string> &sort = std::nullopt,
        const std::optional<std::string> &order = std::nullopt);

    std::vector<BenzingaRating>
    list_benzinga_ratings(const std::optional<std::string> &ticker = std::nullopt,
                          const std::optional<std::string> &date = std::nullopt,
                          const std::optional<std::string> &date_gte = std::nullopt,
                          const std::optional<std::string> &date_lte = std::nullopt,
                          const std::optional<std::string> &firm = std::nullopt,
                          std::optional<int> limit = std::nullopt,
                          const std::optional<std::string> &sort = std::nullopt);

    // Economy
    std::vector<EconomicIndicator>
    list_economic_indicators(const std::optional<std::string> &indicator = std::nullopt,
                             const std::optional<std::string> &country = std::nullopt,
                             std::optional<int> limit = std::nullopt,
                             const std::optional<std::string> &date_gte = std::nullopt,
                             const std::optional<std::string> &date_lte = std::nullopt,
                             const std::optional<std::string> &sort = std::nullopt,
                             const std::optional<std::string> &order = std::nullopt);

    std::vector<EconomicCalendarEvent>
    list_economic_calendar_events(const std::optional<std::string> &country = std::nullopt,
                                  const std::optional<std::string> &importance = std::nullopt,
                                  std::optional<int> limit = std::nullopt,
                                  const std::optional<std::string> &date_gte = std::nullopt,
                                  const std::optional<std::string> &date_lte = std::nullopt,
                                  const std::optional<std::string> &sort = std::nullopt,
                                  const std::optional<std::string> &order = std::nullopt);

    std::vector<EconomicDataPoint>
    get_economic_data_series(const std::string &indicator,
                             const std::optional<std::string> &country = std::nullopt,
                             std::optional<int> limit = std::nullopt,
                             const std::optional<std::string> &date_gte = std::nullopt,
                             const std::optional<std::string> &date_lte = std::nullopt,
                             const std::optional<std::string> &sort = std::nullopt,
                             const std::optional<std::string> &order = std::nullopt);

    // Economy - Treasury Yields
    std::vector<TreasuryYield>
    list_treasury_yields(const std::optional<std::string> &date = std::nullopt,
                         const std::optional<std::string> &date_any_of = std::nullopt,
                         const std::optional<std::string> &date_gt = std::nullopt,
                         const std::optional<std::string> &date_gte = std::nullopt,
                         const std::optional<std::string> &date_lt = std::nullopt,
                         const std::optional<std::string> &date_lte = std::nullopt,
                         std::optional<int> limit = std::nullopt,
                         const std::optional<std::string> &sort = std::nullopt,
                         const std::optional<std::string> &order = std::nullopt);

    // Economy - Inflation
    std::vector<FedInflation>
    list_inflation(const std::optional<std::string> &date = std::nullopt,
                   const std::optional<std::string> &date_any_of = std::nullopt,
                   const std::optional<std::string> &date_gt = std::nullopt,
                   const std::optional<std::string> &date_gte = std::nullopt,
                   const std::optional<std::string> &date_lt = std::nullopt,
                   const std::optional<std::string> &date_lte = std::nullopt,
                   std::optional<int> limit = std::nullopt,
                   const std::optional<std::string> &sort = std::nullopt);

    // Economy - Inflation Expectations
    std::vector<FedInflationExpectations>
    list_inflation_expectations(const std::optional<std::string> &date = std::nullopt,
                                const std::optional<std::string> &date_any_of = std::nullopt,
                                const std::optional<std::string> &date_gt = std::nullopt,
                                const std::optional<std::string> &date_gte = std::nullopt,
                                const std::optional<std::string> &date_lt = std::nullopt,
                                const std::optional<std::string> &date_lte = std::nullopt,
                                std::optional<int> limit = std::nullopt,
                                const std::optional<std::string> &sort = std::nullopt);

    // ETF Global
    std::vector<EtfListItem> list_etfs(const std::optional<std::string> &ticker = std::nullopt,
                                       const std::optional<std::string> &exchange = std::nullopt,
                                       const std::optional<std::string> &category = std::nullopt,
                                       std::optional<int> limit = std::nullopt,
                                       const std::optional<std::string> &sort = std::nullopt,
                                       const std::optional<std::string> &order = std::nullopt);

    EtfDetails get_etf_details(const std::string &ticker);

    std::vector<EtfHolding>
    list_etf_holdings(const std::string &ticker, std::optional<int> limit = std::nullopt,
                      const std::optional<std::string> &date = std::nullopt);

    std::vector<EtfPerformance>
    get_etf_performance(const std::string &ticker,
                        const std::optional<std::string> &date_gte = std::nullopt,
                        const std::optional<std::string> &date_lte = std::nullopt,
                        std::optional<int> limit = std::nullopt,
                        const std::optional<std::string> &sort = std::nullopt,
                        const std::optional<std::string> &order = std::nullopt);

    // ETF Global - Analytics
    std::vector<EtfGlobalAnalytics> get_etf_global_analytics(
        const std::optional<std::string> &composite_ticker = std::nullopt,
        const std::optional<std::string> &composite_ticker_any_of = std::nullopt,
        const std::optional<std::string> &processed_date = std::nullopt,
        const std::optional<std::string> &effective_date = std::nullopt,
        std::optional<int> limit = std::nullopt,
        const std::optional<std::string> &sort = std::nullopt);

    // ETF Global - Constituents
    std::vector<EtfGlobalConstituent> get_etf_global_constituents(
        const std::optional<std::string> &composite_ticker = std::nullopt,
        const std::optional<std::string> &composite_ticker_any_of = std::nullopt,
        const std::optional<std::string> &constituent_ticker = std::nullopt,
        const std::optional<std::string> &effective_date = std::nullopt,
        const std::optional<std::string> &processed_date = std::nullopt,
        std::optional<int> limit = std::nullopt,
        const std::optional<std::string> &sort = std::nullopt);

    // ETF Global - Fund Flows
    std::vector<EtfGlobalFundFlow> get_etf_global_fund_flows(
        const std::optional<std::string> &composite_ticker = std::nullopt,
        const std::optional<std::string> &composite_ticker_any_of = std::nullopt,
        const std::optional<std::string> &processed_date = std::nullopt,
        const std::optional<std::string> &effective_date = std::nullopt,
        std::optional<int> limit = std::nullopt,
        const std::optional<std::string> &sort = std::nullopt);

    // ETF Global - Profiles
    std::vector<EtfGlobalProfile> get_etf_global_profiles(
        const std::optional<std::string> &composite_ticker = std::nullopt,
        const std::optional<std::string> &composite_ticker_any_of = std::nullopt,
        const std::optional<std::string> &processed_date = std::nullopt,
        const std::optional<std::string> &effective_date = std::nullopt,
        std::optional<int> limit = std::nullopt,
        const std::optional<std::string> &sort = std::nullopt);

    // ETF Global - Taxonomies
    std::vector<EtfGlobalTaxonomy> get_etf_global_taxonomies(
        const std::optional<std::string> &composite_ticker = std::nullopt,
        const std::optional<std::string> &composite_ticker_any_of = std::nullopt,
        const std::optional<std::string> &processed_date = std::nullopt,
        const std::optional<std::string> &effective_date = std::nullopt,
        std::optional<int> limit = std::nullopt,
        const std::optional<std::string> &sort = std::nullopt);

    // TMX
    std::vector<TmxTrade>
    list_tmx_trades(const std::string &ticker,
                    const std::optional<std::string> &timestamp = std::nullopt,
                    std::optional<int> limit = std::nullopt,
                    const std::optional<std::string> &sort = std::nullopt);

    TmxTrade get_tmx_last_trade(const std::string &ticker);

    std::vector<TmxQuote>
    list_tmx_quotes(const std::string &ticker,
                    const std::optional<std::string> &timestamp = std::nullopt,
                    std::optional<int> limit = std::nullopt,
                    const std::optional<std::string> &sort = std::nullopt);

    TmxQuote get_tmx_last_quote(const std::string &ticker);

    std::vector<TmxAgg> list_tmx_aggs(const std::string &ticker, int multiplier,
                                      const std::string &timespan, const std::string &from,
                                      const std::string &to,
                                      std::optional<bool> adjusted = std::nullopt,
                                      std::optional<std::string> sort = std::nullopt,
                                      std::optional<int> limit = std::nullopt);

    TmxTickerDetails get_tmx_ticker_details(const std::string &ticker);

    // TMX - Corporate Events
    std::vector<TmxCorporateEvent>
    list_tmx_corporate_events(const std::optional<std::string> &date = std::nullopt,
                              const std::optional<std::string> &date_any_of = std::nullopt,
                              const std::optional<std::string> &date_gt = std::nullopt,
                              const std::optional<std::string> &date_gte = std::nullopt,
                              const std::optional<std::string> &date_lt = std::nullopt,
                              const std::optional<std::string> &date_lte = std::nullopt,
                              const std::optional<std::string> &type = std::nullopt,
                              const std::optional<std::string> &ticker = std::nullopt,
                              std::optional<int> limit = std::nullopt,
                              const std::optional<std::string> &sort = std::nullopt);

    // VX (Experimental)
    std::vector<VxVolatility>
    list_vx_volatility(const std::optional<std::string> &ticker = std::nullopt,
                       std::optional<int> limit = std::nullopt,
                       const std::optional<std::string> &date_gte = std::nullopt,
                       const std::optional<std::string> &date_lte = std::nullopt,
                       const std::optional<std::string> &sort = std::nullopt,
                       const std::optional<std::string> &order = std::nullopt);

    std::vector<VxDataPoint>
    list_vx_experimental(const std::string &endpoint,
                         const std::optional<std::string> &ticker = std::nullopt,
                         std::optional<int> limit = std::nullopt,
                         const std::optional<std::string> &date_gte = std::nullopt,
                         const std::optional<std::string> &date_lte = std::nullopt,
                         const std::map<std::string, std::string> &additional_params = {});

    // VX - Stock Financials
    std::vector<StockFinancial> list_stock_financials(
        const std::optional<std::string> &ticker = std::nullopt,
        const std::optional<std::string> &cik = std::nullopt,
        const std::optional<std::string> &company_name = std::nullopt,
        const std::optional<std::string> &company_name_search = std::nullopt,
        const std::optional<std::string> &sic = std::nullopt,
        const std::optional<std::string> &filing_date = std::nullopt,
        const std::optional<std::string> &filing_date_lt = std::nullopt,
        const std::optional<std::string> &filing_date_lte = std::nullopt,
        const std::optional<std::string> &filing_date_gt = std::nullopt,
        const std::optional<std::string> &filing_date_gte = std::nullopt,
        const std::optional<std::string> &period_of_report_date = std::nullopt,
        const std::optional<std::string> &period_of_report_date_lt = std::nullopt,
        const std::optional<std::string> &period_of_report_date_lte = std::nullopt,
        const std::optional<std::string> &period_of_report_date_gt = std::nullopt,
        const std::optional<std::string> &period_of_report_date_gte = std::nullopt,
        const std::optional<std::string> &timeframe = std::nullopt,
        std::optional<bool> include_sources = std::nullopt, std::optional<int> limit = std::nullopt,
        const std::optional<std::string> &sort = std::nullopt,
        const std::optional<std::string> &order = std::nullopt);

    // Conversion - Forex
    ForexConversion
    get_forex_conversion(const std::string &from, const std::string &to, double amount,
                         const std::optional<std::string> &date = std::nullopt,
                         const std::optional<std::string> &precision = std::nullopt);

    // Conversion - Crypto
    CryptoConversion
    get_crypto_conversion(const std::string &from, const std::string &to, double amount,
                          const std::optional<std::string> &date = std::nullopt,
                          const std::optional<std::string> &precision = std::nullopt);

    // Paginated Iterator Methods
    // These methods return iterators that automatically handle pagination

    // Paginated iterator for tickers
    PaginatedIterator<Ticker>
    list_tickers_iter(const std::optional<std::string> &ticker = std::nullopt,
                      const std::optional<std::string> &ticker_lt = std::nullopt,
                      const std::optional<std::string> &ticker_lte = std::nullopt,
                      const std::optional<std::string> &ticker_gt = std::nullopt,
                      const std::optional<std::string> &ticker_gte = std::nullopt,
                      const std::optional<std::string> &type = std::nullopt,
                      const std::optional<std::string> &market = std::nullopt,
                      const std::optional<std::string> &exchange = std::nullopt,
                      std::optional<int> cusip = std::nullopt,
                      std::optional<int> cik = std::nullopt,
                      const std::optional<std::string> &date = std::nullopt,
                      std::optional<bool> active = std::nullopt,
                      const std::optional<std::string> &search = std::nullopt,
                      const std::optional<std::string> &sort = std::nullopt,
                      const std::optional<std::string> &order = std::nullopt);

    // Paginated iterator for aggregates
    PaginatedIterator<Agg> list_aggs_iter(const std::string &ticker, int multiplier,
                                          const std::string &timespan, const std::string &from,
                                          const std::string &to,
                                          std::optional<bool> adjusted = std::nullopt,
                                          const std::optional<std::string> &sort = std::nullopt);

    // Paginated iterator for trades
    PaginatedIterator<Trade>
    list_trades_iter(const std::string &ticker,
                     const std::optional<std::string> &timestamp = std::nullopt,
                     const std::optional<std::string> &timestamp_lt = std::nullopt,
                     const std::optional<std::string> &timestamp_lte = std::nullopt,
                     const std::optional<std::string> &timestamp_gt = std::nullopt,
                     const std::optional<std::string> &timestamp_gte = std::nullopt,
                     const std::optional<std::string> &sort = std::nullopt,
                     const std::optional<std::string> &order = std::nullopt);

    // Paginated iterator for quotes
    PaginatedIterator<Quote>
    list_quotes_iter(const std::string &ticker,
                     const std::optional<std::string> &timestamp = std::nullopt,
                     const std::optional<std::string> &timestamp_lt = std::nullopt,
                     const std::optional<std::string> &timestamp_lte = std::nullopt,
                     const std::optional<std::string> &timestamp_gt = std::nullopt,
                     const std::optional<std::string> &timestamp_gte = std::nullopt,
                     const std::optional<std::string> &sort = std::nullopt,
                     const std::optional<std::string> &order = std::nullopt);

private:
    core::HttpResponse send_request(core::HttpMethod method, const std::string &path,
                                    const std::map<std::string, std::string> &params = {},
                                    const std::optional<RequestOptions> &options = std::nullopt);

    std::string build_url(const std::string &path,
                          const std::map<std::string, std::string> &params = {},
                          const std::optional<RequestOptions> &options = std::nullopt) const;
    std::map<std::string, std::string>
    build_headers(const std::optional<RequestOptions> &options = std::nullopt) const;

    core::ClientConfig config_;
    std::shared_ptr<core::IHttpTransport> transport_;
    std::shared_ptr<core::JsonCodec> json_codec_;
};

} // namespace massive::rest

#pragma once

#include <optional>
#include <string>
#include <vector>
#include <cstdint>

namespace massive::rest {

// Market Holiday
struct MarketHoliday {
    std::optional<std::string> exchange;
    std::optional<std::string> name;
    std::optional<std::string> date;
    std::optional<std::string> status;
};

// Market Status
struct MarketStatus {
    std::optional<std::string> market;
    std::optional<std::string> server_time;
    std::optional<std::string> exchanges;
    std::optional<std::string> currencies;
    std::optional<std::string> early_opens;
    std::optional<std::string> open_holidays;
};

// Ticker
struct Ticker {
    std::optional<bool> active;
    std::optional<std::string> cik;
    std::optional<std::string> composite_figi;
    std::optional<std::string> currency_name;
    std::optional<std::string> currency_symbol;
    std::optional<std::string> base_currency_symbol;
    std::optional<std::string> base_currency_name;
    std::optional<std::string> delisted_utc;
    std::optional<std::string> last_updated_utc;
    std::optional<std::string> locale;
    std::optional<std::string> market;
    std::optional<std::string> name;
    std::optional<std::string> primary_exchange;
    std::optional<std::string> share_class_figi;
    std::optional<std::string> ticker;
    std::optional<std::string> type;
    std::optional<std::string> source_feed;
};

// Company Address
struct CompanyAddress {
    std::optional<std::string> address1;
    std::optional<std::string> address2;
    std::optional<std::string> city;
    std::optional<std::string> state;
    std::optional<std::string> country;
    std::optional<std::string> postal_code;
};

// Branding
struct Branding {
    std::optional<std::string> icon_url;
    std::optional<std::string> logo_url;
    std::optional<std::string> accent_color;
    std::optional<std::string> light_color;
    std::optional<std::string> dark_color;
};

// Ticker Details
struct TickerDetails {
    std::optional<bool> active;
    std::optional<CompanyAddress> address;
    std::optional<Branding> branding;
    std::optional<std::string> cik;
    std::optional<std::string> composite_figi;
    std::optional<std::string> currency_name;
    std::optional<std::string> currency_symbol;
    std::optional<std::string> base_currency_name;
    std::optional<std::string> base_currency_symbol;
    std::optional<std::string> delisted_utc;
    std::optional<std::string> description;
    std::optional<std::string> ticker_root;
    std::optional<std::string> ticker_suffix;
    std::optional<std::string> homepage_url;
    std::optional<std::string> list_date;
    std::optional<std::string> locale;
    std::optional<std::string> market;
    std::optional<double> market_cap;
    std::optional<std::string> name;
    std::optional<std::string> phone_number;
    std::optional<std::string> primary_exchange;
    std::optional<std::string> share_class_figi;
    std::optional<std::int64_t> share_class_shares_outstanding;
    std::optional<std::string> sic_code;
    std::optional<std::string> sic_description;
    std::optional<std::string> ticker;
    std::optional<std::int64_t> total_employees;
    std::optional<std::string> type;
    std::optional<std::int64_t> weighted_shares_outstanding;
};

// Split
struct Split {
    std::optional<std::int64_t> id;
    std::optional<std::string> execution_date;
    std::optional<std::int64_t> split_from;
    std::optional<std::int64_t> split_to;
    std::optional<std::string> ticker;
};

// Dividend
struct Dividend {
    std::optional<std::int64_t> id;
    std::optional<double> cash_amount;
    std::optional<std::string> currency;
    std::optional<std::string> declaration_date;
    std::optional<std::string> dividend_type;
    std::optional<std::string> ex_dividend_date;
    std::optional<int> frequency;
    std::optional<std::string> pay_date;
    std::optional<std::string> record_date;
    std::optional<std::string> ticker;
};

// Condition
struct Condition {
    std::optional<std::int64_t> id;
    std::optional<std::string> asset_class;
    std::optional<std::string> data_type;
    std::optional<std::string> description;
    std::optional<std::string> sip;
};

// Exchange
struct Exchange {
    std::optional<std::string> id;
    std::optional<std::string> type;
    std::optional<std::string> market;
    std::optional<std::string> mic;
    std::optional<std::string> name;
    std::optional<std::string> tape;
};

// Options Contract
struct OptionsContract {
    std::optional<std::string> cfi;
    std::optional<std::string> contract_type;
    std::optional<std::string> exercise_style;
    std::optional<std::string> expiration_date;
    std::optional<std::string> primary_exchange;
    std::optional<double> shares_per_contract;
    std::optional<double> strike_price;
    std::optional<std::string> ticker;
    std::optional<std::string> underlying_ticker;
};

// Publisher (for news)
struct Publisher {
    std::optional<std::string> name;
    std::optional<std::string> homepage_url;
    std::optional<std::string> logo_url;
    std::optional<std::string> favicon_url;
};

// Ticker News
struct TickerNews {
    std::optional<std::string> id;
    std::optional<std::string> publisher_name;
    std::optional<std::string> title;
    std::optional<std::string> author;
    std::optional<std::string> published_utc;
    std::optional<std::string> article_url;
    std::optional<std::vector<std::string>> tickers;
    std::optional<std::string> amp_url;
    std::optional<std::string> image_url;
    std::optional<std::string> description;
    std::optional<std::vector<std::string>> keywords;
};

// Ticker Types
struct TickerTypes {
    std::optional<std::string> asset_class;
    std::optional<std::string> code;
    std::optional<std::string> description;
    std::optional<bool> locale;
};

// Related Company
struct RelatedCompany {
    std::optional<std::string> ticker;
    std::optional<std::string> name;
    std::optional<std::string> market;
    std::optional<std::string> locale;
    std::optional<std::string> primary_exchange;
    std::optional<std::string> type;
    std::optional<bool> active;
    std::optional<std::string> currency_name;
    std::optional<std::string> currency_symbol;
    std::optional<std::string> base_currency_name;
    std::optional<std::string> base_currency_symbol;
};

// Short Interest
struct ShortInterest {
    std::optional<std::string> ticker;
    std::optional<std::string> settlement_date;
    std::optional<std::int64_t> current_short_interest;
    std::optional<std::int64_t> previous_short_interest;
    std::optional<double> change;
    std::optional<double> change_percent;
    std::optional<std::int64_t> day_to_cover;
    std::optional<std::string> name;
};

// Short Volume
struct ShortVolume {
    std::optional<std::string> ticker;
    std::optional<std::string> date;
    std::optional<std::int64_t> short_volume;
    std::optional<std::int64_t> total_volume;
    std::optional<double> short_volume_ratio;
};

// Ticker Change
struct TickerChange {
    std::optional<std::string> ticker;
};

// Ticker Change Event
struct TickerChangeEvent {
    std::optional<std::string> type;
    std::optional<std::string> date;
    std::optional<TickerChange> ticker_change;
};

// Ticker Change Results
struct TickerChangeResults {
    std::optional<std::string> name;
    std::optional<std::string> composite_figi;
    std::optional<std::string> cik;
    std::optional<std::vector<TickerChangeEvent>> events;
};

// IPO
struct IPO {
    std::optional<std::string> ticker;
    std::optional<std::string> name;
    std::optional<std::string> market;
    std::optional<std::string> locale;
    std::optional<std::string> primary_exchange;
    std::optional<std::string> type;
    std::optional<std::string> currency_name;
    std::optional<std::string> currency_symbol;
    std::optional<std::string> cik;
    std::optional<std::string> composite_figi;
    std::optional<std::string> share_class_figi;
    std::optional<std::string> description;
    std::optional<std::string> ticker_root;
    std::optional<std::string> ticker_suffix;
    std::optional<std::string> homepage_url;
    std::optional<std::string> total_employees;
    std::optional<std::string> list_date;
    std::optional<std::string> branding_icon_url;
    std::optional<std::string> branding_logo_url;
    std::optional<std::string> branding_logo_url_400;
    std::optional<std::string> branding_logo_url_light;
    std::optional<std::string> branding_logo_url_light_400;
    std::optional<std::string> branding_dark_icon_url;
    std::optional<std::string> branding_dark_logo_url;
    std::optional<std::string> branding_dark_logo_url_400;
    std::optional<std::string> share_class_shares_outstanding;
    std::optional<std::string> weighted_shares_outstanding;
    std::optional<std::int64_t> round_lot;
    std::optional<std::string> sic_code;
    std::optional<std::string> sic_description;
    std::optional<std::string> ticker_cik;
    std::optional<std::string> exchange;
    std::optional<std::string> date;
    std::optional<std::string> exchange_symbol;
    std::optional<std::string> name_of_issuer;
    std::optional<std::string> shares;
    std::optional<double> price;
    std::optional<std::string> currency;
    std::optional<std::string> status;
};

}  // namespace massive::rest


#include "massive/rest/client.hpp"
#include "massive/core/config.hpp"
#include "massive/core/http/beast_transport.hpp"
#include <iostream>

using namespace massive;

int main() {
    try {
        // Create client from environment variables
        auto config = core::ClientConfig::FromEnv();
        auto transport = core::make_beast_transport();
        rest::RESTClient client(config, transport);

        // Get aggregates (bars) for AAPL
        std::cout << "Fetching aggregates for AAPL..." << std::endl;
        auto aggs = client.list_aggs("AAPL", 1, "day", "2024-01-01", "2024-01-31");

        std::cout << "Found " << aggs.size() << " aggregates" << std::endl;
        for (const auto& agg : aggs) {
            if (agg.open && agg.close) {
                std::cout << "Open: " << *agg.open 
                          << ", Close: " << *agg.close;
                if (agg.volume) {
                    std::cout << ", Volume: " << *agg.volume;
                }
                std::cout << std::endl;
            }
        }

        // Get last trade
        std::cout << "\nFetching last trade for AAPL..." << std::endl;
        auto last_trade = client.get_last_trade("AAPL");
        if (last_trade.price) {
            std::cout << "Last trade price: " << *last_trade.price;
            if (last_trade.size) {
                std::cout << ", Size: " << *last_trade.size;
            }
            std::cout << std::endl;
        }

        // Get last quote
        std::cout << "\nFetching last quote for AAPL..." << std::endl;
        auto last_quote = client.get_last_quote("AAPL");
        if (last_quote.bid && last_quote.ask) {
            std::cout << "Bid: " << *last_quote.bid 
                      << ", Ask: " << *last_quote.ask << std::endl;
        }

        // Get previous close
        std::cout << "\nFetching previous close for AAPL..." << std::endl;
        auto prev_close = client.get_previous_close_agg("AAPL");
        if (prev_close.close) {
            std::cout << "Previous close: " << *prev_close.close;
            if (prev_close.volume) {
                std::cout << ", Volume: " << *prev_close.volume;
            }
            std::cout << std::endl;
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}


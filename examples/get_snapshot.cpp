#include "massive/rest/client.hpp"
#include "massive/rest/models/snapshot.hpp"
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

        // Get snapshot for a specific ticker
        std::cout << "Fetching snapshot for AAPL..." << std::endl;
        auto snapshot = client.get_snapshot_ticker(
            rest::SnapshotMarketType::Stocks,
            "AAPL"
        );

        if (snapshot.ticker) {
            std::cout << "Ticker: " << *snapshot.ticker << std::endl;
        }

        if (snapshot.day && snapshot.day->close) {
            std::cout << "Day Close: " << *snapshot.day->close;
            if (snapshot.day->volume) {
                std::cout << ", Volume: " << *snapshot.day->volume;
            }
            std::cout << std::endl;
        }

        if (snapshot.todays_change && snapshot.todays_change_percent) {
            std::cout << "Today's Change: " << *snapshot.todays_change 
                      << " (" << *snapshot.todays_change_percent << "%)" << std::endl;
        }

        if (snapshot.last_trade && snapshot.last_trade->price) {
            std::cout << "Last Trade: " << *snapshot.last_trade->price << std::endl;
        }

        if (snapshot.last_quote && snapshot.last_quote->bid && snapshot.last_quote->ask) {
            std::cout << "Bid: " << *snapshot.last_quote->bid 
                      << ", Ask: " << *snapshot.last_quote->ask << std::endl;
        }

        // Get top gainers
        std::cout << "\nFetching top gainers..." << std::endl;
        auto gainers = client.get_snapshot_direction(
            rest::SnapshotMarketType::Stocks,
            rest::Direction::Gainers
        );

        std::cout << "Found " << gainers.size() << " top gainers" << std::endl;
        for (size_t i = 0; i < std::min(gainers.size(), size_t(5)); ++i) {
            const auto& g = gainers[i];
            if (g.ticker && g.todays_change_percent) {
                std::cout << "  " << *g.ticker << ": " 
                          << *g.todays_change_percent << "%" << std::endl;
            }
        }

        // Get snapshots for multiple tickers
        std::cout << "\nFetching snapshots for AAPL, MSFT, GOOGL..." << std::endl;
        auto snapshots = client.get_snapshot_all(
            rest::SnapshotMarketType::Stocks,
            {"AAPL", "MSFT", "GOOGL"}
        );

        std::cout << "Found " << snapshots.size() << " snapshots" << std::endl;
        for (const auto& s : snapshots) {
            if (s.ticker && s.day && s.day->close) {
                std::cout << "  " << *s.ticker << ": $" << *s.day->close;
                if (s.todays_change_percent) {
                    std::cout << " (" << *s.todays_change_percent << "%)";
                }
                std::cout << std::endl;
            }
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}


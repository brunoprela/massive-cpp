#include "massive/websocket/client.hpp"
#include "massive/websocket/models.hpp"
#include <iostream>
#include <thread>
#include <chrono>

using namespace massive::websocket;

int main() {
    try {
        // Get API key from environment
        const char* api_key = std::getenv("MASSIVE_API_KEY");
        if (!api_key) {
            std::cerr << "Error: MASSIVE_API_KEY environment variable not set" << std::endl;
            return 1;
        }

        // Create WebSocket client
        WebSocketClient client(
            api_key,
            Feed::RealTime,
            Market::Stocks,
            false,  // not raw mode
            true    // verbose
        );

        // Define message handler
        auto handler = [](const std::vector<WebSocketMessage>& messages) {
            for (const auto& msg : messages) {
                if (std::holds_alternative<EquityTrade>(msg)) {
                    const auto& trade = std::get<EquityTrade>(msg);
                    if (trade.symbol && trade.price && trade.size) {
                        std::cout << "Trade: " << *trade.symbol 
                                  << " @ $" << *trade.price 
                                  << " x " << *trade.size << std::endl;
                    }
                } else if (std::holds_alternative<EquityQuote>(msg)) {
                    const auto& quote = std::get<EquityQuote>(msg);
                    if (quote.symbol && quote.bid_price && quote.ask_price) {
                        std::cout << "Quote: " << *quote.symbol 
                                  << " Bid: $" << *quote.bid_price 
                                  << " Ask: $" << *quote.ask_price << std::endl;
                    }
                } else if (std::holds_alternative<EquityAgg>(msg)) {
                    const auto& agg = std::get<EquityAgg>(msg);
                    if (agg.symbol && agg.close) {
                        std::cout << "Agg: " << *agg.symbol 
                                  << " Close: $" << *agg.close << std::endl;
                    }
                }
            }
        };

        // Connect and subscribe
        std::cout << "Connecting to WebSocket..." << std::endl;
        client.connect(handler);

        // Subscribe to trades and quotes for AAPL
        std::cout << "Subscribing to T.AAPL (trades) and Q.AAPL (quotes)..." << std::endl;
        client.subscribe({"T.AAPL", "Q.AAPL"});

        // Run for 30 seconds
        std::cout << "Listening for messages (30 seconds)..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(30));

        // Unsubscribe and close
        std::cout << "Unsubscribing and closing..." << std::endl;
        client.unsubscribe_all();
        client.close();

        std::cout << "Done!" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}


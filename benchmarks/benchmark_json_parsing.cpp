#include <benchmark/benchmark.h>
#include "massive/core/json.hpp"
#include <simdjson/ondemand.h>
#include <string>

static void BM_JsonParsing(benchmark::State& state) {
    std::string json_data = R"({"ticker":"AAPL","results":[{"o":150.0,"h":155.0,"l":149.0,"c":154.0,"v":1000000}]})";
    simdjson::padded_string padded(json_data);
    
    for (auto _ : state) {
        simdjson::ondemand::parser parser;
        auto doc = parser.iterate(padded);
        benchmark::DoNotOptimize(doc);
    }
}

BENCHMARK(BM_JsonParsing);

BENCHMARK_MAIN();


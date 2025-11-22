# Benchmark infrastructure (optional)

option(MASSIVE_BUILD_BENCHMARKS "Build benchmarks" OFF)

if(MASSIVE_BUILD_BENCHMARKS)
    include(FetchContent)
    
    FetchContent_Declare(
        googlebenchmark
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG v1.8.3
    )
    
    FetchContent_MakeAvailable(googlebenchmark)
    
    # Example benchmark - can be expanded
    add_executable(benchmark_json_parsing
        benchmark_json_parsing.cpp
    )
    target_link_libraries(benchmark_json_parsing PRIVATE 
        massive::core 
        benchmark::benchmark
        benchmark::benchmark_main
    )
endif()


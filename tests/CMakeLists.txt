# Test infrastructure for massive-cpp

# Find Catch2 if tests are enabled
if(MASSIVE_BUILD_TESTS)
    include(FetchContent)
    
    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.5.2
    )
    
    FetchContent_MakeAvailable(Catch2)
    
    # Example test - can be expanded
    add_executable(massive_core_tests
        unit/test_config.cpp
    )
    target_link_libraries(massive_core_tests PRIVATE massive::core Catch2::Catch2WithMain)
    
    add_test(NAME massive_core_tests COMMAND massive_core_tests)
    
    # Add more tests here as they are created
endif()


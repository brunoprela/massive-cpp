cmake_minimum_required(VERSION 3.20)
project(massive-cpp VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(MASSIVE_ENABLE_WARNINGS "Enable recommended warnings" ON)
option(MASSIVE_VENDOR_DEPS "Fetch required third-party dependencies" ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

if(MASSIVE_ENABLE_WARNINGS)
    include(CheckCXXCompilerFlag)
    set(_warning_flags
        -Wall
        -Wextra
        -Wshadow
        -Wnon-virtual-dtor
        -Wold-style-cast
        -Wcast-align
        -Wunused
        -Woverloaded-virtual
        -pedantic
        -Wconversion
        -Wsign-conversion
        -Wnull-dereference
        -Wdouble-promotion)
    foreach(flag IN LISTS _warning_flags)
        check_cxx_compiler_flag(${flag} HAS_${flag})
        if(HAS_${flag})
            add_compile_options(${flag})
        endif()
    endforeach()
endif()

set(MASSIVE_BOOST_INCLUDE_DIR "")
set(MASSIVE_SIMDJSON_TARGET "")

if(MASSIVE_VENDOR_DEPS)
    include(FetchContent)

    set(BOOST_ENABLE_CMAKE ON CACHE BOOL "" FORCE)
    FetchContent_Declare(
        boost
        URL https://archives.boost.io/release/1.86.0/source/boost_1_86_0.tar.bz2
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE)
    FetchContent_MakeAvailable(boost)
    set(MASSIVE_BOOST_INCLUDE_DIR ${boost_SOURCE_DIR})
    set(BOOST_URL_SOURCES
        ${boost_SOURCE_DIR}/libs/url/src/authority_view.cpp
        ${boost_SOURCE_DIR}/libs/url/src/decode_view.cpp
        ${boost_SOURCE_DIR}/libs/url/src/encoding_opts.cpp
        ${boost_SOURCE_DIR}/libs/url/src/error.cpp
        ${boost_SOURCE_DIR}/libs/url/src/ipv4_address.cpp
        ${boost_SOURCE_DIR}/libs/url/src/ipv6_address.cpp
        ${boost_SOURCE_DIR}/libs/url/src/params_base.cpp
        ${boost_SOURCE_DIR}/libs/url/src/params_encoded_base.cpp
        ${boost_SOURCE_DIR}/libs/url/src/params_encoded_ref.cpp
        ${boost_SOURCE_DIR}/libs/url/src/params_encoded_view.cpp
        ${boost_SOURCE_DIR}/libs/url/src/params_ref.cpp
        ${boost_SOURCE_DIR}/libs/url/src/params_view.cpp
        ${boost_SOURCE_DIR}/libs/url/src/parse.cpp
        ${boost_SOURCE_DIR}/libs/url/src/parse_path.cpp
        ${boost_SOURCE_DIR}/libs/url/src/parse_query.cpp
        ${boost_SOURCE_DIR}/libs/url/src/pct_string_view.cpp
        ${boost_SOURCE_DIR}/libs/url/src/scheme.cpp
        ${boost_SOURCE_DIR}/libs/url/src/segments_base.cpp
        ${boost_SOURCE_DIR}/libs/url/src/segments_encoded_base.cpp
        ${boost_SOURCE_DIR}/libs/url/src/segments_encoded_ref.cpp
        ${boost_SOURCE_DIR}/libs/url/src/segments_encoded_view.cpp
        ${boost_SOURCE_DIR}/libs/url/src/segments_ref.cpp
        ${boost_SOURCE_DIR}/libs/url/src/segments_view.cpp
        ${boost_SOURCE_DIR}/libs/url/src/static_url.cpp
        ${boost_SOURCE_DIR}/libs/url/src/url.cpp
        ${boost_SOURCE_DIR}/libs/url/src/url_base.cpp
        ${boost_SOURCE_DIR}/libs/url/src/url_view.cpp
        ${boost_SOURCE_DIR}/libs/url/src/url_view_base.cpp
        ${boost_SOURCE_DIR}/libs/url/src/detail/any_params_iter.cpp
        ${boost_SOURCE_DIR}/libs/url/src/detail/any_segments_iter.cpp
        ${boost_SOURCE_DIR}/libs/url/src/detail/decode.cpp
        ${boost_SOURCE_DIR}/libs/url/src/detail/except.cpp
        ${boost_SOURCE_DIR}/libs/url/src/detail/format_args.cpp
        ${boost_SOURCE_DIR}/libs/url/src/detail/normalize.cpp
        ${boost_SOURCE_DIR}/libs/url/src/detail/pattern.cpp
        ${boost_SOURCE_DIR}/libs/url/src/detail/pct_format.cpp
        ${boost_SOURCE_DIR}/libs/url/src/detail/params_iter_impl.cpp
        ${boost_SOURCE_DIR}/libs/url/src/detail/replacement_field_rule.cpp
        ${boost_SOURCE_DIR}/libs/url/src/detail/segments_iter_impl.cpp
        ${boost_SOURCE_DIR}/libs/url/src/detail/url_impl.cpp
        ${boost_SOURCE_DIR}/libs/url/src/detail/vformat.cpp
        ${boost_SOURCE_DIR}/libs/url/src/grammar/ci_string.cpp
        ${boost_SOURCE_DIR}/libs/url/src/grammar/dec_octet_rule.cpp
        ${boost_SOURCE_DIR}/libs/url/src/grammar/delim_rule.cpp
        ${boost_SOURCE_DIR}/libs/url/src/grammar/detail/recycled.cpp
        ${boost_SOURCE_DIR}/libs/url/src/grammar/error.cpp
        ${boost_SOURCE_DIR}/libs/url/src/grammar/literal_rule.cpp
        ${boost_SOURCE_DIR}/libs/url/src/grammar/string_view_base.cpp
        ${boost_SOURCE_DIR}/libs/url/src/rfc/absolute_uri_rule.cpp
        ${boost_SOURCE_DIR}/libs/url/src/rfc/authority_rule.cpp
        ${boost_SOURCE_DIR}/libs/url/src/rfc/ipv4_address_rule.cpp
        ${boost_SOURCE_DIR}/libs/url/src/rfc/ipv6_address_rule.cpp
        ${boost_SOURCE_DIR}/libs/url/src/rfc/origin_form_rule.cpp
        ${boost_SOURCE_DIR}/libs/url/src/rfc/query_rule.cpp
        ${boost_SOURCE_DIR}/libs/url/src/rfc/relative_ref_rule.cpp
        ${boost_SOURCE_DIR}/libs/url/src/rfc/uri_reference_rule.cpp
        ${boost_SOURCE_DIR}/libs/url/src/rfc/uri_rule.cpp
        ${boost_SOURCE_DIR}/libs/url/src/rfc/detail/h16_rule.cpp
        ${boost_SOURCE_DIR}/libs/url/src/rfc/detail/host_rule.cpp
        ${boost_SOURCE_DIR}/libs/url/src/rfc/detail/hier_part_rule.cpp
        ${boost_SOURCE_DIR}/libs/url/src/rfc/detail/ipv6_addrz_rule.cpp
        ${boost_SOURCE_DIR}/libs/url/src/rfc/detail/ipvfuture_rule.cpp
        ${boost_SOURCE_DIR}/libs/url/src/rfc/detail/ip_literal_rule.cpp
        ${boost_SOURCE_DIR}/libs/url/src/rfc/detail/port_rule.cpp
        ${boost_SOURCE_DIR}/libs/url/src/rfc/detail/relative_part_rule.cpp
        ${boost_SOURCE_DIR}/libs/url/src/rfc/detail/scheme_rule.cpp
        ${boost_SOURCE_DIR}/libs/url/src/rfc/detail/userinfo_rule.cpp
        ${boost_SOURCE_DIR}/libs/url/src/rfc/uri_rule.cpp
        ${boost_SOURCE_DIR}/libs/url/src/segments_view.cpp)

    FetchContent_Declare(
        simdjson
        GIT_REPOSITORY https://github.com/simdjson/simdjson.git
        GIT_TAG v3.9.2)
    FetchContent_MakeAvailable(simdjson)
    set(MASSIVE_SIMDJSON_TARGET simdjson)
    set(MASSIVE_SIMDJSON_INCLUDE_DIR ${simdjson_SOURCE_DIR}/include)
else()
    find_package(Boost REQUIRED)
    find_package(simdjson CONFIG REQUIRED)
    set(MASSIVE_BOOST_INCLUDE_DIR ${Boost_INCLUDE_DIRS})
    set(MASSIVE_SIMDJSON_TARGET simdjson::simdjson)
    get_target_property(MASSIVE_SIMDJSON_INCLUDE_DIR ${MASSIVE_SIMDJSON_TARGET} INTERFACE_INCLUDE_DIRECTORIES)
    if(NOT MASSIVE_SIMDJSON_INCLUDE_DIR)
        get_target_property(MASSIVE_SIMDJSON_INCLUDE_DIR ${MASSIVE_SIMDJSON_TARGET} INTERFACE_SYSTEM_INCLUDE_DIRECTORIES)
    endif()
    set(BOOST_URL_SOURCES)
endif()

find_package(OpenSSL REQUIRED)

# Core library
add_library(massive_core
    src/massive/core/config.cpp
    src/massive/core/http_transport.cpp
    src/massive/core/http/beast_transport.cpp
    src/massive/core/json.cpp
    src/massive/core/dotenv.cpp
    src/massive/core/logging.cpp
    ${BOOST_URL_SOURCES})
target_include_directories(massive_core
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)
if(MASSIVE_VENDOR_DEPS)
    if(MASSIVE_BOOST_INCLUDE_DIR)
        target_include_directories(massive_core SYSTEM PRIVATE $<BUILD_INTERFACE:${MASSIVE_BOOST_INCLUDE_DIR}>)
    endif()
    if(MASSIVE_SIMDJSON_INCLUDE_DIR)
        target_include_directories(massive_core SYSTEM PRIVATE $<BUILD_INTERFACE:${MASSIVE_SIMDJSON_INCLUDE_DIR}>)
    endif()
endif()
target_compile_features(massive_core PUBLIC cxx_std_20)
target_link_libraries(massive_core PUBLIC ${MASSIVE_SIMDJSON_TARGET} OpenSSL::SSL OpenSSL::Crypto)

add_library(massive::core ALIAS massive_core)

# REST client library
add_library(massive_rest
    src/massive/rest/client_base.cpp
    src/massive/rest/aggs_client.cpp
    src/massive/rest/trades_client.cpp
    src/massive/rest/quotes_client.cpp
    src/massive/rest/snapshot_client.cpp
    src/massive/rest/reference_client.cpp
    src/massive/rest/summaries_client.cpp
    src/massive/rest/indicators_client.cpp
    src/massive/rest/futures_client.cpp
    src/massive/rest/financials_client.cpp
    src/massive/rest/benzinga_client.cpp
    src/massive/rest/economy_client.cpp
    src/massive/rest/etf_client.cpp
    src/massive/rest/tmx_client.cpp
    src/massive/rest/vx_client.cpp
    src/massive/rest/conversion_client.cpp
    src/massive/rest/pagination.cpp
    src/massive/rest/pagination_iterators.cpp
    src/massive/rest/request_options.cpp)
target_link_libraries(massive_rest PUBLIC massive::core)
if(MASSIVE_VENDOR_DEPS)
    target_include_directories(massive_rest PUBLIC $<INSTALL_INTERFACE:include>)
    if(MASSIVE_BOOST_INCLUDE_DIR)
        target_include_directories(massive_rest SYSTEM PRIVATE $<BUILD_INTERFACE:${MASSIVE_BOOST_INCLUDE_DIR}>)
    endif()
    if(MASSIVE_SIMDJSON_INCLUDE_DIR)
        target_include_directories(massive_rest SYSTEM PRIVATE $<BUILD_INTERFACE:${MASSIVE_SIMDJSON_INCLUDE_DIR}>)
    endif()
else()
    target_link_libraries(massive_rest PUBLIC Boost::boost)
endif()
target_include_directories(massive_rest
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)
target_compile_features(massive_rest PUBLIC cxx_std_20)

add_library(massive::rest ALIAS massive_rest)

# WebSocket client library
add_library(massive_websocket
    src/massive/websocket/client.cpp)
target_link_libraries(massive_websocket PUBLIC massive::core)
if(MASSIVE_VENDOR_DEPS)
    target_include_directories(massive_websocket PUBLIC $<INSTALL_INTERFACE:include>)
    if(MASSIVE_BOOST_INCLUDE_DIR)
        target_include_directories(massive_websocket SYSTEM PRIVATE $<BUILD_INTERFACE:${MASSIVE_BOOST_INCLUDE_DIR}>)
    endif()
    if(MASSIVE_SIMDJSON_INCLUDE_DIR)
        target_include_directories(massive_websocket SYSTEM PRIVATE $<BUILD_INTERFACE:${MASSIVE_SIMDJSON_INCLUDE_DIR}>)
    endif()
else()
    target_link_libraries(massive_websocket PUBLIC Boost::boost)
endif()
target_include_directories(massive_websocket
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)
target_compile_features(massive_websocket PUBLIC cxx_std_20)
target_link_libraries(massive_websocket PUBLIC ${MASSIVE_SIMDJSON_TARGET} OpenSSL::SSL OpenSSL::Crypto)

add_library(massive::websocket ALIAS massive_websocket)

# Examples (optional)
option(MASSIVE_BUILD_EXAMPLES "Build example programs" OFF)
if(MASSIVE_BUILD_EXAMPLES)
    add_executable(massive_example_get_aggs examples/get_aggs.cpp)
    target_link_libraries(massive_example_get_aggs PRIVATE massive::rest)

    add_executable(massive_example_get_snapshot examples/get_snapshot.cpp)
    target_link_libraries(massive_example_get_snapshot PRIVATE massive::rest)

    add_executable(massive_example_websocket examples/websocket_example.cpp)
    target_link_libraries(massive_example_websocket PRIVATE massive::websocket)
endif()

# Installation support
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# Install libraries
install(TARGETS massive_core massive_rest massive_websocket
    EXPORT massive-cpp-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Install headers
install(DIRECTORY include/massive
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING PATTERN "*.hpp"
)

# Install CMake config files
install(EXPORT massive-cpp-targets
    FILE massive-cpp-targets.cmake
    NAMESPACE massive::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/massive-cpp
)

# Generate and install package config
configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/massive-cpp-config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/massive-cpp-config.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/massive-cpp
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/massive-cpp-config-version.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/massive-cpp-config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/massive-cpp-config-version.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/massive-cpp
)

# Install LICENSE
install(FILES LICENSE
    DESTINATION ${CMAKE_INSTALL_DOCDIR}
)



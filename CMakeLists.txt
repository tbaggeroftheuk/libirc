cmake_minimum_required(VERSION 3.20)

project(libirc)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

FetchContent_Declare(
    asio
    GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
    GIT_TAG asio-1-30-2
)

FetchContent_MakeAvailable(asio)

FetchContent_Declare(
    openssl
    GIT_REPOSITORY https://github.com/openssl/openssl.git
    GIT_TAG openssl-3.5.0
)

FetchContent_MakeAvailable(openssl)

file(GLOB SRC_FILES
    "src/*.cpp"
)

add_executable(libirc
    ${SRC_FILES}
)

target_compile_features(libirc PRIVATE cxx_std_20)

target_include_directories(libirc PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${asio_SOURCE_DIR}/asio/include
    ${openssl_SOURCE_DIR}/include
)

find_package(Threads REQUIRED)

target_link_libraries(libirc PRIVATE
    Threads::Threads
    ssl
    crypto
)

target_compile_definitions(libirc PRIVATE
    ASIO_USE_SSL
)
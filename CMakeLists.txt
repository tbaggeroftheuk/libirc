cmake_minimum_required(VERSION 3.20)

project(libirc)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB SRC_FILES
    "src/*.cpp"
)

add_executable(libirc
    ${SRC_FILES}
)

target_compile_features(libirc PRIVATE cxx_std_20)

target_include_directories(libirc
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include
)

find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(fmt CONFIG REQUIRED)


target_link_libraries(libirc PRIVATE
    Threads::Threads
    OpenSSL::SSL
    OpenSSL::Crypto
    fmt::fmt
)


target_compile_definitions(libirc PRIVATE
    ASIO_USE_SSL
)
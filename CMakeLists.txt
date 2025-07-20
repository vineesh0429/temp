cmake_minimum_required(VERSION 3.19)
project(modem_client CXX)

# Require C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Ensure pkg-config picks up /usr/local
if(NOT DEFINED ENV{PKG_CONFIG_PATH})
  set(ENV{PKG_CONFIG_PATH} "/usr/local/lib/pkgconfig")
else()
  set(ENV{PKG_CONFIG_PATH} "/usr/local/lib/pkgconfig:$ENV{PKG_CONFIG_PATH}")
endif()

find_package(PkgConfig REQUIRED)

# Find the C++ bindings via pkg-config
pkg_check_modules(LIBYANG_CPP REQUIRED libyang-cpp)
pkg_check_modules(SYSREPO_CPP REQUIRED sysrepo-cpp)

# Your executable
add_executable(modem_client test_client.cpp)

# Includes from pkg-config
target_include_directories(modem_client PRIVATE
    ${LIBYANG_CPP_INCLUDE_DIRS}
    ${SYSREPO_CPP_INCLUDE_DIRS}
)

# Link flags from pkg-config
target_link_libraries(modem_client PRIVATE
    ${LIBYANG_CPP_LIBRARIES}
    ${SYSREPO_CPP_LIBRARIES}
)

# At runtime, have the loader look in /usr/local/lib
set_target_properties(modem_client PROPERTIES
    INSTALL_RPATH "/usr/local/lib"
)


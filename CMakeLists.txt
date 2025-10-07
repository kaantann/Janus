cmake_minimum_required(VERSION 3.10)
project(Janus)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Library Setup using Submodules from 'external' directory ---
set(BUILD_EXAMPLES OFF CACHE BOOL "Build examples")
set(SUPPORT_SSH OFF CACHE BOOL "Build with SSH support")

add_subdirectory(external/raylib)
add_subdirectory(external/enet)
add_subdirectory(external/glm)


# --- Shared Code ---
include_directories(${CMAKE_SOURCE_DIR}/src/shared)


# --- Server Executable ---
add_executable(Server src/server/main.cpp)

# THIS IS THE FIX FOR THE SERVER: Explicitly tell it where all headers are.
target_include_directories(Server PRIVATE
    ${CMAKE_SOURCE_DIR}/external/glm
    ${CMAKE_SOURCE_DIR}/external/enet/include
)

target_link_libraries(Server enet glm)


# --- Client Executable ---
add_executable(Client
    src/client/main.cpp
    src/client/NetworkManager.cpp
    src/client/NetworkManager.h
)

# THIS IS THE FIX FOR THE CLIENT: Add ENet's header location to the list.
target_include_directories(Client PRIVATE
    ${CMAKE_SOURCE_DIR}/src/client
    ${CMAKE_SOURCE_DIR}/external/glm
    ${CMAKE_SOURCE_DIR}/external/enet/include
)

target_link_libraries(Client enet raylib glm)
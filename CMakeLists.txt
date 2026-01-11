cmake_minimum_required(VERSION 3.16)
project(RoChatPlus LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find Qt6 packages
find_package(Qt6 COMPONENTS
    Core
    Gui
    Widgets
    Network
    Sql
    Concurrent
    REQUIRED
)

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

# Source files
set(SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/chatwidget.cpp
    src/networkclient.cpp
    src/moderation/moderationengine.cpp
    src/moderation/linkvalidator.cpp
    include/types.h
    include/constants.h
    src/resources/resources.qrc
)

set(HEADERS
    src/mainwindow.h
    src/chatwidget.h
    src/networkclient.h
    src/moderation/moderationengine.h
    src/moderation/linkvalidator.h
    include/types.h
    include/constants.h
)

# Create executable
add_executable(RoChatPlus ${SOURCES} ${HEADERS})

# Link Qt6 libraries
target_link_libraries(RoChatPlus
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Network
    Qt6::Sql
    Qt6::Concurrent
)

# Platform-specific configuration
if(WIN32)
    set_target_properties(RoChatPlus PROPERTIES
        WIN32_EXECUTABLE ON
    )
endif()

# Enable testing
enable_testing()

# Add test subdirectory if tests exist
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/CMakeLists.txt")
    add_subdirectory(tests)
endif()

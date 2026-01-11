# RoChat+ - Copilot Development Instructions

## Project Overview

RoChat+ is a standalone Windows desktop application that provides an enhanced chat experience for Roblox players. Built with C++ and Qt, it replaces the default Roblox chat with a modern, feature-rich interface supporting multiple servers, images, links, and intelligent moderation.

## Technology Stack

- **Language**: C++17
- **GUI Framework**: Qt 6.x
- **Networking**: Qt Network (QWebSocket, QTcpSocket, QNetworkAccessManager)
- **Build System**: CMake 3.16+
- **IDE**: Visual Studio Code with C++ extensions or Visual Studio 2019+
- **Target Platform**: Windows 10/11 only

## Project Structure

```
src/
├── main.cpp                          # Application entry point
├── mainwindow.h / mainwindow.cpp     # Main application window
├── chatwidget.h / chatwidget.cpp     # Chat display and input widget
├── networkclient.h / networkclient.cpp # Network communication layer
├── moderation/
│   ├── moderationengine.h/.cpp       # Content moderation logic
│   ├── linkvalidator.h/.cpp          # Link validation and filtering
│   └── urlblacklist.h/.cpp           # Blacklist management
├── ui/
│   ├── chatwindow.ui                 # Chat window UI design
│   ├── settingsdialog.ui             # Settings dialog UI design
│   └── logindialog.ui                # Login dialog UI design
└── resources/
    └── resources.qrc                 # Qt resource file (images, icons, etc.)

include/
├── types.h                           # Shared type definitions
├── constants.h                       # Application constants
└── config.h                          # Configuration management

tests/
├── test_moderation.cpp               # Moderation system tests
├── test_network.cpp                  # Network layer tests
└── test_ui.cpp                       # UI component tests

build/                                # CMake build output (generated)
CMakeLists.txt                        # CMake build configuration
```

## Development Setup

### Prerequisites

1. **Visual Studio 2019+ or MinGW**: C++ compiler
2. **Qt 6.x SDK**: Download from [qt.io](https://www.qt.io/)
3. **CMake 3.16+**: Build system
4. **Git**: Version control
5. **VS Code Extensions** (optional):
   - C/C++ by Microsoft
   - CMake Tools
   - Qt Tools

### Environment Setup

1. Install Qt 6 to default location (C:\Qt on Windows)
2. Add Qt bin folder to PATH environment variable
3. Install Visual Studio Build Tools or MinGW compiler
4. Install CMake

## Build Instructions

### From Command Line

```bash
# Clone repository
git clone https://github.com/yourusername/RoChatPlus.git
cd RoChatPlus

# Create and enter build directory
mkdir build && cd build

# Generate build files
cmake .. -G "Visual Studio 16 2019"

# Build the application
cmake --build . --config Release

# Output will be in build/Release/RoChatPlus.exe
```

### From Visual Studio

1. Open Visual Studio
2. Select "Open a folder"
3. Choose the RoChatPlus directory
4. CMake will auto-configure
5. Select target and build

## Architecture Overview

### Core Components

1. **MainWindow (mainwindow.h/cpp)**
   - Application main window
   - Manages tabbed interface for multiple servers
   - Handles window events and lifecycle

2. **ChatWidget (chatwidget.h/cpp)**
   - Display area for messages
   - Message input field
   - User list and status indicators

3. **NetworkClient (networkclient.h/cpp)**
   - WebSocket/TCP connection management
   - Message sending and receiving
   - Reconnection handling
   - Data serialization/deserialization

4. **ModerationEngine (moderation/moderationengine.h/cpp)**
   - Link validation
   - Image content scanning
   - Suspicious content detection
   - Automatic filtering

5. **LinkValidator (moderation/linkvalidator.h/cpp)**
   - URL parsing and analysis
   - Blacklist checking
   - Domain reputation lookup
   - Return malicious link detection

## Coding Standards

### C++ Guidelines

- Use modern C++17 features
- Prefer standard library over manual memory management
- Use smart pointers (unique_ptr, shared_ptr)
- Const-correctness
- Meaningful variable and function names
- Comments for complex logic

### Qt Guidelines

- Use Qt signals/slots for event handling
- Inherit from appropriate Qt classes
- Use tr() for all user-visible strings
- Proper parent/child relationships for memory management
- Follow Qt naming conventions

## Security Practices

- Use HTTPS/WSS for all connections
- Validate all user input
- Implement rate limiting
- Use secure token authentication
- Never log sensitive user data
- Sanitize displayed content

## Tasks Checklist

- [ ] Project structure and build system configured
- [ ] Main window and tabbed interface created
- [ ] Chat widget and message display implemented
- [ ] Network communication layer established
- [ ] Message handling with image/link support
- [ ] Moderation engine and link validator
- [ ] User authentication system
- [ ] Settings and preferences dialog
- [ ] Installer creation
- [ ] Testing and optimization
- [ ] v1.0 Release

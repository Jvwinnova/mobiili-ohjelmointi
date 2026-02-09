cmake_minimum_required(VERSION 3.16)

project(untitled5 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find the required Qt6 modules
find_package(Qt6 REQUIRED COMPONENTS Quick Multimedia QuickControls2)

# Standard Qt project setup
qt_standard_project_setup(REQUIRES 6.8)

# Add the main executable
qt_add_executable(appuntitled5
    main.cpp
)

# Add QML module and sources
qt_add_qml_module(appuntitled5
    URI untitled5
    VERSION 1.0
    QML_FILES
        Main.qml
    SOURCES
        player.h
        player.cpp
    RESOURCES
        android/AndroidManifest.xml
        resources.qrc
)

# Set properties for macOS/iOS and Windows
set_target_properties(appuntitled5 PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# Link required Qt libraries
target_link_libraries(appuntitled5
    PRIVATE Qt6::Quick
            Qt6::Multimedia
            Qt6::QuickControls2
)

# Install rules
include(GNUInstallDirs)
install(TARGETS appuntitled5
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Android-specific settings
if(ANDROID)
    set_property(TARGET appuntitled5 APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
endif()

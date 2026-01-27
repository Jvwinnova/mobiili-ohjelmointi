cmake_minimum_required(VERSION 3.16)

project(laskinnn VERSION 0.1 LANGUAGES CXX)

# ---------- C++ ----------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---------- Qt ----------
find_package(Qt6 REQUIRED COMPONENTS Quick Qml)
qt_standard_project_setup(REQUIRES 6.8)

# ---------- Executable ----------
qt_add_executable(applaskinnn
    main.cpp
    calculator.cpp
    calculator.h
)


# ---------- QML module ----------
qt_add_qml_module(applaskinnn
    URI laskinnn
    VERSION 1.0
    QML_FILES
        Main.qml
        CalcButton.qml
)

# ---------- Link Qt ----------
target_link_libraries(applaskinnn
    PRIVATE
        Qt6::Quick
        Qt6::Qml
)

# ---------- App properties ----------
set_target_properties(applaskinnn PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING
        ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

# ---------- Android ----------
if(ANDROID)
    set_property(TARGET applaskinnn APPEND PROPERTY
        QT_ANDROID_PACKAGE_SOURCE_DIR
        ${CMAKE_CURRENT_SOURCE_DIR}/android
    )
endif()

# ---------- Install ----------
include(GNUInstallDirs)
install(TARGETS applaskinnn
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# ---------- REQUIRED (DO NOT REMOVE) ----------
qt_finalize_executable(applaskinnn)

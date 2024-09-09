cmake_minimum_required(VERSION 3.5)

project(awe VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets Core Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Core Network WebEngineWidgets)
set(QSCINTILLA_INCLUDE_DIR "/usr/include/x86_64-linux-gnu/Qsci/")
set(QSCINTILLA_LIBRARY "${CMAKE_CURRENT_LIST_DIR}/lib/QScintilla/src/libqscintilla2_qt6.so.15.2")
set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        leftDock.cpp
        leftDock.h
        networkWindow.cpp
        networkWindow.h
        repeater.cpp
        repeater.h
        rightDock.cpp
        rightDock.h
        siteMapWindow.cpp
        siteMapWindow.h
        targetWindow.cpp
        targetWindow.h
        testtargetwindow.cpp
        testtargetwindow.h
        threadrunners.cpp
        threadrunners.h
        lowerDock.cpp
        lowerDock.h
        capture.cpp
        capture.h
        cmake_install.cmake
        comparer.cpp
        comparer.h
        functionUtils.cpp
        functionUtils.h
        guiUtilities.cpp
        guiUtilities.h
        browserWindow.cpp
        browserWindow.h
        actionsWidget.cpp
        actionsWidget.h
        config.h
        net/socketswrapper.cpp
        syntaxHighlighter.cpp
        syntaxHighlighter.h
        texteditor.cpp 
        texteditor.h 
        toolsrunner.cpp 
        toolsrunner.h
        guiproxyclient.cpp 
        guiproxyclient.h
        reqrestexteditor.cpp 
        reqrestexteditor.h
        messagebox.cpp 
        messagebox.h 
        hoverbutton.cpp
        hoverbutton.h 
        customcheckbox.cpp 
        customcheckbox.h 
        encodingwidget.cpp 
        encodingwidget.h 
        actionsWidget.cpp 
        actionsWidget.h 
        RepeaterReqResTextEditor.cpp 
        RepeaterReqResTextEditor.h 
        atomfunctions.cpp 
        atomfunctions.h
        scieditor.h 
        scieditor.cpp 
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(awe
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET awe APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(awe SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(awe
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_directories(awe PRIVATE ${QSCINTILLA_INCLUDE_DIR})

target_link_libraries(awe PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSN_MAJOR}::Core Qt6::Network Qt6::WebEngineWidgets)
target_link_libraries(awe PRIVATE ${QSCINTILLA_LIBRARY})

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.awe)
endif()
set_target_properties(awe PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS awe
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(awe)
endif()

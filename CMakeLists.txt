#[[
 * Copyright (c) 2024 Wadel Julien.
 *
 * This file is part of Nutrixious
 * (see https://github.com/Ledjlale/Nutrixious).
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 ]]

cmake_minimum_required(VERSION 3.16)
set(TARGET_NAME Nutrixious)
project(${TARGET_NAME} VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INSTALL_PREFIX "OUTPUT")

find_package(Qt6 6.4 REQUIRED COMPONENTS Quick QuickControls2 Sql Charts Multimedia)

add_subdirectory(external/SCodes/src)

qt_standard_project_setup()

set(_APP_QML_FILES)
set(_APP_SOURCE_FILES)
set(_APP_QML_SINGLETONS)

add_subdirectory(ui)
add_subdirectory(src)


qt_add_executable(${TARGET_NAME} main.cpp)

set_source_files_properties(${_APP_QML_SINGLETONS} PROPERTIES QT_QML_SINGLETON_TYPE TRUE)
set_property(TARGET ${TARGET_NAME} APPEND PROPERTY
	QT_ANDROID_PACKAGE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/asset/android
)

qt_add_qml_module(${TARGET_NAME}
	URI App
	VERSION 1.0
	RESOURCE_PREFIX "/"
	QML_FILES ${_APP_QML_FILES} ${_APP_QML_SINGLETONS}
	SOURCES ${_APP_SOURCE_FILES}
)

if (ANDROID)
	include(FetchContent)
	FetchContent_Declare(
		android_openssl
		DOWNLOAD_EXTRACT_TIMESTAMP true
		URL https://github.com/KDAB/android_openssl/archive/refs/heads/master.zip
	)
	FetchContent_MakeAvailable(android_openssl)
	include(${android_openssl_SOURCE_DIR}/android_openssl.cmake)
	add_android_openssl_libraries(${TARGET_NAME})
endif()

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(${TARGET_NAME} PROPERTIES
	MACOSX_BUNDLE_GUI_IDENTIFIER org.ledjlale.nutrixious
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)
include_directories(/usr/local/include)

target_link_libraries(${TARGET_NAME}
	PRIVATE Qt6::Quick Qt6::QuickControls2 Qt6::Sql Qt6::Charts SCodes
)
include(GNUInstallDirs)
install(TARGETS ${TARGET_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

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
project(${TARGET_NAME} VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INSTALL_PREFIX "OUTPUT")

find_package(Qt6 6.5 REQUIRED COMPONENTS Quick QuickControls2 Sql Charts Multimedia Svg Concurrent Network)
find_package(Git)


if (GIT_EXECUTABLE)
  execute_process(
	COMMAND ${GIT_EXECUTABLE} describe --always
	OUTPUT_VARIABLE APP_VERSION
	OUTPUT_STRIP_TRAILING_WHITESPACE
	WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
  )
else()
  set(APP_VERSION "1.0.0")
endif ()

set(APP_MAJOR_VERSION)
set(APP_MINOR_VERSION)
set(APP_MICRO_VERSION)
set(APP_BRANCH_VERSION)
#string(REGEX REPLACE "([0-9.]+)-?.*" "\\1" APP_VERSION "${APP_VERSION}")
#string(REPLACE "." ";" SPLITTED_VERSION "${APP_VERSION}")
#list(LENGTH SPLITTED_VERSION SPLITTED_APP_VERSION_LENGTH)
#list(GET SPLITTED_VERSION 0 APP_MAJOR_VERSION)
#list(GET SPLITTED_VERSION 1 APP_MINOR_VERSION)
#if (SPLITTED_VERSION_LENGTH GREATER 2)
#  list(GET SPLITTED_VERSION 2 APP_MICRO_VERSION)
#endif ()

#set(APP_VERSION ${APP_MAJOR_VERSION}.${APP_MINOR_VERSION}.${APP_MICRO_VERSION})
message(STATUS "Versions : ${APP_VERSION}")


add_subdirectory(external/SCodes/src)

qt_standard_project_setup()

set(_APP_QML_FILES)
set(_APP_SOURCE_FILES)
set(_APP_QML_SINGLETONS)
set(_APP_RC_FILES)

add_definitions("-DAPPLICATION_URL=\"https://github.com/Ledjlale/Nutrixious\"")
add_definitions("-DAPPLICATION_VERSION_NAME=\"V1 Proof of Concept\"")
add_definitions("-DAPPLICATION_VERSION=\"${APP_VERSION}\"")

add_subdirectory(ui)
add_subdirectory(src)
add_subdirectory(asset)


qt_add_executable(${TARGET_NAME} main.cpp)
qt_add_resources(${TARGET_NAME} "resources" PREFIX "/" FILES ${_APP_RC_FILES})

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

#if (ANDROID)
#	include(FetchContent)
#	FetchContent_Declare(
#		android_openssl
#		DOWNLOAD_EXTRACT_TIMESTAMP true
#		URL https://github.com/KDAB/android_openssl/archive/refs/heads/master.zip
#	)
#	FetchContent_MakeAvailable(android_openssl)
#	include(${android_openssl_SOURCE_DIR}/android_openssl.cmake)
#	add_android_openssl_libraries(${TARGET_NAME})
#endif()
if (ANDROID)
# Need to be build with NDK=21.0.6113669
	include("external/android_openssl/android_openssl.cmake")
	add_android_openssl_libraries(${TARGET_NAME})
endif()

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(${TARGET_NAME} PROPERTIES
	MACOSX_BUNDLE_GUI_IDENTIFIER org.ledjlale.Nutrixious
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
    OUTPUT_NAME ${TARGET_NAME}
)
include_directories(/usr/local/include)

target_link_libraries(${TARGET_NAME}
	PRIVATE Qt6::Quick Qt6::QuickControls2 Qt6::Sql Qt6::Charts Qt6::Svg Qt6::Concurrent SCodes
)
include(GNUInstallDirs)
install(TARGETS ${TARGET_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

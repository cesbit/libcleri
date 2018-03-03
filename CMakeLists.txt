cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(cleri VERSION 0.9.4)

option(BUILD_EXAMPLES "Build examples" OFF)

if(NOT DEFINED CMAKE_BUILD_TYPE)
	set (CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_DEBUG_POSTFIX _d)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
find_package(PCRE2 REQUIRED)

configure_file(
	"${CMAKE_SOURCE_DIR}/cmake/version.h.in"
	"${CMAKE_SOURCE_DIR}/inc/cleri/version.h"
)	

set(cleri_src_dir "${CMAKE_SOURCE_DIR}/src")
file(GLOB cleri_src ${cleri_src_dir}/*.c)

add_library(cleri STATIC ${cleri_src})
target_include_directories(cleri PRIVATE ${PCRE2_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/inc)
set_target_properties(cleri PROPERTIES SUFFIX ".so")
target_link_libraries(cleri PRIVATE ${PCRE2_LIBRARIES})

if (BUILD_EXAMPLES)
	set(cleri_example_dir ${CMAKE_SOURCE_DIR}/examples)

	add_executable(choice ${cleri_example_dir}/choice/main.c)
	target_include_directories(choice PRIVATE ${CMAKE_SOURCE_DIR}/inc)
	target_link_libraries(choice PRIVATE cleri)

	add_executable(hi_iris ${cleri_example_dir}/hi_iris/main.c)
	target_include_directories(hi_iris PRIVATE ${CMAKE_SOURCE_DIR}/inc)
	target_link_libraries(hi_iris PRIVATE cleri)

	file(GLOB json_src ${cleri_example_dir}/json/*.*)
	add_executable(json ${json_src})
	target_include_directories(json PRIVATE ${CMAKE_SOURCE_DIR}/inc)
	target_link_libraries(json PRIVATE cleri)

	add_executable(keyword  ${cleri_example_dir}/keyword/main.c)
	target_include_directories(keyword PRIVATE ${CMAKE_SOURCE_DIR}/inc)
	target_link_libraries(keyword cleri)

	add_executable(list_ ${cleri_example_dir}/list/main.c)
	target_include_directories(list_ PRIVATE ${CMAKE_SOURCE_DIR}/inc)
	target_link_libraries(list_ cleri)

	add_executable(optional ${cleri_example_dir}/optional/main.c)
	target_include_directories(optional PRIVATE ${CMAKE_SOURCE_DIR}/inc)
	target_link_libraries(optional cleri)

	add_executable(prio ${cleri_example_dir}/prio/main.c)
	target_include_directories(prio PRIVATE ${CMAKE_SOURCE_DIR}/inc)
	target_link_libraries(prio cleri)

	add_executable(ref ${cleri_example_dir}/ref/main.c)
	target_include_directories(ref PRIVATE ${CMAKE_SOURCE_DIR}/inc)
	target_link_libraries(ref cleri)

	add_executable(repeat ${cleri_example_dir}/repeat/main.c)
	target_include_directories(repeat PRIVATE ${CMAKE_SOURCE_DIR}/inc)
	target_link_libraries(repeat cleri)

	add_executable(sequence ${cleri_example_dir}/sequence/main.c)
	target_include_directories(sequence PRIVATE ${CMAKE_SOURCE_DIR}/inc)
	target_link_libraries(sequence cleri)

	add_executable(token ${cleri_example_dir}/token/main.c)
	target_include_directories(token PRIVATE ${CMAKE_SOURCE_DIR}/inc)
	target_link_libraries(token cleri)

	add_executable(tokens ${cleri_example_dir}/tokens/main.c)
	target_include_directories(tokens PRIVATE ${CMAKE_SOURCE_DIR}/inc)
	target_link_libraries(tokens cleri)
endif()

install(TARGETS cleri EXPORT cleri-targets ARCHIVE DESTINATION lib CONFIGURATIONS ${CMAKE_BUILD_TYPE})
install(DIRECTORY "${CMAKE_SOURCE_DIR}/inc/" DESTINATION include CONFIGURATIONS ${CMAKE_BUILD_TYPE})

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

set(CONF_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/lib/cmake/${CMAKE_PROJECT_NAME}")

write_basic_package_version_file(
	"${CMAKE_BINARY_DIR}/cleri-config-version.cmake"
	COMPATIBILITY AnyNewerVersion
)

export(EXPORT cleri-targets FILE ${CMAKE_BINARY_DIR}/cleri-targets.cmake)

configure_package_config_file(
	"${CMAKE_MODULE_PATH}/cleri-config.cmake.in"
	"${CMAKE_BINARY_DIR}/cleri-config.cmake"
	INSTALL_DESTINATION ${CONF_INSTALL_DIR}
	PATH_VARS CMAKE_INSTALL_FULL_INCLUDEDIR CMAKE_INSTALL_FULL_LIBDIR
)

install(EXPORT cleri-targets FILE cleri-targets.cmake DESTINATION ${CONF_INSTALL_DIR})

install(FILES ${CMAKE_BINARY_DIR}/cleri-config.cmake ${CMAKE_BINARY_DIR}/cleri-config-version.cmake DESTINATION ${CONF_INSTALL_DIR})

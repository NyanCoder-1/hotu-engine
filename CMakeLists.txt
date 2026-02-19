cmake_minimum_required(VERSION 4.0.0)

set(PROJECT "hotu-engine")
set(TARGET "hotu-engine")
project(${PROJECT})

# Enable export compile commands
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Paths
set(PROJECT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
cmake_path(NORMAL_PATH PROJECT_DIR)
set(SOURCE_DIR "${PROJECT_DIR}/src")
set(HEADERS_DIR "${PROJECT_DIR}/include")
set(SUBMODULES_DIR "${PROJECT_DIR}/submodules")

# Scan source files and headers
file(GLOB_RECURSE SOURCES "${SOURCE_DIR}/*.cpp" "${SOURCE_DIR}/*.c")
file(GLOB_RECURSE HEADERS "${SOURCE_DIR}/*.hpp" "${HEADERS_DIR}/*.hpp" "${SOURCE_DIR}/*.h" "${HEADERS_DIR}/*.h")
# Exclude platform-specific files (we'll add them in corresponding if-else blocks below)
list(FILTER SOURCES EXCLUDE REGEX "/platform-.*/")
list(FILTER HEADERS EXCLUDE REGEX "/platform-.*/")

set(INCLUDE_DIR
	"${SOURCE_DIR}/"
	"${HEADERS_DIR}/"
	"${SUBMODULES_DIR}/stb"
	"${SUBMODULES_DIR}/cglm/include")
set(EXTERNAL_LIBRARIES)

if (${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")

	file(GLOB_RECURSE SOURCES_EMSCRIPTEN "${SOURCE_DIR}/platform-emscripten/*.cpp" "${SOURCE_DIR}/platform-emscripten/*.c")
	set(SOURCES ${SOURCES} ${SOURCES_EMSCRIPTEN})

elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")

	message(INFO "Warning: Wayland only, no X11 yet")
	file(GLOB_RECURSE SOURCES_LINUX "${SOURCE_DIR}/platform-linux/*.cpp" "${SOURCE_DIR}/platform-linux/*.c")
	set(SOURCES ${SOURCES} ${SOURCES_LINUX})
	set(EXTERNAL_LIBRARIES ${EXTERNAL_LIBRARIES} m wayland-client wayland-egl EGL xkbcommon)

elseif (${CMAKE_SYSTEM_NAME} MATCHES "Android")

	message(FATAL_ERROR "Error: This platform is planned to support but not yet")

elseif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")

	message(FATAL_ERROR "Error: This platform is planned to support but not yet")

else ()

	message(FATAL_ERROR "Error: Unknown platform")

endif ()

# Target
add_library(${TARGET} STATIC ${SOURCES})
include_directories(${INCLUDE_DIR})
if (EXTERNAL_LIBRARIES)
	target_link_libraries(${TARGET} ${EXTERNAL_LIBRARIES})
endif()


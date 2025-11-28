cmake_minimum_required(VERSION 3.16)
project(MiGestorMultimedia LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ------------------------------------------------
# 1. CONFIGURACIÓN DE VCPKG
# ------------------------------------------------

# Especifica la ruta al toolchain de vcpkg
# IMPORTANTE: Ajusta esta ruta a tu instalación de vcpkg
set(CMAKE_TOOLCHAIN_FILE "D:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

# Triplet para Windows x64
set(VCPKG_TARGET_TRIPLET "x64-windows" CACHE STRING "")

# ------------------------------------------------
# 2. CONFIGURACIÓN DE QT
# ------------------------------------------------

# RUTA DE QT
set(CMAKE_PREFIX_PATH "D:/Qt/6.10.0/msvc2022_64") 

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# ------------------------------------------------
# 3. BÚSQUEDA DE PAQUETES
# ------------------------------------------------

# Buscar Qt
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# Buscar FFmpeg (vcpkg proporciona targets modernos)
find_package(PkgConfig REQUIRED)
pkg_check_modules(FFMPEG REQUIRED IMPORTED_TARGET
    libavformat
    libavcodec
    libavutil
    libswscale
    libswresample
)

# Buscar OpenCV
find_package(OpenCV REQUIRED COMPONENTS
    core
    imgproc
    imgcodecs
    highgui
    videoio
    video
    features2d
)

# ------------------------------------------------
# 4. CONFIGURACIÓN DEL PROYECTO
# ------------------------------------------------

add_executable(MiApp
    src/main.cpp
    src/gui/MainWindow.cpp
    src/gui/MainWindow.h
    src/media_engine/MediaReader.cpp
    src/media_engine/MediaReader.h
    src/media_engine/MediaDecoder.cpp
    src/media_engine/MediaDecoder.h
    src/media_engine/VideoProcessor.cpp
    src/media_engine/VideoProcessor.h
    src/core/MediaStructs.h
)

# Establece el subsistema a Windows (GUI)
set_property(TARGET MiApp PROPERTY WIN32_EXECUTABLE TRUE)

# ------------------------------------------------
# 5. INCLUDES Y LIBRARIES
# ------------------------------------------------

# Incluir directorios (vcpkg y find_package se encargan automáticamente)
target_include_directories(MiApp PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Configurar UTF-8 para MSVC
if(MSVC)
    target_compile_options(MiApp PRIVATE /utf-8)
endif()

# Enlazar librerías
target_link_libraries(MiApp PRIVATE
    Qt6::Widgets
    PkgConfig::FFMPEG
    ${OpenCV_LIBS}
)

# ------------------------------------------------
# 6. POST-BUILD: COPIA DE DLLs
# ------------------------------------------------

# Copiar DLLs de Qt
add_custom_command(TARGET MiApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:Qt6::Core>
    $<TARGET_FILE:Qt6::Gui>
    $<TARGET_FILE:Qt6::Widgets>
    $<TARGET_FILE_DIR:MiApp>
    COMMENT "Copiando DLLs de Qt..."
)

# Copiar DLLs de vcpkg (FFmpeg y dependencias)
if(CMAKE_BUILD_TYPE MATCHES "Debug")
    set(VCPKG_BIN_DIR "D:/vcpkg/installed/x64-windows/debug/bin")
else()
    set(VCPKG_BIN_DIR "D:/vcpkg/installed/x64-windows/bin")
endif()

add_custom_command(TARGET MiApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${VCPKG_BIN_DIR}
    $<TARGET_FILE_DIR:MiApp>
    COMMENT "Copiando DLLs de vcpkg (FFmpeg, OpenCV, etc.)..."
)

# ------------------------------------------------
# 7. MENSAJES DE INFORMACIÓN
# ------------------------------------------------

message(STATUS "==============================================")
message(STATUS "Configuración del proyecto:")
message(STATUS "  - Vcpkg toolchain: ${CMAKE_TOOLCHAIN_FILE}")
message(STATUS "  - Qt prefix: ${CMAKE_PREFIX_PATH}")
message(STATUS "  - FFmpeg encontrado: ${FFMPEG_FOUND}")
message(STATUS "  - OpenCV versión: ${OpenCV_VERSION}")
message(STATUS "  - OpenCV libs: ${OpenCV_LIBS}")
message(STATUS "==============================================")

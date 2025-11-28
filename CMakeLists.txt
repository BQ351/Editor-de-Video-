cmake_minimum_required(VERSION 3.16)
project(MiGestorMultimedia LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ------------------------------------------------
# 1. CONFIGURACIÓN DE RUTAS (PATHS)
# ------------------------------------------------

# RUTA DE QT
set(CMAKE_PREFIX_PATH "D:/Qt/6.10.0/msvc2022_64") 

# RUTA DE FFMPEG (Rutas originales de VCPKG para las DLLs de FFmpeg)
set(FFMPEG_INCLUDE_DIR "D:/vcpkg/installed/x64-windows/include")
set(FFMPEG_LIB_DIR "D:/vcpkg/installed/x64-windows/lib")

# RUTA DE OPENCV (✅ CORREGIDA)
set(OPENCV_ROOT "D:/ProgramacionDelSistemaProyect/lib/opencv")
set(OPENCV_INCLUDE_DIR "${OPENCV_ROOT}/include/opencv4")
set(OPENCV_LIB_DIR "${OPENCV_ROOT}/lib")

# ------------------------------------------------
# 2. CONFIGURACIÓN DEL PROYECTO
# ------------------------------------------------

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

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
# 3. INCLUDES Y LIBRARIES
# ------------------------------------------------

# ✅ CORREGIDO: Incluir la ruta correcta de OpenCV
target_include_directories(MiApp PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${FFMPEG_INCLUDE_DIR}
    ${OPENCV_INCLUDE_DIR}  # Esto resuelve a: D:/ProgramacionDelSistemaProyect/lib/opencv/include/opencv4
)

# ✅ CORREGIDO: Especificar UTF-8 para evitar warnings C4828
if(MSVC)
    target_compile_options(MiApp PRIVATE /utf-8)
endif()

# Enlace a las librerías
target_link_libraries(MiApp PRIVATE
    Qt6::Widgets
    
    # FFmpeg (Desde VCPKG)
    ${FFMPEG_LIB_DIR}/avformat.lib
    ${FFMPEG_LIB_DIR}/avcodec.lib
    ${FFMPEG_LIB_DIR}/avutil.lib
    ${FFMPEG_LIB_DIR}/swscale.lib
    ${FFMPEG_LIB_DIR}/swresample.lib
    
    # OpenCV (Solo los módulos esenciales)
    ${OPENCV_LIB_DIR}/opencv_core4.lib
    ${OPENCV_LIB_DIR}/opencv_imgproc4.lib
    ${OPENCV_LIB_DIR}/opencv_imgcodecs4.lib
    ${OPENCV_LIB_DIR}/opencv_highgui4.lib
    ${OPENCV_LIB_DIR}/opencv_videoio4.lib
    ${OPENCV_LIB_DIR}/opencv_video4.lib
    ${OPENCV_LIB_DIR}/opencv_features2d4.lib
)

# ------------------------------------------------
# 4. COPIA AUTOMÁTICA DE DLLs
# ------------------------------------------------

# Copiar DLLs de VCPKG (FFmpeg)
add_custom_command(TARGET MiApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "D:/vcpkg/installed/x64-windows/bin"
    $<TARGET_FILE_DIR:MiApp>
    COMMENT "Copiando DLLs de FFmpeg desde Vcpkg..."
)

# Copiar DLLs de OpenCV
add_custom_command(TARGET MiApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${OPENCV_ROOT}/bin"
    $<TARGET_FILE_DIR:MiApp>
    COMMENT "Copiando DLLs de OpenCV..."
)
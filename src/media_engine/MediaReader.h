// src/media_engine/MediaReader.h
#pragma once
#include "core/MediaStructs.h"
#include "MediaDecoder.h"
#include <string>

// Clase encargada EXCLUSIVAMENTE de hablar con FFmpeg
class MediaReader {
public:
    MediaReader();
    ~MediaReader();

    MediaInfo extractMetadata(const std::string& filepath);

    // Metodo para obtener el siguiente paquete (chunk comprimido) del archivo
    // Este sera el nucleo de tu bucle de procesamiento
    DecodedFrame readNextFrame();

private:
    void cleanUp();
    // Usamos void* para no ensuciar el header con tipos de FFmpeg
    void* format_context = nullptr;
    void* packet = nullptr;

    // Nueva variable para guardar el indice del stream de video
    int video_stream_index = -1;

    MediaDecoder decoder;
};
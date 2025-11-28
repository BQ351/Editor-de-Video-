#pragma once

#include "core/MediaStructs.h" // Incluye MediaInfo
#include <string>
#include <vector>

// Necesitaras una estructura para manejar los datos del frame sin comprimir
struct DecodedFrame {
    // Usaremos un puntero simple para Frame data
    std::vector<uint8_t> data; 
    int width = 0;
    int height = 0;
    long long pts = 0; // Presentation Time Stamp
    bool is_valid = false;
};

class MediaDecoder {
public:
    MediaDecoder();
    ~MediaDecoder();

    // 1. Inicializa el decodificador (usa el codec_id y los parametros)
    bool openDecoder(int codec_id, int width, int height);

    // 2. Procesa datos: Envia un paquete y espera un frame
    DecodedFrame decodePacket(const void* packet_data, int packet_size);

private:
    void cleanup();

    // Punteros opacos de FFmpeg para ocultar la complejidad
    void* codec_context = nullptr; 
    void* frame = nullptr; 
    void* packet = nullptr;
};
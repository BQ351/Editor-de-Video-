// src/media_engine/MediaDecoder.cpp

#include "MediaDecoder.h"
#include <iostream>
#include <stdexcept>
#include <vector>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavutil/imgutils.h>
#include <libavutil/mem.h> 
}

MediaDecoder::MediaDecoder() {
    // Inicializar estructuras AVPacket y AVFrame (av_frame_alloc)
    packet = av_packet_alloc();
    frame = av_frame_alloc();
}

MediaDecoder::~MediaDecoder() {
    cleanup();
}

void MediaDecoder::cleanup() {
    // 1. Liberar contexto del c dec
    if (codec_context) {
        AVCodecContext*& ctx = reinterpret_cast<AVCodecContext*&>(codec_context);
        avcodec_free_context(&ctx);
        codec_context = nullptr;
    }
    // 2. Liberar frame y paquete
    av_frame_free(reinterpret_cast<AVFrame**>(&frame));
    av_packet_free(reinterpret_cast<AVPacket**>(&packet));
}

// ----------------------------------------------------
// 1. Abrir Decodificador (Setup)
// ----------------------------------------------------

bool MediaDecoder::openDecoder(int codec_id, int width, int height) {
    const AVCodec* codec = avcodec_find_decoder((AVCodecID)codec_id);
    if (!codec) {
        std::cerr << "Error: No se encontr  el c dec con ID: " << codec_id << std::endl;
        return false;
    }

    AVCodecContext* ctx = avcodec_alloc_context3(codec);
    if (!ctx) {
        std::cerr << "Error: No se pudo asignar el contexto del c dec." << std::endl;
        return false;
    }

    // Configurar el contexto con los par metros del stream
    ctx->width = width;
    ctx->height = height;

    // Abrir el decodificador
    if (avcodec_open2(ctx, codec, nullptr) < 0) {
        std::cerr << "Error: No se pudo abrir el c dec." << std::endl;
        avcodec_free_context(&ctx);
        return false;
    }

    codec_context = ctx;
    return true;
}

// ----------------------------------------------------
// 2. Decodificar un Paquete (Loop Core Logic)
// ----------------------------------------------------

DecodedFrame MediaDecoder::decodePacket(const void* packet_data, int packet_size) {
    DecodedFrame decodedFrame;
    AVCodecContext* ctx = reinterpret_cast<AVCodecContext*>(codec_context);
    AVPacket* pkt = reinterpret_cast<AVPacket*>(packet);
    AVFrame* frm = reinterpret_cast<AVFrame*>(frame);

    if (!ctx || !pkt) return decodedFrame; // No inicializado

    // Inicializar el paquete AVPacket con los datos recibidos
    av_packet_unref(pkt); // Limpiar datos anteriores
    pkt->data = const_cast<uint8_t*>(static_cast<const uint8_t*>(packet_data));
    pkt->size = packet_size;

    // 1. Enviar el paquete comprimido al decodificador
    int ret = avcodec_send_packet(ctx, pkt);
    if (ret < 0) {
        std::cerr << "Error al enviar el paquete al decodificador." << std::endl;
        return decodedFrame;
    }

    // 2. Entrar en el bucle para recibir todos los frames disponibles
    while (ret >= 0) {
        ret = avcodec_receive_frame(ctx, frm);

        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
            // Decodificador necesita m s datos (EAGAIN) o stream ha terminado (EOF)
            decodedFrame.is_valid = false;
            return decodedFrame;
        }
        else if (ret < 0) {
            std::cerr << "Error al recibir frame del decodificador." << std::endl;
            decodedFrame.is_valid = false;
            return decodedFrame;
        }

        // 3. Éxito: El frame está listo.
        decodedFrame.is_valid = true;
        decodedFrame.width = frm->width;
        decodedFrame.height = frm->height;
        decodedFrame.pts = frm->pts;

        // --- PROCESAMIENTO DE DATOS DEL FRAME (SIMPLIFICADO) ---

        // El campo frm->format contiene el AVPixelFormat (un enum que se comporta como int)
        // El argumento 1 de av_image_get_buffer_size es de tipo enum AVPixelFormat.

        int buffer_size = av_image_get_buffer_size(
            (AVPixelFormat)frm->format, // Corregido: Usamos el formato del frame
            frm->width,                 // Corregido: Usamos el ancho del frame
            frm->height,                // Corregido: Usamos el alto del frame
            1                           // padding (alineación, 1 es un valor seguro)
        );

        if (buffer_size < 0) {
            // Manejar error si el formato es inválido o cálculo falla
            std::cerr << "Error al calcular el tamaño del buffer de imagen." << std::endl;
            decodedFrame.is_valid = false;
            return decodedFrame;
        }

        decodedFrame.data.resize(buffer_size); // Usamos el tamaño calculado (buffer_size)

        // av_image_copy_to_buffer(decodedFrame.data.data(), buffer_size, frm->data, frm->linesize, frm->format, frm->width, frm->height, 1);
        // Dejamos el copiado complejo para la clase de procesamiento, solo devolvemos la info.

        return decodedFrame; // Devolvemos el primer frame decodificado
    }

    return decodedFrame;
}
//#include "MediaReader.h"
//#include <iostream>
//
//extern "C" {
//    #include <libavformat/avformat.h>
//    #include <libavutil/avutil.h>
//    #include <libavcodec/avcodec.h>
//}
//
//MediaReader::MediaReader() {
//    // En versiones nuevas de FFmpeg no hace falta av_register_all()
//}
//
//MediaReader::~MediaReader() {
//    cleanUp();
//}
//
//void MediaReader::cleanUp() {
//    if (format_context) {
//        AVFormatContext* ctx = (AVFormatContext*)format_context;
//        avformat_close_input(&ctx);
//        format_context = nullptr;
//    }
//}
//
//MediaInfo MediaReader::extractMetadata(const std::string& filepath) {
//    MediaInfo info;
//    info.filepath = filepath;
//    info.isValid = false;
//
//    AVFormatContext* ctx = nullptr;
//
//    // 1. Abrir el archivo (Header)
//    if (avformat_open_input(&ctx, filepath.c_str(), nullptr, nullptr) != 0) {
//        std::cerr << "Error: No se pudo abrir el archivo." << std::endl;
//        return info;
//    }
//    format_context = ctx; // Guardamos referencia para limpiar luego
//
//    // 2. Buscar información de los streams (crucial para saber codecs)
//    if (avformat_find_stream_info(ctx, nullptr) < 0) {
//        std::cerr << "Error: No se pudo encontrar info de stream." << std::endl;
//        cleanUp();
//        return info;
//    }
//
//    // 3. Rellenar datos generales
//    info.isValid = true;
//    info.durationSec = (double)ctx->duration / AV_TIME_BASE;
//    info.formatName = ctx->iformat->name;
//    info.bitRate = ctx->bit_rate;
//
//    // 4. Buscar el stream de video para detalles específicos
//    for (unsigned int i = 0; i < ctx->nb_streams; i++) {
//        AVCodecParameters* localCodec = ctx->streams[i]->codecpar;
//        
//        if (localCodec->codec_type == AVMEDIA_TYPE_VIDEO) {
//            info.width = localCodec->width;
//            info.height = localCodec->height;
//            
//            // Obtener nombre del codec
//            const AVCodec* codec = avcodec_find_decoder(localCodec->codec_id);
//            if (codec) {
//                info.codecName = codec->name;
//            } else {
//                info.codecName = "Desconocido ID: " + std::to_string(localCodec->codec_id);
//            }
//            
//            // Solo nos interesa el primer stream de video
//            break; 
//        }
//    }
//
//    cleanUp(); // Cerramos el archivo al terminar de leer metadatos
//    return info;
//}

////////////////////MediaReader.cpp////////////////////

#include "MediaReader.h"
#include <iostream>
#include <sstream>
#include <stdexcept>

// Incluir las librerías C de FFmpeg
extern "C" {
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libavcodec/avcodec.h>
#include <libavutil/mathematics.h> 
}

// Función auxiliar para convertir unidades de tiempo de FFmpeg a segundos (double)
static double timeBaseToSeconds(int64_t pts, AVRational time_base) {
    if (pts == AV_NOPTS_VALUE || time_base.den == 0 || time_base.num == 0)
        return 0.0;
    return pts * av_q2d(time_base);
}



// ----------------------------------------------------
// LIMPIEZA DE MEMORIA (CLEANUP)
// ----------------------------------------------------

// ----------------------------------------------------
// LÓGICA PRINCIPAL (METADATOS)
// ----------------------------------------------------

MediaInfo MediaReader::extractMetadata(const std::string& filepath) {
    MediaInfo info;
    info.filepath = filepath;
    info.isValid = false; // Asumimos fallo hasta el final

    AVFormatContext* fmt_ctx = nullptr;

    try {
        // 1. Abrir el archivo y leer encabezados
        int ret = avformat_open_input(&fmt_ctx, filepath.c_str(), nullptr, nullptr);
        if (ret < 0) {
            char err_buf[AV_ERROR_MAX_STRING_SIZE];
            av_strerror(ret, err_buf, AV_ERROR_MAX_STRING_SIZE);
            throw std::runtime_error("Error FFmpeg al abrir archivo: " + std::string(err_buf));
        }

        // 2. Encontrar la información del stream
        ret = avformat_find_stream_info(fmt_ctx, nullptr);
        if (ret < 0) {
            throw std::runtime_error("Error FFmpeg: No se pudo encontrar la información del stream.");
        }

        // 3. Rellenar datos generales del contenedor
        info.bitRate = fmt_ctx->bit_rate;
        info.formatName = fmt_ctx->iformat->name;

        // 4. Buscar el mejor stream de video para detalles específicos
        int video_stream_index = av_find_best_stream(fmt_ctx, AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0);

        if (video_stream_index >= 0) {
            AVStream* video_stream = fmt_ctx->streams[video_stream_index];
            AVCodecParameters* codec_params = video_stream->codecpar;

            // Rellenar información específica del video
            info.width = codec_params->width;
            info.height = codec_params->height;

            // Obtener nombre del códec
            const AVCodec* codec = avcodec_find_decoder(codec_params->codec_id);
            info.codecName = codec ? codec->name : "Desconocido";

    //        // Calcular duración en segundos
    //        if (video_stream->duration != AV_NOPTS_VALUE) {
    //            info.durationSec = timeBaseToSeconds(video_stream->duration, video_stream->time_base);
    //        }
    //        else if (fmt_ctx->duration != AV_NOPTS_VALUE) {
    //            info.durationSec = (double)fmt_ctx->duration / AV_TIME_BASE;
    //        }
    //    }

    //    // Éxito
    //    info.isValid = true;

    //}
    //catch (const std::exception& e) {
    //    // Si hay un error, aseguramos la limpieza y lanzamos el mensaje a la GUI
    //    std::cerr << "MediaReader Exception: " << e.what() << std::endl;
    //    if (fmt_ctx) {
    //        avformat_close_input(&fmt_ctx);
    //    }
    //    throw; // Relanzar excepción para que la atrape MainWindow
    //}

    //// Cerramos los recursos de FFmpeg y devolvemos la información
    //if (fmt_ctx) {
    //    format_context = fmt_ctx; // Establecer el puntero para que cleanUp lo cierre
    //    cleanUp();
    //}
    //return info;

            // Rellenar información específica del video
            info.width = codec_params->width;
            info.height = codec_params->height;
            // ... (resto del relleno de info)

            // GUARDAR ESTADO PARA DECODIFICACIÓN
            video_stream_index = video_stream_index; // Guardamos el índice encontrado

            // Inicializar el decodificador (usando los parámetros que encontramos)
            bool decoder_ok = decoder.openDecoder(
                codec_params->codec_id,
                codec_params->width,
                codec_params->height
            );

            if (!decoder_ok) {
                // Lanzar error si el decodificador no puede abrirse
                throw std::runtime_error("Error: No se pudo inicializar el decodificador de video.");
            }
        }

        // Éxito
        info.isValid = true;

        // Guardar el contexto del formato (AVFormatContext) para usar en readNextFrame
        format_context = fmt_ctx; // ¡NO LLAMAMOS cleanUp AQUÍ!
    }
    catch (const std::exception& e) {
        // ... (Tu código de manejo de errores, asegurando la limpieza)
        if (fmt_ctx) {
            avformat_close_input(&fmt_ctx);
        }
        throw;
    }

    return info;
}

MediaReader::MediaReader() {
    // Inicializar el paquete que se usará para leer datos
    packet = av_packet_alloc(); // <-- ¡NUEVO!
}

MediaReader::~MediaReader() { cleanUp(); }

void MediaReader::cleanUp() {
    if (format_context) {
        AVFormatContext* ctx = reinterpret_cast<AVFormatContext*>(format_context);
        avformat_close_input(&ctx);
        format_context = nullptr;
    }

    // Liberar memoria del paquete
    if (packet) {
        AVPacket* pkt = reinterpret_cast<AVPacket*>(packet);
        av_packet_free(&pkt); // <-- ¡NUEVO!
        packet = nullptr;
    }

    // El destructor de MediaDecoder se llama automáticamente
}

DecodedFrame MediaReader::readNextFrame() {
    AVFormatContext* fmt_ctx = reinterpret_cast<AVFormatContext*>(format_context);
    AVPacket* pkt = reinterpret_cast<AVPacket*>(packet);

    if (!fmt_ctx || !pkt || video_stream_index == -1) {
        return DecodedFrame(); // Devuelve un frame inválido si el motor no está abierto
    }

    // Bucle para leer paquetes hasta que obtenemos un frame o llegamos al final del archivo
    while (av_read_frame(fmt_ctx, pkt) >= 0) {

        // 1. Verificar si el paquete pertenece al stream de video que nos interesa
        if (pkt->stream_index == video_stream_index) {

            // 2. Intentar decodificar el paquete usando el decodificador inicializado
            // Nota: Aquí pasarías el AVPacket real, no solo los datos y el tamaño.
            DecodedFrame frame = decoder.decodePacket(pkt->data, pkt->size);

            // Liberamos el paquete inmediatamente después de que el decodificador lo procese
            av_packet_unref(pkt);

            if (frame.is_valid) {
                return frame; // ¡Éxito! Frame decodificado.
            }
        }

        // Si no es el stream de video o el decodificador necesita más datos, 
        // liberamos el paquete y continuamos con el siguiente.
        av_packet_unref(pkt);
    }

    // Si salimos del bucle, intentamos liberar cualquier frame pendiente (flushing)
    // Decodificar el último frame (Flushing the decoder)
    DecodedFrame final_frame = decoder.decodePacket(nullptr, 0);
    if (final_frame.is_valid) {
        return final_frame;
    }

    // Fin del stream (EOF)
    return DecodedFrame();
}
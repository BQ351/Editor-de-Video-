// src/media_engine/VideoProcessor.cpp

#include "VideoProcessor.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
}

#include <opencv2/opencv.hpp>

VideoProcessor::VideoProcessor() {}
VideoProcessor::~VideoProcessor() {}

// ===== VALIDACION =====
bool VideoProcessor::validateInputFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.good()) {
        lastError = "Archivo no encontrado: " + path;
        return false;
    }
    return true;
}

void VideoProcessor::updateProgress(float progress, const ProcessConfig& config) {
    if (config.progressCallback) {
        config.progressCallback(progress);
    }
}

// ===== 1. CONVERSION DE FORMATO =====
bool VideoProcessor::convertFormat(const std::string& inputPath,
    const std::string& outputPath,
    const std::string& format) {
    if (!validateInputFile(inputPath)) return false;

    AVFormatContext* inputCtx = nullptr;
    AVFormatContext* outputCtx = nullptr;

    try {
        // Abrir archivo de entrada
        if (avformat_open_input(&inputCtx, inputPath.c_str(), nullptr, nullptr) < 0) {
            throw std::runtime_error("No se pudo abrir el archivo de entrada");
        }

        if (avformat_find_stream_info(inputCtx, nullptr) < 0) {
            throw std::runtime_error("No se pudo encontrar informacion del stream");
        }

        // Crear contexto de salida
        avformat_alloc_output_context2(&outputCtx, nullptr, format.c_str(), outputPath.c_str());
        if (!outputCtx) {
            throw std::runtime_error("No se pudo crear el contexto de salida");
        }

        // Copiar streams
        for (unsigned int i = 0; i < inputCtx->nb_streams; i++) {
            AVStream* inStream = inputCtx->streams[i];
            AVStream* outStream = avformat_new_stream(outputCtx, nullptr);

            if (!outStream) {
                throw std::runtime_error("Error al crear stream de salida");
            }

            avcodec_parameters_copy(outStream->codecpar, inStream->codecpar);
            outStream->codecpar->codec_tag = 0;
        }

        // Abrir archivo de salida
        if (!(outputCtx->oformat->flags & AVFMT_NOFILE)) {
            if (avio_open(&outputCtx->pb, outputPath.c_str(), AVIO_FLAG_WRITE) < 0) {
                throw std::runtime_error("No se pudo abrir el archivo de salida");
            }
        }

        // Escribir header
        if (avformat_write_header(outputCtx, nullptr) < 0) {
            throw std::runtime_error("Error al escribir header");
        }

        // Copiar paquetes
        AVPacket packet;
        while (av_read_frame(inputCtx, &packet) >= 0) {
            AVStream* inStream = inputCtx->streams[packet.stream_index];
            AVStream* outStream = outputCtx->streams[packet.stream_index];

            packet.pts = av_rescale_q_rnd(packet.pts, inStream->time_base,
                outStream->time_base,
                (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
            packet.dts = av_rescale_q_rnd(packet.dts, inStream->time_base,
                outStream->time_base,
                (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
            packet.duration = av_rescale_q(packet.duration, inStream->time_base,
                outStream->time_base);
            packet.pos = -1;

            if (av_interleaved_write_frame(outputCtx, &packet) < 0) {
                av_packet_unref(&packet);
                throw std::runtime_error("Error al escribir frame");
            }

            av_packet_unref(&packet);
        }

        av_write_trailer(outputCtx);

        // Cleanup
        avformat_close_input(&inputCtx);
        if (outputCtx && !(outputCtx->oformat->flags & AVFMT_NOFILE))
            avio_closep(&outputCtx->pb);
        avformat_free_context(outputCtx);

        return true;
    }
    catch (const std::exception& e) {
        lastError = e.what();
        if (inputCtx) avformat_close_input(&inputCtx);
        if (outputCtx) {
            if (!(outputCtx->oformat->flags & AVFMT_NOFILE))
                avio_closep(&outputCtx->pb);
            avformat_free_context(outputCtx);
        }
        return false;
    }
}

// ===== 2. RECORTE TEMPORAL =====
bool VideoProcessor::trimVideo(const ProcessConfig& config) {
    if (!validateInputFile(config.inputPath)) return false;
    lastError = "Funcion trimVideo en desarrollo";
    return false;
}

// ===== 3. CAMBIO DE RESOLUCION =====
bool VideoProcessor::scaleVideo(const ProcessConfig& config) {
    if (!validateInputFile(config.inputPath)) return false;

    if (config.targetWidth <= 0 || config.targetHeight <= 0) {
        lastError = "Dimensiones invalidas";
        return false;
    }

    lastError = "Funcion scaleVideo en desarrollo";
    return false;
}

// ===== 4. EXTRACCION DE FRAMES =====
bool VideoProcessor::extractFrames(const ProcessConfig& config) {
    if (!validateInputFile(config.inputPath)) return false;

    AVFormatContext* fmtCtx = nullptr;
    AVCodecContext* codecCtx = nullptr;
    SwsContext* swsCtx = nullptr;

    try {
        // Abrir archivo
        if (avformat_open_input(&fmtCtx, config.inputPath.c_str(), nullptr, nullptr) < 0) {
            throw std::runtime_error("No se pudo abrir el archivo");
        }

        if (avformat_find_stream_info(fmtCtx, nullptr) < 0) {
            throw std::runtime_error("No se pudo encontrar info del stream");
        }

        // Buscar stream de video
        int videoStreamIdx = av_find_best_stream(fmtCtx, AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0);
        if (videoStreamIdx < 0) {
            throw std::runtime_error("No se encontro stream de video");
        }

        AVStream* videoStream = fmtCtx->streams[videoStreamIdx];
        const AVCodec* codec = avcodec_find_decoder(videoStream->codecpar->codec_id);

        codecCtx = avcodec_alloc_context3(codec);
        avcodec_parameters_to_context(codecCtx, videoStream->codecpar);

        if (avcodec_open2(codecCtx, codec, nullptr) < 0) {
            throw std::runtime_error("No se pudo abrir el codec");
        }

        // Preparar conversion a RGB
        swsCtx = sws_getContext(
            codecCtx->width, codecCtx->height, codecCtx->pix_fmt,
            codecCtx->width, codecCtx->height, AV_PIX_FMT_RGB24,
            SWS_BILINEAR, nullptr, nullptr, nullptr
        );

        AVFrame* frame = av_frame_alloc();
        AVFrame* frameRGB = av_frame_alloc();

        int numBytes = av_image_get_buffer_size(AV_PIX_FMT_RGB24,
            codecCtx->width,
            codecCtx->height, 1);
        uint8_t* buffer = (uint8_t*)av_malloc(numBytes * sizeof(uint8_t));

        av_image_fill_arrays(frameRGB->data, frameRGB->linesize, buffer,
            AV_PIX_FMT_RGB24, codecCtx->width, codecCtx->height, 1);

        AVPacket packet;
        int frameCount = 0;
        int savedCount = 0;

        while (av_read_frame(fmtCtx, &packet) >= 0) {
            if (packet.stream_index == videoStreamIdx) {
                avcodec_send_packet(codecCtx, &packet);

                while (avcodec_receive_frame(codecCtx, frame) == 0) {
                    frameCount++;

                    if (frameCount % config.frameInterval == 0) {
                        // Convertir a RGB
                        sws_scale(swsCtx, frame->data, frame->linesize, 0,
                            codecCtx->height, frameRGB->data, frameRGB->linesize);

                        // Crear imagen OpenCV
                        cv::Mat img(codecCtx->height, codecCtx->width, CV_8UC3,
                            frameRGB->data[0], frameRGB->linesize[0]);
                        cv::cvtColor(img, img, cv::COLOR_RGB2BGR);

                        // Guardar
                        std::stringstream ss;
                        ss << config.frameOutputDir << "/frame_"
                            << std::setw(6) << std::setfill('0') << savedCount << ".jpg";
                        cv::imwrite(ss.str(), img);
                        savedCount++;

                        updateProgress((float)frameCount / 1000.0f, config);
                    }
                }
            }
            av_packet_unref(&packet);
        }

        // Cleanup
        av_free(buffer);
        av_frame_free(&frameRGB);
        av_frame_free(&frame);
        sws_freeContext(swsCtx);
        avcodec_free_context(&codecCtx);
        avformat_close_input(&fmtCtx);

        return true;
    }
    catch (const std::exception& e) {
        lastError = e.what();
        if (swsCtx) sws_freeContext(swsCtx);
        if (codecCtx) avcodec_free_context(&codecCtx);
        if (fmtCtx) avformat_close_input(&fmtCtx);
        return false;
    }
}

// ===== 5. EXTRACCION DE AUDIO =====
bool VideoProcessor::extractAudio(const std::string& inputPath,
    const std::string& outputPath) {
    if (!validateInputFile(inputPath)) return false;
    lastError = "Funcion extractAudio en desarrollo";
    return false;
}

// ===== 6. APLICAR FILTROS OPENCV =====
bool VideoProcessor::applyFilters(const ProcessConfig& config) {
    if (!validateInputFile(config.inputPath)) return false;
    lastError = "Funcion applyFilters en desarrollo";
    return false;
}

// ===== 7. OBTENER MINIATURA =====
bool VideoProcessor::getThumbnail(const std::string& inputPath,
    const std::string& outputPath,
    double timeSeconds) {
    if (!validateInputFile(inputPath)) return false;
    lastError = "Funcion getThumbnail en desarrollo";
    return false;
}
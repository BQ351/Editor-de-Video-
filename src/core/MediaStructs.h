/////////////////MediaStructs.h////////////////
//Media information structure
#pragma once
#include <string>

struct MediaInfo {
    bool isValid = false;       // ¿Se leyó bien?
    std::string filepath;       // Ruta original
    std::string fileName;       // Solo el nombre
    
    // Datos técnicos
    int width = 0;
    int height = 0;
    double durationSec = 0.0;
    std::string codecName = "Desconocido";
    std::string formatName = "Desconocido";
    long long bitRate = 0;
};
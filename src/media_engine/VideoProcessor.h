#pragma once

#include <string>
#include <functional>

// Estructura para configuración de procesamiento
struct ProcessConfig {
    std::string inputPath;
    std::string outputPath;
    
    // Para recorte temporal
    double startTime = 0.0;  // segundos
    double endTime = -1.0;   // -1 = hasta el final
    
    // Para escalado
    int targetWidth = -1;    // -1 = mantener original
    int targetHeight = -1;
    
    // Para extracción de frames
    int frameInterval = 30;  // cada N frames
    std::string frameOutputDir;
    
    // Para filtros OpenCV
    bool applyEdgeDetection = false;
    bool applyGrayscale = false;
    bool applyBlur = false;
    
    // Callback para progreso (0.0 - 1.0)
    std::function<void(float)> progressCallback;
};

class VideoProcessor {
public:
    VideoProcessor();
    ~VideoProcessor();
    
    // ===== OPERACIONES PRINCIPALES =====
    
    // 1. Conversión de formato
    bool convertFormat(const std::string& inputPath, 
                      const std::string& outputPath,
                      const std::string& format);
    
    // 2. Recorte temporal (trim)
    bool trimVideo(const ProcessConfig& config);
    
    // 3. Cambio de resolución
    bool scaleVideo(const ProcessConfig& config);
    
    // 4. Extracción de frames como imágenes
    bool extractFrames(const ProcessConfig& config);
    
    // 5. Extracción de audio
    bool extractAudio(const std::string& inputPath,
                     const std::string& outputPath);
    
    // 6. Aplicar filtros OpenCV
    bool applyFilters(const ProcessConfig& config);
    
    // 7. Obtener miniatura del video
    bool getThumbnail(const std::string& inputPath,
                     const std::string& outputPath,
                     double timeSeconds = 1.0);
    
    // Obtener último error
    std::string getLastError() const { return lastError; }
    
private:
    std::string lastError;
    
    // Helpers internos
    bool validateInputFile(const std::string& path);
    void updateProgress(float progress, const ProcessConfig& config);
};
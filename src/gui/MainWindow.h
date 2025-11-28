// src/gui/MainWindow.h
#pragma once

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTabWidget>
#include <QLineEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QProgressBar>
#include <QGroupBox>
#include <QComboBox>
#include <QTextEdit>

#include "media_engine/MediaReader.h"
#include "media_engine/VideoProcessor.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Tab 1: Información
    void onBtnInfoOpen();
    void onBtnAnalyze();
    
    // Tab 2: Conversión
    void onBtnConvertSelectInput();
    void onBtnConvertSelectOutput();
    void onBtnConvertExecute();
    
    // Tab 3: Extracción
    void onBtnExtractSelectInput();
    void onBtnExtractFrames();
    void onBtnExtractAudio();
    void onBtnExtractThumbnail();
    
    // Tab 4: Transformación
    void onBtnTransformSelectInput();
    void onBtnTransformSelectOutput();
    void onBtnTransformExecute();
    
    // Tab 5: Filtros
    void onBtnFiltersSelectInput();
    void onBtnFiltersSelectOutput();
    void onBtnFiltersExecute();

private:
    // Motor de procesamiento
    MediaReader mediaReader;
    VideoProcessor videoProcessor;
    
    // UI Principal
    QTabWidget* tabWidget;
    
    // ===== TAB 1: INFORMACIÓN =====
    QWidget* createInfoTab();
    QPushButton* btnInfoOpen;
    QTextEdit* txtInfo;
    QString currentInfoFile;
    
    // ===== TAB 2: CONVERSIÓN =====
    QWidget* createConvertTab();
    QLineEdit* editConvertInput;
    QLineEdit* editConvertOutput;
    QPushButton* btnConvertSelectInput;
    QPushButton* btnConvertSelectOutput;
    QComboBox* comboFormat;
    QPushButton* btnConvertExecute;
    QProgressBar* progressConvert;
    QLabel* lblConvertStatus;
    
    // ===== TAB 3: EXTRACCIÓN =====
    QWidget* createExtractTab();
    QLineEdit* editExtractInput;
    QPushButton* btnExtractSelectInput;
    QSpinBox* spinFrameInterval;
    QLineEdit* editFrameOutputDir;
    QPushButton* btnExtractFrames;
    QPushButton* btnExtractAudio;
    QDoubleSpinBox* spinThumbnailTime;
    QPushButton* btnExtractThumbnail;
    QProgressBar* progressExtract;
    QLabel* lblExtractStatus;
    
    // ===== TAB 4: TRANSFORMACIÓN =====
    QWidget* createTransformTab();
    QLineEdit* editTransformInput;
    QLineEdit* editTransformOutput;
    QPushButton* btnTransformSelectInput;
    QPushButton* btnTransformSelectOutput;
    QDoubleSpinBox* spinStartTime;
    QDoubleSpinBox* spinEndTime;
    QSpinBox* spinTargetWidth;
    QSpinBox* spinTargetHeight;
    QPushButton* btnTransformExecute;
    QProgressBar* progressTransform;
    QLabel* lblTransformStatus;
    
    // ===== TAB 5: FILTROS =====
    QWidget* createFiltersTab();
    QLineEdit* editFiltersInput;
    QLineEdit* editFiltersOutput;
    QPushButton* btnFiltersSelectInput;
    QPushButton* btnFiltersSelectOutput;
    QCheckBox* chkEdgeDetection;
    QCheckBox* chkGrayscale;
    QCheckBox* chkBlur;
    QPushButton* btnFiltersExecute;
    QProgressBar* progressFilters;
    QLabel* lblFiltersStatus;
    
    // Helpers
    void displayInfo(const MediaInfo& info);
    QString selectInputFile();
    QString selectOutputFile(const QString& filter);
    QString selectDirectory();
};
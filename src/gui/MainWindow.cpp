// src/gui/MainWindow.cpp
#include "MainWindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle("Gestor Multimedia Profesional v2.0");
    resize(900, 700);

    // Widget central con pestañas
    tabWidget = new QTabWidget(this);
    setCentralWidget(tabWidget);

    // Crear cada tab
    tabWidget->addTab(createInfoTab(), "📊 Información");
    tabWidget->addTab(createConvertTab(), "🔄 Conversión");
    tabWidget->addTab(createExtractTab(), "📤 Extracción");
    tabWidget->addTab(createTransformTab(), "✂️ Transformación");
    tabWidget->addTab(createFiltersTab(), "🎨 Filtros");

    // Estilo oscuro moderno
    setStyleSheet(R"(
        QMainWindow {
            background: #1e1e1e;
        }
        QTabWidget::pane {
            border: 1px solid #3d3d3d;
            background: #252525;
        }
        QTabBar::tab {
            background: #2d2d2d;
            color: #ffffff;
            padding: 10px 20px;
            margin-right: 2px;
            border-top-left-radius: 4px;
            border-top-right-radius: 4px;
        }
        QTabBar::tab:selected {
            background: #007acc;
        }
        QTabBar::tab:hover {
            background: #3d3d3d;
        }
        QPushButton {
            background: #0e639c;
            color: white;
            border: none;
            padding: 8px 16px;
            border-radius: 4px;
            font-size: 13px;
        }
        QPushButton:hover {
            background: #1177bb;
        }
        QPushButton:pressed {
            background: #005a9e;
        }
        QLineEdit, QSpinBox, QDoubleSpinBox, QComboBox {
            background: #3c3c3c;
            color: white;
            border: 1px solid #555;
            padding: 6px;
            border-radius: 3px;
        }
        QLabel {
            color: #ffffff;
        }
        QTextEdit {
            background: #1e1e1e;
            color: #d4d4d4;
            border: 1px solid #3d3d3d;
            padding: 10px;
        }
        QGroupBox {
            color: #ffffff;
            border: 2px solid #3d3d3d;
            border-radius: 5px;
            margin-top: 10px;
            font-weight: bold;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 10px;
            padding: 0 5px;
        }
        QProgressBar {
            border: 1px solid #3d3d3d;
            border-radius: 3px;
            text-align: center;
            background: #2d2d2d;
            color: white;
        }
        QProgressBar::chunk {
            background: #007acc;
        }
        QCheckBox {
            color: white;
            spacing: 8px;
        }
        QCheckBox::indicator {
            width: 18px;
            height: 18px;
        }
    )");
}

MainWindow::~MainWindow() {}

// ========================================
// TAB 1: INFORMACIÓN
// ========================================

QWidget* MainWindow::createInfoTab() {
    QWidget* tab = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(tab);
    layout->setSpacing(15);
    layout->setContentsMargins(20, 20, 20, 20);

    // Título
    QLabel* title = new QLabel("Analizar Archivo Multimedia");
    QFont titleFont;
    titleFont.setPointSize(14);
    titleFont.setBold(true);
    title->setFont(titleFont);
    layout->addWidget(title);

    // Botón abrir
    btnInfoOpen = new QPushButton("📁 Seleccionar Archivo");
    btnInfoOpen->setMinimumHeight(40);
    connect(btnInfoOpen, &QPushButton::clicked, this, &MainWindow::onBtnInfoOpen);
    layout->addWidget(btnInfoOpen);

    // Área de información
    txtInfo = new QTextEdit();
    txtInfo->setReadOnly(true);
    txtInfo->setPlaceholderText("Selecciona un archivo para ver sus metadatos...");
    layout->addWidget(txtInfo);

    return tab;
}

// ========================================
// TAB 2: CONVERSIÓN
// ========================================

QWidget* MainWindow::createConvertTab() {
    QWidget* tab = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(tab);
    layout->setSpacing(15);
    layout->setContentsMargins(20, 20, 20, 20);

    QLabel* title = new QLabel("Convertir Formato de Video");
    QFont titleFont;
    titleFont.setPointSize(14);
    titleFont.setBold(true);
    title->setFont(titleFont);
    layout->addWidget(title);

    // Input
    QGroupBox* groupInput = new QGroupBox("Archivo de Entrada");
    QHBoxLayout* layoutInput = new QHBoxLayout(groupInput);
    editConvertInput = new QLineEdit();
    editConvertInput->setPlaceholderText("Ruta del archivo de entrada...");
    btnConvertSelectInput = new QPushButton("📁 Seleccionar");
    connect(btnConvertSelectInput, &QPushButton::clicked, this, &MainWindow::onBtnConvertSelectInput);
    layoutInput->addWidget(editConvertInput);
    layoutInput->addWidget(btnConvertSelectInput);
    layout->addWidget(groupInput);

    // Output
    QGroupBox* groupOutput = new QGroupBox("Archivo de Salida");
    QVBoxLayout* layoutOutput = new QVBoxLayout(groupOutput);

    QHBoxLayout* layoutOutputPath = new QHBoxLayout();
    editConvertOutput = new QLineEdit();
    editConvertOutput->setPlaceholderText("Ruta del archivo de salida...");
    btnConvertSelectOutput = new QPushButton("💾 Guardar como");
    connect(btnConvertSelectOutput, &QPushButton::clicked, this, &MainWindow::onBtnConvertSelectOutput);
    layoutOutputPath->addWidget(editConvertOutput);
    layoutOutputPath->addWidget(btnConvertSelectOutput);
    layoutOutput->addLayout(layoutOutputPath);

    QHBoxLayout* layoutFormat = new QHBoxLayout();
    layoutFormat->addWidget(new QLabel("Formato de salida:"));
    comboFormat = new QComboBox();
    comboFormat->addItems({ "mp4", "avi", "mkv", "mov", "webm", "flv" });
    layoutFormat->addWidget(comboFormat);
    layoutFormat->addStretch();
    layoutOutput->addLayout(layoutFormat);

    layout->addWidget(groupOutput);

    // Ejecutar
    btnConvertExecute = new QPushButton("▶️ Convertir Video");
    btnConvertExecute->setMinimumHeight(40);
    connect(btnConvertExecute, &QPushButton::clicked, this, &MainWindow::onBtnConvertExecute);
    layout->addWidget(btnConvertExecute);

    // Progreso
    progressConvert = new QProgressBar();
    progressConvert->setVisible(false);
    layout->addWidget(progressConvert);

    lblConvertStatus = new QLabel("");
    layout->addWidget(lblConvertStatus);

    layout->addStretch();
    return tab;
}

// ========================================
// TAB 3: EXTRACCIÓN
// ========================================

QWidget* MainWindow::createExtractTab() {
    QWidget* tab = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(tab);
    layout->setSpacing(15);
    layout->setContentsMargins(20, 20, 20, 20);

    QLabel* title = new QLabel("Extraer Contenido del Video");
    QFont titleFont;
    titleFont.setPointSize(14);
    titleFont.setBold(true);
    title->setFont(titleFont);
    layout->addWidget(title);

    // Input
    QGroupBox* groupInput = new QGroupBox("Archivo de Entrada");
    QHBoxLayout* layoutInput = new QHBoxLayout(groupInput);
    editExtractInput = new QLineEdit();
    editExtractInput->setPlaceholderText("Selecciona un archivo de video...");
    btnExtractSelectInput = new QPushButton("📁 Seleccionar");
    connect(btnExtractSelectInput, &QPushButton::clicked, this, &MainWindow::onBtnExtractSelectInput);
    layoutInput->addWidget(editExtractInput);
    layoutInput->addWidget(btnExtractSelectInput);
    layout->addWidget(groupInput);

    // Frames
    QGroupBox* groupFrames = new QGroupBox("Extraer Frames como Imágenes");
    QVBoxLayout* layoutFrames = new QVBoxLayout(groupFrames);

    QHBoxLayout* layoutFrameInterval = new QHBoxLayout();
    layoutFrameInterval->addWidget(new QLabel("Extraer cada:"));
    spinFrameInterval = new QSpinBox();
    spinFrameInterval->setRange(1, 1000);
    spinFrameInterval->setValue(30);
    spinFrameInterval->setSuffix(" frames");
    layoutFrameInterval->addWidget(spinFrameInterval);
    layoutFrameInterval->addStretch();
    layoutFrames->addLayout(layoutFrameInterval);

    QHBoxLayout* layoutFrameDir = new QHBoxLayout();
    editFrameOutputDir = new QLineEdit();
    editFrameOutputDir->setPlaceholderText("Carpeta de salida para frames...");
    QPushButton* btnSelectDir = new QPushButton("📂 Seleccionar Carpeta");
    connect(btnSelectDir, &QPushButton::clicked, [this]() {
        QString dir = selectDirectory();
        if (!dir.isEmpty()) editFrameOutputDir->setText(dir);
        });
    layoutFrameDir->addWidget(editFrameOutputDir);
    layoutFrameDir->addWidget(btnSelectDir);
    layoutFrames->addLayout(layoutFrameDir);

    btnExtractFrames = new QPushButton("📸 Extraer Frames");
    connect(btnExtractFrames, &QPushButton::clicked, this, &MainWindow::onBtnExtractFrames);
    layoutFrames->addWidget(btnExtractFrames);

    layout->addWidget(groupFrames);

    // Audio
    QGroupBox* groupAudio = new QGroupBox("Extraer Audio");
    QVBoxLayout* layoutAudio = new QVBoxLayout(groupAudio);
    btnExtractAudio = new QPushButton("🎵 Extraer Audio a MP3");
    connect(btnExtractAudio, &QPushButton::clicked, this, &MainWindow::onBtnExtractAudio);
    layoutAudio->addWidget(btnExtractAudio);
    layout->addWidget(groupAudio);

    // Miniatura
    QGroupBox* groupThumb = new QGroupBox("Extraer Miniatura");
    QVBoxLayout* layoutThumb = new QVBoxLayout(groupThumb);

    QHBoxLayout* layoutThumbTime = new QHBoxLayout();
    layoutThumbTime->addWidget(new QLabel("En el segundo:"));
    spinThumbnailTime = new QDoubleSpinBox();
    spinThumbnailTime->setRange(0.0, 10000.0);
    spinThumbnailTime->setValue(1.0);
    spinThumbnailTime->setSuffix(" seg");
    layoutThumbTime->addWidget(spinThumbnailTime);
    layoutThumbTime->addStretch();
    layoutThumb->addLayout(layoutThumbTime);

    btnExtractThumbnail = new QPushButton("🖼️ Extraer Miniatura");
    connect(btnExtractThumbnail, &QPushButton::clicked, this, &MainWindow::onBtnExtractThumbnail);
    layoutThumb->addWidget(btnExtractThumbnail);

    layout->addWidget(groupThumb);

    // Progreso
    progressExtract = new QProgressBar();
    progressExtract->setVisible(false);
    layout->addWidget(progressExtract);

    lblExtractStatus = new QLabel("");
    layout->addWidget(lblExtractStatus);

    layout->addStretch();
    return tab;
}

// ========================================
// TAB 4: TRANSFORMACIÓN
// ========================================

QWidget* MainWindow::createTransformTab() {
    QWidget* tab = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(tab);
    layout->setSpacing(15);
    layout->setContentsMargins(20, 20, 20, 20);

    QLabel* title = new QLabel("Transformar Video");
    QFont titleFont;
    titleFont.setPointSize(14);
    titleFont.setBold(true);
    title->setFont(titleFont);
    layout->addWidget(title);

    // Input/Output
    QGroupBox* groupIO = new QGroupBox("Archivos");
    QVBoxLayout* layoutIO = new QVBoxLayout(groupIO);

    QHBoxLayout* layoutInput = new QHBoxLayout();
    editTransformInput = new QLineEdit();
    editTransformInput->setPlaceholderText("Archivo de entrada...");
    btnTransformSelectInput = new QPushButton("📁 Entrada");
    connect(btnTransformSelectInput, &QPushButton::clicked, this, &MainWindow::onBtnTransformSelectInput);
    layoutInput->addWidget(editTransformInput);
    layoutInput->addWidget(btnTransformSelectInput);
    layoutIO->addLayout(layoutInput);

    QHBoxLayout* layoutOutput = new QHBoxLayout();
    editTransformOutput = new QLineEdit();
    editTransformOutput->setPlaceholderText("Archivo de salida...");
    btnTransformSelectOutput = new QPushButton("💾 Salida");
    connect(btnTransformSelectOutput, &QPushButton::clicked, this, &MainWindow::onBtnTransformSelectOutput);
    layoutOutput->addWidget(editTransformOutput);
    layoutOutput->addWidget(btnTransformSelectOutput);
    layoutIO->addLayout(layoutOutput);

    layout->addWidget(groupIO);

    // Recorte temporal
    QGroupBox* groupTrim = new QGroupBox("Recorte Temporal");
    QHBoxLayout* layoutTrim = new QHBoxLayout(groupTrim);

    layoutTrim->addWidget(new QLabel("Desde:"));
    spinStartTime = new QDoubleSpinBox();
    spinStartTime->setRange(0.0, 10000.0);
    spinStartTime->setSuffix(" seg");
    layoutTrim->addWidget(spinStartTime);

    layoutTrim->addWidget(new QLabel("Hasta:"));
    spinEndTime = new QDoubleSpinBox();
    spinEndTime->setRange(0.0, 10000.0);
    spinEndTime->setValue(10.0);
    spinEndTime->setSuffix(" seg");
    layoutTrim->addWidget(spinEndTime);
    layoutTrim->addStretch();

    layout->addWidget(groupTrim);

    // Escalado
    QGroupBox* groupScale = new QGroupBox("Cambiar Resolución");
    QHBoxLayout* layoutScale = new QHBoxLayout(groupScale);

    layoutScale->addWidget(new QLabel("Ancho:"));
    spinTargetWidth = new QSpinBox();
    spinTargetWidth->setRange(0, 7680);
    spinTargetWidth->setValue(1920);
    spinTargetWidth->setSuffix(" px");
    layoutScale->addWidget(spinTargetWidth);

    layoutScale->addWidget(new QLabel("Alto:"));
    spinTargetHeight = new QSpinBox();
    spinTargetHeight->setRange(0, 4320);
    spinTargetHeight->setValue(1080);
    spinTargetHeight->setSuffix(" px");
    layoutScale->addWidget(spinTargetHeight);
    layoutScale->addStretch();

    layout->addWidget(groupScale);

    // Ejecutar
    btnTransformExecute = new QPushButton("⚙️ Aplicar Transformaciones");
    btnTransformExecute->setMinimumHeight(40);
    connect(btnTransformExecute, &QPushButton::clicked, this, &MainWindow::onBtnTransformExecute);
    layout->addWidget(btnTransformExecute);

    // Progreso
    progressTransform = new QProgressBar();
    progressTransform->setVisible(false);
    layout->addWidget(progressTransform);

    lblTransformStatus = new QLabel("");
    layout->addWidget(lblTransformStatus);

    layout->addStretch();
    return tab;
}

// ========================================
// TAB 5: FILTROS
// ========================================

QWidget* MainWindow::createFiltersTab() {
    QWidget* tab = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(tab);
    layout->setSpacing(15);
    layout->setContentsMargins(20, 20, 20, 20);

    QLabel* title = new QLabel("Aplicar Filtros con OpenCV");
    QFont titleFont;
    titleFont.setPointSize(14);
    titleFont.setBold(true);
    title->setFont(titleFont);
    layout->addWidget(title);

    // Input/Output
    QGroupBox* groupIO = new QGroupBox("Archivos");
    QVBoxLayout* layoutIO = new QVBoxLayout(groupIO);

    QHBoxLayout* layoutInput = new QHBoxLayout();
    editFiltersInput = new QLineEdit();
    editFiltersInput->setPlaceholderText("Archivo de entrada...");
    btnFiltersSelectInput = new QPushButton("📁 Entrada");
    connect(btnFiltersSelectInput, &QPushButton::clicked, this, &MainWindow::onBtnFiltersSelectInput);
    layoutInput->addWidget(editFiltersInput);
    layoutInput->addWidget(btnFiltersSelectInput);
    layoutIO->addLayout(layoutInput);

    QHBoxLayout* layoutOutput = new QHBoxLayout();
    editFiltersOutput = new QLineEdit();
    editFiltersOutput->setPlaceholderText("Archivo de salida...");
    btnFiltersSelectOutput = new QPushButton("💾 Salida");
    connect(btnFiltersSelectOutput, &QPushButton::clicked, this, &MainWindow::onBtnFiltersSelectOutput);
    layoutOutput->addWidget(editFiltersOutput);
    layoutOutput->addWidget(btnFiltersSelectOutput);
    layoutIO->addLayout(layoutOutput);

    layout->addWidget(groupIO);

    // Filtros
    QGroupBox* groupFilters = new QGroupBox("Seleccionar Filtros");
    QVBoxLayout* layoutFilters = new QVBoxLayout(groupFilters);

    chkEdgeDetection = new QCheckBox("Detección de Bordes (Canny)");
    layoutFilters->addWidget(chkEdgeDetection);

    chkGrayscale = new QCheckBox("Escala de Grises");
    layoutFilters->addWidget(chkGrayscale);

    chkBlur = new QCheckBox("Desenfoque Gaussiano");
    layoutFilters->addWidget(chkBlur);

    layout->addWidget(groupFilters);

    // Ejecutar
    btnFiltersExecute = new QPushButton("🎨 Aplicar Filtros");
    btnFiltersExecute->setMinimumHeight(40);
    connect(btnFiltersExecute, &QPushButton::clicked, this, &MainWindow::onBtnFiltersExecute);
    layout->addWidget(btnFiltersExecute);

    // Progreso
    progressFilters = new QProgressBar();
    progressFilters->setVisible(false);
    layout->addWidget(progressFilters);

    lblFiltersStatus = new QLabel("");
    layout->addWidget(lblFiltersStatus);

    layout->addStretch();
    return tab;
}

// ========================================
// IMPLEMENTACIÓN DE SLOTS
// ========================================

// TAB 1: Información
void MainWindow::onBtnInfoOpen() {
    QString fileName = selectInputFile();
    if (fileName.isEmpty()) return;

    currentInfoFile = fileName;
    MediaInfo info = mediaReader.extractMetadata(fileName.toStdString());
    displayInfo(info);
}

void MainWindow::onBtnAnalyze() {
    // Análisis adicional futuro
}

// TAB 2: Conversión
void MainWindow::onBtnConvertSelectInput() {
    QString file = selectInputFile();
    if (!file.isEmpty()) editConvertInput->setText(file);
}

void MainWindow::onBtnConvertSelectOutput() {
    QString format = comboFormat->currentText();
    QString filter = QString("Videos *.%1 (*.%1)").arg(format);
    QString file = selectOutputFile(filter);
    if (!file.isEmpty()) editConvertOutput->setText(file);
}

void MainWindow::onBtnConvertExecute() {
    QString input = editConvertInput->text();
    QString output = editConvertOutput->text();
    QString format = comboFormat->currentText();

    if (input.isEmpty() || output.isEmpty()) {
        QMessageBox::warning(this, "Error", "Debes seleccionar archivos de entrada y salida");
        return;
    }

    progressConvert->setVisible(true);
    progressConvert->setValue(0);
    lblConvertStatus->setText("Convirtiendo...");

    bool success = videoProcessor.convertFormat(
        input.toStdString(),
        output.toStdString(),
        format.toStdString()
    );

    progressConvert->setValue(100);

    if (success) {
        lblConvertStatus->setText("✅ Conversión completada exitosamente");
        QMessageBox::information(this, "Éxito", "Video convertido correctamente");
    }
    else {
        lblConvertStatus->setText("❌ Error: " + QString::fromStdString(videoProcessor.getLastError()));
        QMessageBox::critical(this, "Error", QString::fromStdString(videoProcessor.getLastError()));
    }

    progressConvert->setVisible(false);
}

// TAB 3: Extracción
void MainWindow::onBtnExtractSelectInput() {
    QString file = selectInputFile();
    if (!file.isEmpty()) editExtractInput->setText(file);
}

void MainWindow::onBtnExtractFrames() {
    QString input = editExtractInput->text();
    QString outputDir = editFrameOutputDir->text();

    if (input.isEmpty() || outputDir.isEmpty()) {
        QMessageBox::warning(this, "Error", "Debes seleccionar archivo de entrada y carpeta de salida");
        return;
    }

    ProcessConfig config;
    config.inputPath = input.toStdString();
    config.frameOutputDir = outputDir.toStdString();
    config.frameInterval = spinFrameInterval->value();

    config.progressCallback = [this](float progress) {
        progressExtract->setValue((int)(progress * 100));
        };

    progressExtract->setVisible(true);
    progressExtract->setValue(0);
    lblExtractStatus->setText("Extrayendo frames...");

    bool success = videoProcessor.extractFrames(config);

    if (success) {
        lblExtractStatus->setText("✅ Frames extraídos exitosamente");
        QMessageBox::information(this, "Éxito", "Frames guardados en: " + outputDir);
    }
    else {
        lblExtractStatus->setText("❌ Error: " + QString::fromStdString(videoProcessor.getLastError()));
        QMessageBox::critical(this, "Error", QString::fromStdString(videoProcessor.getLastError()));
    }

    progressExtract->setVisible(false);
}

void MainWindow::onBtnExtractAudio() {
    QString input = editExtractInput->text();
    if (input.isEmpty()) {
        QMessageBox::warning(this, "Error", "Selecciona un archivo de entrada");
        return;
    }

    QString output = selectOutputFile("Audio MP3 (*.mp3)");
    if (output.isEmpty()) return;

    progressExtract->setVisible(true);
    lblExtractStatus->setText("Extrayendo audio...");

    bool success = videoProcessor.extractAudio(
        input.toStdString(),
        output.toStdString()
    );

    if (success) {
        lblExtractStatus->setText("✅ Audio extraído exitosamente");
        QMessageBox::information(this, "Éxito", "Audio guardado en: " + output);
    }
    else {
        lblExtractStatus->setText("❌ " + QString::fromStdString(videoProcessor.getLastError()));
    }

    progressExtract->setVisible(false);
}

void MainWindow::onBtnExtractThumbnail() {
    QString input = editExtractInput->text();
    if (input.isEmpty()) {
        QMessageBox::warning(this, "Error", "Selecciona un archivo de entrada");
        return;
    }

    QString output = selectOutputFile("Imágenes (*.jpg *.png)");
    if (output.isEmpty()) return;

    bool success = videoProcessor.getThumbnail(
        input.toStdString(),
        output.toStdString(),
        spinThumbnailTime->value()
    );

    if (success) {
        QMessageBox::information(this, "Éxito", "Miniatura guardada");
    }
    else {
        QMessageBox::critical(this, "Error", QString::fromStdString(videoProcessor.getLastError()));
    }
}

// TAB 4: Transformación
void MainWindow::onBtnTransformSelectInput() {
    QString file = selectInputFile();
    if (!file.isEmpty()) editTransformInput->setText(file);
}

void MainWindow::onBtnTransformSelectOutput() {
    QString file = selectOutputFile("Videos (*.mp4 *.avi *.mkv)");
    if (!file.isEmpty()) editTransformOutput->setText(file);
}

void MainWindow::onBtnTransformExecute() {
    QString input = editTransformInput->text();
    QString output = editTransformOutput->text();

    if (input.isEmpty() || output.isEmpty()) {
        QMessageBox::warning(this, "Error", "Selecciona archivos de entrada y salida");
        return;
    }

    ProcessConfig config;
    config.inputPath = input.toStdString();
    config.outputPath = output.toStdString();
    config.startTime = spinStartTime->value();
    config.endTime = spinEndTime->value();
    config.targetWidth = spinTargetWidth->value();
    config.targetHeight = spinTargetHeight->value();

    progressTransform->setVisible(true);
    lblTransformStatus->setText("Procesando transformaciones...");

    // Aquí llamarías a las funciones correspondientes según lo que esté activo
    bool success = false;

    if (config.startTime > 0 || config.endTime > 0) {
        success = videoProcessor.trimVideo(config);
    }
    else if (config.targetWidth > 0 && config.targetHeight > 0) {
        success = videoProcessor.scaleVideo(config);
    }

    if (success) {
        lblTransformStatus->setText("✅ Transformación completada");
        QMessageBox::information(this, "Éxito", "Video transformado correctamente");
    }
    else {
        lblTransformStatus->setText("❌ " + QString::fromStdString(videoProcessor.getLastError()));
    }

    progressTransform->setVisible(false);
}

// TAB 5: Filtros
void MainWindow::onBtnFiltersSelectInput() {
    QString file = selectInputFile();
    if (!file.isEmpty()) editFiltersInput->setText(file);
}

void MainWindow::onBtnFiltersSelectOutput() {
    QString file = selectOutputFile("Videos (*.mp4 *.avi)");
    if (!file.isEmpty()) editFiltersOutput->setText(file);
}

void MainWindow::onBtnFiltersExecute() {
    QString input = editFiltersInput->text();
    QString output = editFiltersOutput->text();

    if (input.isEmpty() || output.isEmpty()) {
        QMessageBox::warning(this, "Error", "Selecciona archivos de entrada y salida");
        return;
    }

    ProcessConfig config;
    config.inputPath = input.toStdString();
    config.outputPath = output.toStdString();
    config.applyEdgeDetection = chkEdgeDetection->isChecked();
    config.applyGrayscale = chkGrayscale->isChecked();
    config.applyBlur = chkBlur->isChecked();

    progressFilters->setVisible(true);
    lblFiltersStatus->setText("Aplicando filtros...");

    bool success = videoProcessor.applyFilters(config);

    if (success) {
        lblFiltersStatus->setText("✅ Filtros aplicados correctamente");
        QMessageBox::information(this, "Éxito", "Video procesado con filtros");
    }
    else {
        lblFiltersStatus->setText("❌ " + QString::fromStdString(videoProcessor.getLastError()));
    }

    progressFilters->setVisible(false);
}

// ========================================
// HELPERS
// ========================================

void MainWindow::displayInfo(const MediaInfo& info) {
    if (!info.isValid) {
        txtInfo->setHtml("<p style='color: #ff6b6b;'><b>❌ Error:</b> No se pudo leer el archivo.</p>");
        return;
    }

    QString html = QString(
        "<div style='font-family: Consolas, monospace;'>"
        "<h2 style='color: #4fc3f7; border-bottom: 2px solid #4fc3f7; padding-bottom: 5px;'>📊 Resultados del Análisis</h2>"

        "<h3 style='color: #81c784; margin-top: 20px;'>📁 Archivo</h3>"
        "<p><b>Ruta:</b> %1</p>"

        "<h3 style='color: #81c784; margin-top: 20px;'>🎬 Información de Video</h3>"
        "<table style='width: 100%%; border-collapse: collapse;'>"
        "<tr><td style='padding: 5px; width: 40%%;'><b>Contenedor:</b></td><td style='padding: 5px;'>%2</td></tr>"
        "<tr><td style='padding: 5px;'><b>Codec de Video:</b></td><td style='padding: 5px;'>%3</td></tr>"
        "<tr><td style='padding: 5px;'><b>Resolución:</b></td><td style='padding: 5px;'>%4 x %5 px</td></tr>"
        "<tr><td style='padding: 5px;'><b>Duración:</b></td><td style='padding: 5px;'>%6 segundos</td></tr>"
        "<tr><td style='padding: 5px;'><b>Bitrate:</b></td><td style='padding: 5px;'>%7 bps (~%8 Mbps)</td></tr>"
        "</table>"
        "</div>"
    ).arg(QString::fromStdString(info.filepath))
        .arg(QString::fromStdString(info.formatName))
        .arg(QString::fromStdString(info.codecName))
        .arg(info.width)
        .arg(info.height)
        .arg(info.durationSec, 0, 'f', 2)
        .arg(info.bitRate)
        .arg(info.bitRate / 1000000.0, 0, 'f', 2);

    txtInfo->setHtml(html);
}

QString MainWindow::selectInputFile() {
    return QFileDialog::getOpenFileName(
        this,
        "Seleccionar Archivo de Video",
        "",
        "Videos (*.mp4 *.avi *.mkv *.mov *.webm *.flv);;Todos los archivos (*.*)"
    );
}

QString MainWindow::selectOutputFile(const QString& filter) {
    return QFileDialog::getSaveFileName(
        this,
        "Guardar Como",
        "",
        filter
    );
}

QString MainWindow::selectDirectory() {
    return QFileDialog::getExistingDirectory(
        this,
        "Seleccionar Carpeta",
        "",
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
    );
}

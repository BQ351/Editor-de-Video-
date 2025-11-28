# V-Make & Edit
Proyecto FFmpeg + opencv + Qt en C++

Hay que entender que en el repositorio solo se encuentra el contenido que fue escrito, los demás archivos y carpetas hacen referencia a los frameworks utilizados o son directamente proporcionales a la compilación del programa.


Análisis de Desarrollo: V-Make & Edit
1. Visión General del Proyecto
Como desarrollador, concebí V-Make & Edit no solo como una herramienta de usuario final, sino como una demostración de capacidad técnica para orquestar bibliotecas de alto rendimiento en un entorno C++ moderno.

Mi objetivo principal fue resolver la complejidad de manipular archivos multimedia (video e imagen) encapsulando la potencia bruta de FFmpeg y los algoritmos de visión de OpenCV bajo una interfaz gráfica limpia y reactiva construida con Qt6.

2. Arquitectura y Stack Tecnológico
La robustez de la aplicación reside en su cimiento, diseñado para ser modular y eficiente:

Lenguaje Core (C++): Elegí C++ (estándar 17) para garantizar la gestión de memoria manual donde es crítico (procesamiento de video frame a frame) y la máxima velocidad de ejecución.

Gestión de Dependencias (Vcpkg): Para evitar el "infierno de DLLs", utilicé Vcpkg. Esto permite que el repositorio se mantenga ligero (solo código fuente), mientras que las librerías pesadas se descargan y vinculan dinámicamente en tiempo de compilación.

Sistema de Construcción (CMake): Implementé CMake para orquestar la compilación, facilitando la detección de rutas de Qt y el enlace con las librerías estáticas/dinámicas de FFmpeg y OpenCV de forma agnóstica al entorno.

3. Desglose de Módulos (Análisis de la Interfaz)
Basándome en la estructura de pestañas que diseñé, la aplicación se divide en flujos de trabajo lógicos:

A. Módulo de Ingesta y Metadatos (Pestaña: Información)
Función: Es el punto de entrada. Aquí implementé un probe que utiliza avformat de FFmpeg para desempaquetar el contenedor del video sin decodificarlo completamente.

Reto técnico: Extraer metadata precisa (codecs, duración, bitrate) sin latencia, permitiendo al usuario validar el archivo antes de procesarlo.

B. Motor de Transcodificación (Pestaña: Conversión)
Función: Permite la conversión de formatos (ej. a .mp4).

Lógica: Este módulo actúa como un wrapper sobre las librerías avcodec y swscale. En lugar de usar llamadas al sistema (como system("ffmpeg...")), integré las llamadas a la API de C de FFmpeg directamente, lo que me da control sobre el buffer y el manejo de errores en tiempo real.

C. Procesamiento y Deconstrucción (Pestaña: Extracción)
Extracción de Frames: Permite descomponer el video en una secuencia de imágenes. Ideal para datasets de IA o análisis forense.

Separación de Audio: Demuxing del stream de audio para exportarlo a MP3.

Miniaturas: Extracción puntual basada en timestamps específicos.

D. Edición No Lineal Básica (Pestaña: Transformación)
Recorte Temporal: Implementé lógica para copiar streams de video entre dos puntos de tiempo (seek) sin necesidad de recodificar todo el video (cuando es posible), o recodificando solo los GOPs necesarios.

Escalado: Uso de swscale para redimensionar la resolución (ej. a 1920x1080), gestionando la interpolación de píxeles para mantener la calidad.

E. Visión Artificial y Post-Procesado (Pestaña: Filtros)
Integración Híbrida: Este es el módulo más complejo. Aquí convierto los frames de FFmpeg (formato YUV) a matrices cv::Mat de OpenCV (BGR).

Filtros Implementados:

Canny (Detección de Bordes): Análisis estructural de la imagen.

Escala de Grises: Reducción de dimensionalidad de color.

Desenfoque Gaussiano: Suavizado de imagen para reducción de ruido.

Flujo: El pipeline es: Decodificar (FFmpeg) -> Convertir a Mat -> Procesar (OpenCV) -> Codificar (FFmpeg) -> Guardar.

4. Conclusión del Desarrollador
V-Make & Edit demuestra que es posible crear software de escritorio nativo de alto nivel en Windows sin depender de frameworks pesados de terceros o electron.

La decisión de usar Vcpkg + CMake fue crucial: asegura que cualquiera que clone el repositorio solo obtenga el código fuente ("lo escrito"), mientras que el entorno de desarrollo se encarga de aprovisionar los gigabytes de librerías binarias necesarios para que FFmpeg y OpenCV funcionen. Es un ejemplo de ingeniería de software disciplinada y moderna.


<img width="1918" height="1078" alt="image" src="https://github.com/user-attachments/assets/c4a6fe18-e540-4be8-8d2f-19d6bafc7ccf" />
<img width="1918" height="1078" alt="image" src="https://github.com/user-attachments/assets/5c830858-a020-4d6f-9e32-35192225051e" />
<img width="1918" height="1078" alt="image" src="https://github.com/user-attachments/assets/8248cf8f-e068-4de9-b668-fb5a9fe01a77" />
<img width="1918" height="1078" alt="image" src="https://github.com/user-attachments/assets/5cc7ae8b-dc43-498f-81ea-200d03d73328" />
<img width="1918" height="1078" alt="image" src="https://github.com/user-attachments/assets/c933b703-c71d-4145-b734-8f89f1325901" />





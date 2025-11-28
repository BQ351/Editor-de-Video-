#include <QApplication>
#include "gui/MainWindow.h"
#include <QPalette> // <-- ¡ESTE ES VITAL!
#include <QColor>   // <-- Asegúrate de tenerlo o usar 'Qt::white', etc.
#include <QStyleFactory> // <-- ¡NUEVO INCLUDE REQUERIDO!


int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    MainWindow window;
    window.show();

    return app.exec();
}
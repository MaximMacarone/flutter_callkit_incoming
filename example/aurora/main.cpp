#include <flutter/flutter_aurora.h>
#include "generated_plugin_registrant.h"
#include <QCoreApplication>
#include <QtDBus/QDBusConnection>
#include <glib.h>

int main(int argc, char *argv[]) {
  aurora::Initialize(argc, argv);

  // Создаём QCoreApplication
  static QCoreApplication qtApp(argc, argv);

  // GSource, который будет «прокачивать» Qt-цикл
  GSource* qtPump = g_timeout_source_new(10);
  // Здесь явно указываем возвращаемый тип gint
  g_source_set_callback(qtPump,
    [](gpointer) -> gint {                // <- важно: gint!
      QCoreApplication::processEvents();
      return G_SOURCE_CONTINUE;           // <- возвращаемое значение типа gint
    },
    nullptr,
    nullptr);
  aurora::AttachEventSource(qtPump);

  aurora::RegisterPlugins();
  aurora::Launch();
  return 0;
}

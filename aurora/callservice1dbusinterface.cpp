// SPDX-FileCopyrightText: 2024 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#include <QtDBus/QDBusReply>
#include <QtDBus/QDBusError>

#include <QDebug>
#include <iostream>

#include "include/flutter_callkit_incoming/callservice1dbusinterface.h"

static const QString s_callService1DBusService = QStringLiteral("org.nemomobile.voicecall");
static const QString s_callService1DBusObject = QStringLiteral("/ru/auroraos/call/service");
static const QString s_callService1DBusInterface = QStringLiteral("ru.auroraos.Call.Service1");

CallService1DBusInterface::CallService1DBusInterface(QObject *parent)
    : QObject(parent), m_isInitialized(false)
{
}

bool CallService1DBusInterface::isInitialized() const
{
    return m_isInitialized;
}

void CallService1DBusInterface::registerCallManager(const QString &dbusObjectPath)
{
    QVariant dbusObjPathVariant = QVariant::fromValue(QDBusObjectPath(dbusObjectPath));
    QDBusReply<void> reply = m_dbusInterface->call("RegisterCallManager", dbusObjPathVariant);
    if (!reply.isValid()) {
        QString message = QStringLiteral("Cannot call %1.RegisterCallManager(%2) method: %3, %4")
                                  .arg(s_callService1DBusInterface, dbusObjectPath,
                                       reply.error().name(), reply.error().message());
        qWarning() << message;
        emit dbusErrorReceived(message);
        std::cout << "\nFail to Register CallManager";
        return;
    }
    qInfo() << QStringLiteral("%1.RegisterCallManager('%2') called successfully")
                       .arg(s_callService1DBusInterface, dbusObjectPath);
    std::cout << "Register CallManager called successfully";
    emit callManagerRegistered();
}

void CallService1DBusInterface::initialize()
{
    m_dbusInterface.reset(new QDBusInterface(s_callService1DBusService, s_callService1DBusObject,
                                             s_callService1DBusInterface,
                                             QDBusConnection::sessionBus(), this));
    if (m_dbusInterface == nullptr) {
        QString message =
                QStringLiteral("DBus-interface %1 is not created").arg(s_callService1DBusService);
        qWarning() << message;
        emit dbusErrorReceived(message);
        return;
    }
    if (!m_dbusInterface->isValid()) {
        QString message = QStringLiteral("DBus-interface %1 is not available")
                                  .arg(s_callService1DBusInterface)
                                  .append("\n")
                                  .append(QStringLiteral("Error: %1 %2")
                                                  .arg(m_dbusInterface->lastError().name(),
                                                       m_dbusInterface->lastError().message()));
        qWarning() << message;
        qWarning() << "Error type:" << m_dbusInterface->lastError().type();
        emit dbusErrorReceived(message);
        return;
    }
    qInfo() << QStringLiteral("DBus-interface %1 is initialized").arg(s_callService1DBusInterface);
    m_isInitialized = true;
    emit dbusInterfaceInitialized();
}

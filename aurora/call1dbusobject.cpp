// SPDX-FileCopyrightText: 2024 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#include <QDebug>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusError>
#include <iostream>

#include "include/flutter_callkit_incoming/call1dbusadaptor.h"

#include "include/flutter_callkit_incoming/call1dbusobject.h"

static const QString s_call1ObjectPath = QStringLiteral("/com/hiennv/AppName/DBus/Call1");

Call1DBusObject::Call1DBusObject(QObject *parent) : QObject(parent)
{
    m_dbusAdaptor = new Call1DBusAdaptor(this);

    connect(this, &Call1DBusObject::StatusChanged, m_dbusAdaptor, &Call1DBusAdaptor::StatusChanged);
}

Call1DBusObject::~Call1DBusObject()
{
    unregisterCall1DBusObject();
}

QDBusObjectPath Call1DBusObject::objectPath()
{
    return QDBusObjectPath(s_call1ObjectPath);
}

void Call1DBusObject::Accept()
{
    qInfo() << QStringLiteral("Call1DBusObject::%1() called").arg(__func__);
    m_params.status = Accepting;
    sendStatus(Accepting);
    m_params.status = Active;
    sendStatus(Active);
}

void Call1DBusObject::Reject(const uint32_t reason)
{
    qInfo() << QStringLiteral("Call1DBusObject::%1(%2) called").arg(__func__).arg(reason);
    m_params.status = Rejecting;
    sendStatus(Rejecting);
    m_params.status = Disconnected;
    sendStatus(Disconnected);
}

void Call1DBusObject::SetHold(const bool hold)
{
    qInfo() << QStringLiteral("Call1DBusObject::%1(%2) called").arg(__func__).arg(hold);
    m_params.status = hold ? Call1DBusObject::Held : Call1DBusObject::Active;
    sendStatus(hold ? Held : Active);
}

const QString Call1DBusObject::statusToString(const uint32_t status)
{
    QMap<int, QString> callStatuses = { { Unknown, QStringLiteral("Unknown (0)") },
                                        { Disconnected, QStringLiteral("Disconnected (1)") },
                                        { Dialing, QStringLiteral("Dialing (2)") },
                                        { Ringing, QStringLiteral("Ringing (3)") },
                                        { Rejecting, QStringLiteral("Rejecting (4)") },
                                        { Accepting, QStringLiteral("Accepting (5)") },
                                        { Active, QStringLiteral("Active (6)") },
                                        { Held, QStringLiteral("Held (7)") } };
    return callStatuses[status];
}

void Call1DBusObject::registerCall1DBusObject(const bool incoming)
{
    if (!QDBusConnection::sessionBus().isConnected()) {
        qWarning() << "No connection to session bus";
    }

    qInfo() << "Registering object at path: " << s_call1ObjectPath;
    qInfo() << "Object parent: " << this->parent();
    qInfo() << "MetaObject name: " << this->metaObject()->className();

    if (!QDBusConnection::sessionBus().registerObject(s_call1ObjectPath, this, QDBusConnection::ExportAdaptors)) {
        QDBusError error = QDBusConnection::sessionBus().lastError();
        qWarning() << error.name();
        qWarning() << error.message();
        qWarning() << error.type();
        //emit dbusErrorReceived(message);
        return;
    }
    qInfo() << QStringLiteral("DBus-object %1 is registered successfully").arg(s_call1ObjectPath);
    sendStatus(incoming ? Ringing : Dialing);
}

void Call1DBusObject::unregisterCall1DBusObject()
{
    QDBusConnection::sessionBus().unregisterObject(s_call1ObjectPath);
}

void Call1DBusObject::sendStatus(const CallStatus status)
{
    qInfo() << "Call status changed to: " << status;
    emit StatusChanged(status);
}

void Call1DBusObject::setParams(AuroraParams params) {
    m_params = params;
}

AuroraParams Call1DBusObject::getParams() {
    return m_params;
}

// SPDX-FileCopyrightText: 2024 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#include <QDebug>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusError>
#include <iostream>

#include "include/flutter_callkit_incoming/call1dbusadaptor.h"

#include "include/flutter_callkit_incoming/call1dbusobject.h"

static const QString s_call1ObjectPath = QStringLiteral("/ru/auroraos/CallApiOverall/DBus/Call1");

/*!
 * \brief Constructor performs following:
 * * Initializes the Call1DBusAdaptor instance with this object as parent.
 * * Sets up the signals connections.
 * * Registers the '/ru/auroraos/CallApiOverall/DBus/Call1' DBus object on the session bus.
 * \param parent QObject parent instance.
 */
Call1DBusObject::Call1DBusObject(QObject *parent) : QObject(parent)
{
    m_dbusAdaptor = new Call1DBusAdaptor(this);
    connect(this, &Call1DBusObject::StatusChanged, m_dbusAdaptor, &Call1DBusAdaptor::StatusChanged);
}

/*!
 * \brief Destructor unregisters the '/ru/auroraos/CallApiOverall/DBus/Call1' DBus-object from the
 * session bus.
 */
Call1DBusObject::~Call1DBusObject()
{
    unregisterCall1DBusObject();
}

QDBusObjectPath Call1DBusObject::objectPath()
{
    return QDBusObjectPath(s_call1ObjectPath);
}

/*!
 * \brief Accepts the incoming call.
 */
void Call1DBusObject::Accept()
{
    qInfo() << QStringLiteral("Call1DBusObject::%1() called").arg(__func__);
    sendStatus(Accepting);
    sendStatus(Active);
}

/*!
 * \brief Rejects the current call with the given reason.
 * \param reason Reason to reject the call: 0 — Unknown, 1 — By User, 2 — By System.
 */
void Call1DBusObject::Reject(const uint32_t reason)
{
    qInfo() << QStringLiteral("Call1DBusObject::%1(%2) called").arg(__func__).arg(reason);
    sendStatus(Rejecting);
    sendStatus(Disconnected);
}

/*!
 * \brief Changes the holding status of the current call.
 * \param hold True if the call should be holded, false — if should be unholded.
 */
void Call1DBusObject::SetHold(const bool hold)
{
    qInfo() << QStringLiteral("Call1DBusObject::%1(%2) called").arg(__func__).arg(hold);
    sendStatus(hold ? Held : Active);
}

/*!
 * \brief Retrieves the string representation of the call status value.
 * \param status CallStatus enum value.
 * \return String representation of the call status value.
 */
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

/*!
 * \brief Registers the '/ru/auroraos/CallApiOverall/DBus/Call1' DBus-object on the session bus.
 * \param incoming True if the call should be incoming, false — if outgoing.
 */
void Call1DBusObject::registerCall1DBusObject(const bool incoming)
{
    std::cout << "[Call1DbusObject] invoked registerCall1DBusObject";
    if (!QDBusConnection::sessionBus().registerObject(s_call1ObjectPath, this)) {
        QString message = QDBusConnection::sessionBus().lastError().message();
        qWarning() << message;
        emit dbusErrorReceived(message);
        return;
    }
    qInfo() << QStringLiteral("DBus-object %1 is registered successfully").arg(s_call1ObjectPath);
    sendStatus(incoming ? Ringing : Dialing);
}

/*!
 * \brief Unregisters the '/ru/auroraos/CallApiOverall/DBus/Call1' DBus-object from the session bus.
 */
void Call1DBusObject::unregisterCall1DBusObject()
{
    QDBusConnection::sessionBus().unregisterObject(s_call1ObjectPath);
}

/*!
 * \brief Sens the StatusChanged() signal with the given status value.
 * \param status Status to send.
 */
void Call1DBusObject::sendStatus(const CallStatus status)
{
    std::cout << "[Call Dbus object] emit status changed";
    emit StatusChanged(status);
}

// SPDX-FileCopyrightText: 2024 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#include <QDebug>

#include "include/flutter_callkit_incoming/call1dbusobject.h"

#include "include/flutter_callkit_incoming/call1dbusadaptor.h"

/*!
 * \brief Default constructor.
 * \param parent QObject parent instance.
 */
Call1DBusAdaptor::Call1DBusAdaptor(QObject *parent)
    : QDBusAbstractAdaptor(parent), m_call1DBusObject(qobject_cast<Call1DBusObject *>(parent))
{
}

/*!
 * \brief Accepts the incoming call using Call1 DBus-object.
 */
void Call1DBusAdaptor::Accept()
{
    qInfo() << QStringLiteral("Call1DBusAdaptor::%1() called").arg(__func__);
    m_call1DBusObject->Accept();
}

/*!
 * \brief Rejects the current call with the given reason using Call1 DBus-object.
 * \param reason Reason to reject the call: 0 — Unknown, 1 — By User, 2 — By System.
 */
void Call1DBusAdaptor::Reject(const uint32_t reason)
{
    qInfo() << QStringLiteral("Call1DBusAdaptor::%1(%2) called").arg(__func__).arg(reason);
    m_call1DBusObject->Reject(reason);
}

/*!
 * \brief Changes the holding status of the current call using Call1 DBus-object.
 * \param hold True if the call should be holded, false — if should be unholded.
 */
void Call1DBusAdaptor::SetHold(const bool hold)
{
    qInfo() << QStringLiteral("Call1DBusAdaptor::%1(%2) called").arg(__func__).arg(hold);
    m_call1DBusObject->SetHold(hold);
}

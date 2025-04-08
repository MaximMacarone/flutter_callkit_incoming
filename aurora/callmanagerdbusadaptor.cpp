// SPDX-FileCopyrightText: 2024 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#include <QDebug>

#include "include/flutter_callkit_incoming/callmanager.h"

#include "include/flutter_callkit_incoming/callmanagerdbusadaptor.h"

/*!
 * \brief Default constructor.
 * \param parent QObject parent instance.
 */
CallManagerDBusAdaptor::CallManagerDBusAdaptor(QObject *parent)
    : QDBusAbstractAdaptor(parent), m_callManager(qobject_cast<CallManager *>(parent))
{
}

/*!
 * \brief Retrieves the registered managed objects.
 * \return DBusManagerStruct instance with info about the managed call objects.
 */
DBusManagerStruct CallManagerDBusAdaptor::GetManagedObjects()
{
    qInfo() << QStringLiteral("CallManagerDBusAdaptor::%1() called").arg(__func__);
    return m_callManager->GetManagedObjects();
}

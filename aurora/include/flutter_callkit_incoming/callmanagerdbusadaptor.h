// SPDX-FileCopyrightText: 2024 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef CALLMANAGERDBUSADAPTOR_H
#define CALLMANAGERDBUSADAPTOR_H

#include <QtDBus/QDBusAbstractAdaptor>
#include <QtDBus/QDBusObjectPath>

#include "dbustypes.h"

class CallManager;

/*!
 * \brief CallManagerDBusAdaptor class is a DBus adaptor for the CallManager object.
 */
class CallManagerDBusAdaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.freedesktop.DBus.ObjectManager")

public:
    explicit CallManagerDBusAdaptor(QObject *parent = nullptr);

public slots:
    // org.freedesktop.DBus.ObjectManager method
    DBusManagerStruct GetManagedObjects();

private:
    CallManager *m_callManager;

signals:
    // org.freedesktop.DBus.ObjectManager signals
    void InterfacesAdded(const QDBusObjectPath &object, const VariantMapMap &interfaces);
    void InterfacesRemoved(const QDBusObjectPath &object, const QStringList &interfaces);
};

#endif // CALLMANAGERDBUSADAPTOR_H

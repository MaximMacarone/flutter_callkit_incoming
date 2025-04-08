// SPDX-FileCopyrightText: 2024 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef CALL1DBUSADAPTOR_H
#define CALL1DBUSADAPTOR_H

#include <QtDBus/QDBusAbstractAdaptor>
#include <QtDBus/QDBusObjectPath>

class Call1DBusObject;

/*!
 * \brief Call1DBusAdaptor class is a DBus adaptor for the Call1 DBus-object.
 */
class Call1DBusAdaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "ru.auroraos.Call.Call1")

public:
    explicit Call1DBusAdaptor(QObject *parent = nullptr);

public slots:
    // ru.auroraos.Call.Call1 methods
    void Accept();
    void Reject(const uint32_t reason);
    void SetHold(const bool hold);

private:
    Call1DBusObject *m_call1DBusObject;

signals:
    // ru.auroraos.Call.Call1 signals
    void StatusChanged(const uint32_t status);
};

#endif // CALL1DBUSADAPTOR_H

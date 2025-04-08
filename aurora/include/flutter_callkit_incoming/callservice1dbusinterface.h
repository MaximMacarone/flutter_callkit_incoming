// SPDX-FileCopyrightText: 2024 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef CALLSERVICE1DBUSINTERFACE_H
#define CALLSERVICE1DBUSINTERFACE_H

#include <QObject>

#include <QtDBus/QDBusInterface>

/*!
 * \brief CallService1DBusInterface class is a proxy-class to work with the
 * ru.auroraos.Call.Service1 DBus-interface:
 * https://developer.auroraos.ru/doc/5.0.0/software_development/reference/call_api/service1.
 */
class CallService1DBusInterface : public QObject
{
    Q_OBJECT

public:
    explicit CallService1DBusInterface(QObject *parent = nullptr);

    // Getters
    bool isInitialized() const;

    void initialize();
    void registerCallManager(const QString &dbusObjectPath);

private:
    QScopedPointer<QDBusInterface> m_dbusInterface;
    bool m_isInitialized;

signals:
    void dbusInterfaceInitialized();
    void callManagerRegistered();

    void dbusErrorReceived(const QString &errorMessage);
};

#endif // CALLSERVICE1DBUSINTERFACE_H

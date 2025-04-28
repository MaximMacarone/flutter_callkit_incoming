// SPDX-FileCopyrightText: 2024 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef CALL1DBUSOBJECT_H
#define CALL1DBUSOBJECT_H

#include <QObject>
#include <QtDBus/QDBusContext>
#include <QtDBus/QDBusObjectPath>

#include "aurora_params.h"

class Call1DBusAdaptor;

class Call1DBusObject : public QObject, public QDBusContext {

    Q_OBJECT

public:
    enum CallStatus {
        Unknown = 0,
        Disconnected,
        Dialing,
        Ringing,
        Rejecting,
        Accepting,
        Active,
        Held
    };
    Q_ENUMS(CallStatus)

    explicit Call1DBusObject(QObject *parent = nullptr);
    ~Call1DBusObject();

    QDBusObjectPath objectPath();
    void registerCall1DBusObject(const bool incoming);
    void unregisterCall1DBusObject();
    void sendStatus(const CallStatus status);
    void setParams(AuroraParams params);
    AuroraParams getParams();

public slots:
    // ru.auroraos.Call.Call1 methods
    void Accept();
    void Reject(const uint32_t reason);
    void SetHold(const bool hold);

public:
    static const QString statusToString(const uint32_t status);

private:
    Call1DBusAdaptor *m_dbusAdaptor;
    // bool m_holdable;
    // bool m_incoming;
    // QString m_localHandle;
    // QString m_localName;
    // QString m_remoteHandle;
    // QString m_remoteName;
    AuroraParams m_params;

signals:
    void dbusErrorReceived(const QString &errorMessage);

signals:
    void StatusChanged(const uint32_t status);
};

#endif // CALL1DBUSOBJECT_H

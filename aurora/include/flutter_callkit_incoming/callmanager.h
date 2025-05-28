// SPDX-FileCopyrightText: 2024 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef CALLMANAGER_H
#define CALLMANAGER_H

#include <QObject>
#include <QTimer>
#include <QtDBus/QDBusObjectPath>
#include <QtDBus/QDBusContext>
#include <QJsonDocument>
#include <QJsonObject>
#include <QVariantMap>
#include <QtDBus/qdbusmetatype.h>

#include <cstdint>
#include <functional>

#include "call_event.h"

#include "dbustypes.h"
#include "callservice1dbusinterface.h"
#include "call1dbusobject.h"
#include "callmanagerdbusadaptor.h"
//#include "earpieceplayer.h"

class CallManagerDBusAdaptor;

class CallManager : public QObject, public QDBusContext
{
    Q_OBJECT

public:
    explicit CallManager(QObject *parent = nullptr);
    ~CallManager();

    void registerCallManager();
    void registerCall1DBusObject(AuroraParams params, bool incoming);

    void startIncomingCall(AuroraParams params);
    void startOutgoingCall(AuroraParams params);

    void setHold(bool isHold);
    void setActive();
    void endCurrentCall();

    void setEventDispatcher(std::function<void(const CallEvent::Event event, const flutter::EncodableMap&)> callback);

public slots:
    // org.freedesktop.DBus.ObjectManager method
    DBusManagerStruct GetManagedObjects();

private:
    void registerDBusObject();
    void unregisterDBusObject();

private slots:
    void sendCall1DialingStatus();
    void sendCall1RingingStatus();
    //void playCallAudioFile();
    void answerOutgoingCall();
    void handleCallStatusChanged(const uint32_t status);
    //void handleAudioResourceGrantedChanged();

private:
    QTimer m_answerTimer;
    CallManagerDBusAdaptor *m_dbusAdaptor;
    Call1DBusObject m_call1DBusObject;
    //EarpiecePlayer m_earpiecePlayer;
    DBusManagerStruct m_dbusManagedObjects;
    Call1DBusObject::CallStatus m_currentCallStatus;

    // std::function<void(const CallEvent::Event event, const QVariantMap&)> m_eventDispatcher;
    std::function<void(const CallEvent::Event event, const flutter::EncodableMap&)> m_eventDispatcher;

signals:
    void callManagerRegistered();
    void dbusErrorReceived(const QString &errorMessage);

signals:
    // org.freedesktop.DBus.ObjectManager signals
    void InterfacesAdded(const QDBusObjectPath &object, const VariantMapMap &interfaces);
    void InterfacesRemoved(const QDBusObjectPath &object, const QStringList &interfaces);
};

#endif // CALLMANAGER_H

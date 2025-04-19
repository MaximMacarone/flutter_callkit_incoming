// SPDX-FileCopyrightText: 2024 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#include <QDebug>
#include <iostream>


#include "include/flutter_callkit_incoming/callmanager.h"

static const QString s_callManagerObjectPath =
        QStringLiteral("/com/hiennv/flutterCallKitIncoming/DBus/ObjectManager");
static const int s_answerInterval = 10 * 1000;

CallManager::CallManager(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<VariantMapMap>("VariantMapMap");
    qDBusRegisterMetaType<VariantMapMap>();

    qRegisterMetaType<DBusManagerStruct>("DBusManagerStruct");
    qDBusRegisterMetaType<DBusManagerStruct>();

    qRegisterMetaType<uint32_t>("uint32_t");
    qDBusRegisterMetaType<uint32_t>();

    m_dbusAdaptor = new CallManagerDBusAdaptor(this);
    connect(this, &CallManager::InterfacesAdded, m_dbusAdaptor,
            &CallManagerDBusAdaptor::InterfacesAdded);
    connect(this, &CallManager::InterfacesRemoved, m_dbusAdaptor,
            &CallManagerDBusAdaptor::InterfacesRemoved);
    connect(&m_call1DBusObject, &Call1DBusObject::StatusChanged, this,
            &CallManager::handleCallStatusChanged);
    //connect(&m_earpiecePlayer, &EarpiecePlayer::resourceGrantedChanged, this,
    //        &CallManager::handleAudioResourceGrantedChanged);
    //connect(&m_answerTimer, &QTimer::timeout, this, &CallManager::answerOutgoingCall);
    registerDBusObject();
}

CallManager::~CallManager()
{
    unregisterDBusObject();
}

void CallManager::registerCallManager()
{
    CallService1DBusInterface callService1;
    connect(&callService1, &CallService1DBusInterface::callManagerRegistered, this,
            &CallManager::callManagerRegistered);
    connect(&callService1, &CallService1DBusInterface::dbusErrorReceived, this,
            &CallManager::dbusErrorReceived);
    callService1.initialize();
    if (!callService1.isInitialized()) {
        std::cout << "Call Service not initialized";
        return;
    }
    callService1.registerCallManager(s_callManagerObjectPath);
}

void CallManager::registerCall1DBusObject(QVariantMap params)
{
    for (auto it = params.begin(); it != params.end(); it++) {
        std::cout << "  " << it.key().toStdString() << ": " << it.value().toString().toStdString() << "\n";
    }

    m_call1DBusObject.registerCall1DBusObject(params.value("incoming").toBool());
    m_dbusManagedObjects[m_call1DBusObject.objectPath()] =
            VariantMapMap{ { "ru.auroraos.Call.Call1", params } };
    emit InterfacesAdded(m_call1DBusObject.objectPath(),
                         m_dbusManagedObjects[m_call1DBusObject.objectPath()]);
}

void CallManager::startIncomingCall(QVariantMap params)
{
    registerCall1DBusObject(params);
}

void CallManager::startOutgoingCall(QVariantMap params)
{
    registerCall1DBusObject(params);
    m_answerTimer.setSingleShot(true);
    m_answerTimer.setInterval(s_answerInterval);
    m_answerTimer.start();
}

DBusManagerStruct CallManager::GetManagedObjects()
{
    qInfo() << QStringLiteral("CallManager::%1() called").arg(__func__);
    return m_dbusManagedObjects;
}

void CallManager::registerDBusObject()
{
    if (!QDBusConnection::sessionBus().registerObject(s_callManagerObjectPath, this)) {
        QString message = QDBusConnection::sessionBus().lastError().message();
        qWarning() << message;
        emit dbusErrorReceived(message);
        std::cout << "\nCallManager::registerDBusObject failed";
        return;
    }
    qInfo() << QStringLiteral("DBus-object %1 is registered successfully")
                       .arg(s_callManagerObjectPath);
    std::cout << "\nCallManager::registerDBusObject succeeded";
}

void CallManager::unregisterDBusObject()
{
    QDBusConnection::sessionBus().unregisterObject(s_callManagerObjectPath);
}

void CallManager::sendCall1DialingStatus()
{
    m_call1DBusObject.sendStatus(Call1DBusObject::Dialing);
}

void CallManager::sendCall1RingingStatus()
{
    m_call1DBusObject.sendStatus(Call1DBusObject::Ringing);
}

// void CallManager::playCallAudioFile()
// {
//     if (!m_earpiecePlayer.resourceGranted())
//         return;
//     if (m_currentCallStatus == Call1DBusObject::Active) {
//         m_earpiecePlayer.play(QStringLiteral("qrc:/audio/active_call.mp3"));
//     } else if (m_currentCallStatus == Call1DBusObject::Held) {
//         m_earpiecePlayer.play(QStringLiteral("qrc:/audio/held_call.mp3"));
//     } else if (m_currentCallStatus == Call1DBusObject::Dialing) {
//         m_earpiecePlayer.play(QStringLiteral("qrc:/audio/outgoing_call.mp3"));
//     }
// }

void CallManager::answerOutgoingCall()
{
    m_call1DBusObject.sendStatus(Call1DBusObject::Accepting);
    m_call1DBusObject.sendStatus(Call1DBusObject::Active);
}

void CallManager::handleCallStatusChanged(const uint32_t status)
{
    qInfo() << QStringLiteral("Call status changed: %1")
                       .arg(Call1DBusObject::statusToString(status));
    m_currentCallStatus = Call1DBusObject::CallStatus(status);
    if (status == Call1DBusObject::Disconnected) {
        m_dbusManagedObjects.remove(m_call1DBusObject.objectPath());
        emit InterfacesRemoved(m_call1DBusObject.objectPath(),
                               QStringList{ "ru.auroraos.Call.Call1" });
        m_call1DBusObject.unregisterCall1DBusObject();
    } else if (status == Call1DBusObject::Rejecting) {
        //m_earpiecePlayer.releaseAudioResource();
    } else if (status == Call1DBusObject::Active || status == Call1DBusObject::Held
               || status == Call1DBusObject::Dialing) {
        //m_earpiecePlayer.grantAudioResource();
    }
}

// void CallManager::handleAudioResourceGrantedChanged()
// {
//     if (m_earpiecePlayer.resourceGranted()) {
//         playCallAudioFile();
//     } else {
//         m_earpiecePlayer.stop();
//     }
// }

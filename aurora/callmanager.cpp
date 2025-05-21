// SPDX-FileCopyrightText: 2024 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#include <QDebug>
#include <QtQuick>
#include <iostream>


#include "include/flutter_callkit_incoming/callmanager.h"

static const QString s_callManagerObjectPath =
        QStringLiteral("/com/hiennv/FlutterCallkitIncoming/DBus/ObjectManager");
static const int s_answerInterval = 10 * 1000;

CallManager::CallManager(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<VariantMapMap>("VariantMapMap");

    qRegisterMetaType<DBusManagerStruct>("DBusManagerStruct");

    qRegisterMetaType<uint32_t>("uint32_t");

    qDBusRegisterMetaType<DBusManagerStruct>();
    qDBusRegisterMetaType<VariantMapMap>();
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
    CallService1DBusInterface callService1 = CallService1DBusInterface(this);
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

void CallManager::registerCall1DBusObject(AuroraParams params, bool incoming)
{
    m_call1DBusObject.setParams(params);
    m_call1DBusObject.registerCall1DBusObject(incoming);
    qInfo() << m_call1DBusObject.getParams().toQVariantMap();
    m_dbusManagedObjects[m_call1DBusObject.objectPath()] =
            VariantMapMap{ { "ru.auroraos.Call.Call1", m_call1DBusObject.getParams().toQVariantMap()} };
    emit InterfacesAdded(m_call1DBusObject.objectPath(),
                         m_dbusManagedObjects[m_call1DBusObject.objectPath()]);
}

void CallManager::startIncomingCall(AuroraParams params)
{
    params.status = Call1DBusObject::Ringing;
    registerCall1DBusObject(params, true);
}

void CallManager::startOutgoingCall(AuroraParams params)
{
    params.status = Call1DBusObject::Dialing;
    registerCall1DBusObject(params, false);
    m_answerTimer.setSingleShot(true);
    m_answerTimer.setInterval(s_answerInterval);
    m_answerTimer.start();
}

DBusManagerStruct CallManager::GetManagedObjects()
{
    qInfo() << QStringLiteral("CallManager::%1() called").arg(__func__);

    qInfo() << "Managed D-Bus Objects:";

    for (const QDBusObjectPath &objectPath : m_dbusManagedObjects.keys()) {
        qInfo() << "Object Path:" << objectPath.path();

        const VariantMapMap &interfaces = m_dbusManagedObjects[objectPath];

        for (const QString &interfaceName : interfaces.keys()) {
            qInfo() << "  Interface:" << interfaceName;
            const QVariantMap &properties = interfaces[interfaceName];

            for (const QString &key : properties.keys()) {
                qInfo() << "    " << key << ":" << properties[key].toString();
            }
        }
    }

    return m_dbusManagedObjects;
}

void CallManager::registerDBusObject()
{
    if (!QDBusConnection::sessionBus().registerObject(s_callManagerObjectPath, this, QDBusConnection::ExportAdaptors)) {
        QString message = QDBusConnection::sessionBus().lastError().message();
        qWarning() << message;
        emit dbusErrorReceived(message);
        return;
    }
    qInfo() << QStringLiteral("DBus-object %1 is registered successfully")
                       .arg(s_callManagerObjectPath);
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
    QVariantMap body;
    AuroraParams params;

    qInfo() << QStringLiteral("Call status changed: %1")
                       .arg(Call1DBusObject::statusToString(status));
                       
    m_currentCallStatus = Call1DBusObject::CallStatus(status);
    switch (status)
    {
    case Call1DBusObject::Unknown:
        /* code */
        break;
    case Call1DBusObject::Disconnected:

        m_dbusManagedObjects.remove(m_call1DBusObject.objectPath());
        emit InterfacesRemoved(m_call1DBusObject.objectPath(),
                            QStringList{ "ru.auroraos.Call.Call1" });
        m_call1DBusObject.unregisterCall1DBusObject();

        m_eventDispatcher(CallEvent::ACTION_CALL_ENDED, body);
        
        break;
    case Call1DBusObject::Dialing:

        params = m_call1DBusObject.getParams();

        qInfo() << "PARAMS: " << params.nameCaller << params.id << params.handle;
        body["nameCaller"] = params.nameCaller;
        body["id"] = params.id;
        body["handle"] = params.handle;

        m_eventDispatcher(CallEvent::ACTION_CALL_START, body);
        
        break;
    case Call1DBusObject::Ringing:

        params = m_call1DBusObject.getParams();

        body["nameCaller"] = params.nameCaller;
        body["id"] = params.id;
        body["handle"] = params.handle;

        m_eventDispatcher(CallEvent::ACTION_CALL_INCOMING, body);
        
        break;
    case Call1DBusObject::Rejecting:
        m_eventDispatcher(CallEvent::ACTION_CALL_DECLINE, body);
        break;
    case Call1DBusObject::Accepting:

        params = m_call1DBusObject.getParams();

        body["nameCaller"] = params.nameCaller;
        body["id"] = params.id;
        body["handle"] = params.handle;
        
        m_eventDispatcher(CallEvent::ACTION_CALL_ACCEPT, body);

        break;
    case Call1DBusObject::Active:
        
        body["isHold"] = m_currentCallStatus == Call1DBusObject::Held ? true : false;
        body["id"] = m_call1DBusObject.getParams().id;
        m_eventDispatcher(CallEvent::ACTION_CALL_TOGGLE_HOLD, body);

        break;
    case Call1DBusObject::Held:

        body["isHold"] = m_currentCallStatus == Call1DBusObject::Held ? true : false;
        body["id"] = m_call1DBusObject.getParams().id;
        m_eventDispatcher(CallEvent::ACTION_CALL_TOGGLE_HOLD, body);

        break;
    default:
        break;
    }
}

void CallManager::setHold(bool isHold) {
    m_call1DBusObject.SetHold(isHold);
}

void CallManager::setActive() {
    if (m_call1DBusObject.getParams().status != Call1DBusObject::CallStatus::Active) {
        m_call1DBusObject.Accept();
    }
}

void CallManager::endCurrentCall() {
    if (m_call1DBusObject.getParams().status != Call1DBusObject::CallStatus::Disconnected) {
        m_call1DBusObject.Reject(0);
    }
}

void CallManager::setEventDispatcher(std::function<void(const CallEvent::Event event, const QVariantMap&)> callback) {
    m_eventDispatcher = std::move(callback);
}

// void CallManager::handleAudioResourceGrantedChanged()
// {
//     if (m_earpiecePlayer.resourceGranted()) {
//         playCallAudioFile();
//     } else {
//         m_earpiecePlayer.stop();
//     }
// }

// SPDX-FileCopyrightText: 2024 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#include <QDebug>
#include <iostream>

#include "include/flutter_callkit_incoming/callmanagerdbusadaptor.h"

#include "include/flutter_callkit_incoming/callmanager.h"

static const QString s_callManagerObjectPath =
        QStringLiteral("/com/hiennv/flutterCallKitIncoming/DBus/ObjectManager");
static const int s_answerInterval = 10 * 1000;


void printQVariantMap(const QVariantMap& map) {
    std::cout << "\n\nAuroraParams:\n\n" << std::endl;
    for (auto it = map.begin(); it != map.end(); ++it) {
        std::cout << "  " << it.key().toStdString() << ": " << it.value().toString().toStdString() << std::endl;
      }
  }


CallManager::CallManager(QObject *parent) : QObject(parent)
{
    m_dbusAdaptor = new CallManagerDBusAdaptor(this);
    connect(this, &CallManager::InterfacesAdded, m_dbusAdaptor,
            &CallManagerDBusAdaptor::InterfacesAdded);
    connect(this, &CallManager::InterfacesRemoved, m_dbusAdaptor,
            &CallManagerDBusAdaptor::InterfacesRemoved);
    connect(&m_call1DBusObject, &Call1DBusObject::StatusChanged, this,
            &CallManager::handleCallStatusChanged);
    //connect(&m_earpiecePlayer, &EarpiecePlayer::resourceGrantedChanged, this,
    //        &CallManager::handleAudioResourceGrantedChanged);
    connect(&m_answerTimer, &QTimer::timeout, this, &CallManager::answerOutgoingCall);
    registerDBusObject();
}

CallManager::~CallManager()
{
    unregisterDBusObject();
}

/*!
 * \brief Calls the 'RegisterCallManager' method of the 'ru.auroraos.Call.Service1' DBus-interface
 * to register the '/ru/auroraos/CallApiOverall/DBus/ObjectManager' DBus-object as call manager.
 */
void CallManager::registerCallManager()
{
    CallService1DBusInterface callService1;
    connect(&callService1, &CallService1DBusInterface::callManagerRegistered, this,
            &CallManager::callManagerRegistered);
    connect(&callService1, &CallService1DBusInterface::dbusErrorReceived, this,
            &CallManager::dbusErrorReceived);
    callService1.initialize();
    if (!callService1.isInitialized())
        return;
    callService1.registerCallManager(s_callManagerObjectPath);
}

/*!
 * \brief Starts the '/ru/auroraos/CallApiOverall/DBus/Call1' DBus object registration.
 * In the end the method emits the InterfacesAdded() signal.
 * \param incoming True if the call should be incoming, false — if outgoing.
 */
void CallManager::registerCall1DBusObject(QVariantMap params)
{
    std::cout << "[Call manager] invoke registerCall1DBusObject\n";
    qDebug() << "Properties:" << params;

    m_call1DBusObject.registerCall1DBusObject(params.value("incoming").toBool());
    m_dbusManagedObjects[m_call1DBusObject.objectPath()] =
            VariantMapMap{ { "ru.auroraos.Call.Call1", params } };
    emit InterfacesAdded(m_call1DBusObject.objectPath(),
                         m_dbusManagedObjects[m_call1DBusObject.objectPath()]);
}

/*!
 * \brief Starts the incoming call by the sending status 'Ringing'.
 */
void CallManager::startIncomingCall(QVariantMap params)
{
    std::cout << "[Call manager] invoke startIncomingCall\n";
    std::cout << "\n\nAuroraParams (before printing):\n\n" << std::endl;
    // Проверяем, что map не пустой
    if (params.isEmpty()) {
        std::cout << "AuroraParams is empty!" << std::endl;
    } else {
        printQVariantMap(params);
    }
    registerCall1DBusObject(params);
    printQVariantMap(params);
    registerCall1DBusObject(params);
}

/*!
 * \brief Starts the incoming call by the sending status 'Dialing'.
 */
void CallManager::startOutgoingCall(QVariantMap params)
{
    registerCall1DBusObject(params);
    m_answerTimer.setSingleShot(true);
    m_answerTimer.setInterval(s_answerInterval);
    m_answerTimer.start();
}

/*!
 * \brief Retrieves the registered managed objects.
 * \return DBusManagerStruct instance with info about the managed call objects.
 */
DBusManagerStruct CallManager::GetManagedObjects()
{
    qInfo() << QStringLiteral("CallManager::%1() called").arg(__func__);
    return m_dbusManagedObjects;
}

/*!
 * \brief Registers the '/ru/auroraos/CallApiOverall/DBus/ObjectManager' DBus object on the session
 * bus.
 */
void CallManager::registerDBusObject()
{
    if (!QDBusConnection::sessionBus().registerObject(s_callManagerObjectPath, this)) {
        QString message = QDBusConnection::sessionBus().lastError().message();
        qWarning() << message;
        emit dbusErrorReceived(message);
        return;
    }
    qInfo() << QStringLiteral("DBus-object %1 is registered successfully")
                       .arg(s_callManagerObjectPath);
}

/*!
 * \brief Unregisters the '/ru/auroraos/CallApiOverall/DBus/ObjectManager' DBus-object from the
 * session bus.
 */
void CallManager::unregisterDBusObject()
{
    QDBusConnection::sessionBus().unregisterObject(s_callManagerObjectPath);
}

/*!
 * \brief Sends the 'Dialing' status for the Call1 DBus-object.
 */
void CallManager::sendCall1DialingStatus()
{
    m_call1DBusObject.sendStatus(Call1DBusObject::Dialing);
}

/*!
 * \brief Sends the 'Ringing' status for the Call1 DBus-object.
 */
void CallManager::sendCall1RingingStatus()
{
    m_call1DBusObject.sendStatus(Call1DBusObject::Ringing);
}

/*!
 * \brief Plays the call audio file for the current call status (active, held or dialing) if the
 * audio resource is granted.
 */
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

/*!
 * \brief Sens the 'Accepting' and 'Active' statuses for the Call1 DBus-object to emulate current
 * outgoing call answer.
 */
void CallManager::answerOutgoingCall()
{
    m_call1DBusObject.sendStatus(Call1DBusObject::Accepting);
    m_call1DBusObject.sendStatus(Call1DBusObject::Active);
}

/*!
 * \brief Handles changing the call status for the current Call1 DBus-object.
 * * Disconnected — removes the managed object, emits the InterfacesRemoved() signal and unregisters
 * the Call1 DBus-object,
 * * Rejecting — releases the current granted audio resource,
 * * Active, Held and Dialing — grants the audio resource to play an audio file in depend on the
 * status.
 * \param status New call status.
 */
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

/*!
 * \brief Handles the audio resource granted status changing.
 * If the audio resource is granted audio file starts playing in earpiece.
 * If the audio resource is not granted the playing is stopped.
 */
// void CallManager::handleAudioResourceGrantedChanged()
// {
//     if (m_earpiecePlayer.resourceGranted()) {
//         playCallAudioFile();
//     } else {
//         m_earpiecePlayer.stop();
//     }
// }

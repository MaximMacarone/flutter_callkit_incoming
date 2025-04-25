#include <flutter_callkit_incoming/flutter_callkit_incoming_plugin.h>
#include <iostream>
#include <QDebug>

namespace Channels {
constexpr auto Methods = "flutter_callkit_incoming";
constexpr auto Events = "flutter_callkit_incoming_events";
} // namespace Channels

namespace Methods {
constexpr auto ShowCallkitIncoming = "showCallkitIncoming";
constexpr auto ShowMissCallNotification = "showMissCallNotification";
constexpr auto HideCallkitIncoming = "hideCallkitIncoming";
constexpr auto StartCall = "startCall";
constexpr auto MuteCall = "muteCall";
constexpr auto IsMuted = "isMuted";
constexpr auto HoldCall = "holdCall";
constexpr auto EndCall = "endCall";
constexpr auto SetCallConnected = "setCallConnected";
constexpr auto EndAllCalls = "endAllCalls";
constexpr auto ActiveCalls = "activeCalls";
constexpr auto GetDevicePushTokenVoIP = "getDevicePushTokenVoIP";
constexpr auto SilenceEvents = "silenceEvents";
constexpr auto RequestNotificationPermission = "requestNotificationPermission";
constexpr auto RequestFullIntentPermission = "requestFullIntentPermission";
} // namespace Methods

void FlutterCallkitIncomingPlugin::RegisterWithRegistrar(
    flutter::PluginRegistrar *registrar) {
  auto methodChannel = std::make_unique<MethodChannel>(
    registrar->messenger(), Channels::Methods,
    &flutter::StandardMethodCodec::GetInstance());
  
  auto eventChannel = std::make_unique<flutter::EventChannel<flutter::EncodableValue>>(
    registrar->messenger(), Channels::Events,
    &flutter::StandardMethodCodec::GetInstance());
    
  auto plugin = std::make_unique<FlutterCallkitIncomingPlugin>(
      std::move(methodChannel), std::move(eventChannel));

  // Register plugin
  registrar->AddPlugin(std::move(plugin));
}

FlutterCallkitIncomingPlugin::FlutterCallkitIncomingPlugin(
    std::unique_ptr<flutter::MethodChannel<flutter::EncodableValue>> methodChannel,
    std::unique_ptr<flutter::EventChannel<flutter::EncodableValue>> eventChannel)
    : m_methodChannel(std::move(methodChannel)),
      m_eventChannel(std::move(eventChannel)) {

  m_callManager = std::make_unique<CallManager>();
  m_callManager->registerCallManager();

  m_callManager->setEventDispatcher(
    [this](const CallEvent::Event event, const QVariantMap &body) {

      auto encBody = QVariantMapToEncodableMap(body);

      sendCallEvent(event, encBody);
    });

  RegisterMethodHandler();
  RegisterEventChannel();
}

void FlutterCallkitIncomingPlugin::RegisterMethodHandler() {
  m_methodChannel->SetMethodCallHandler(
    [this](const MethodCall &call, std::unique_ptr<MethodResult> result) {

      

      if (call.method_name().compare(Methods::ShowCallkitIncoming) == 0) {
        const flutter::EncodableValue* args_ptr = call.arguments();

        result->Success(onShowCallkitIncoming(args_ptr));
        return;

      } else if (call.method_name().compare(Methods::ShowMissCallNotification) == 0) {
        return;
      } else if (call.method_name().compare(Methods::HideCallkitIncoming) == 0) {
        return;
      } else if (call.method_name().compare(Methods::StartCall) == 0) {
        const flutter::EncodableValue* args_ptr = call.arguments();

        result->Success(onStartCall(args_ptr));
        return;

      } else if (call.method_name().compare(Methods::MuteCall) == 0) {
        result->Success();
        return;
      } else if (call.method_name().compare(Methods::IsMuted) == 0) {
        result->Success();
        return;
      } else if (call.method_name().compare(Methods::HoldCall) == 0) {
        result->Success();
        return;
      } else if (call.method_name().compare(Methods::EndCall) == 0) {
        result->Success();
        return;
      } else if (call.method_name().compare(Methods::SetCallConnected) == 0) {
        result->Success();
        return;
      } else if (call.method_name().compare(Methods::EndAllCalls) == 0) {
        result->Success();
        return;
      } else if (call.method_name().compare(Methods::ActiveCalls) == 0) {
        result->Success();
        return;
      } else if (call.method_name().compare(Methods::GetDevicePushTokenVoIP) == 0) {
        result->Success();
        return;
      } else if (call.method_name().compare(Methods::SilenceEvents) == 0) {
        result->Success();
        return;
      } else if (call.method_name().compare(Methods::RequestNotificationPermission) == 0) {
        result->Success();
        return;
      } else if (call.method_name().compare(Methods::RequestFullIntentPermission) == 0) {
        result->Success();
        return;
      } else {
        result->Success();
        return;
      }
    }
  );
}

void FlutterCallkitIncomingPlugin::RegisterEventChannel() {
  m_streamHandler = std::make_unique<flutter::StreamHandlerFunctions<flutter::EncodableValue>>(
      [this](const flutter::EncodableValue* arguments,
             std::unique_ptr<flutter::EventSink<flutter::EncodableValue>>&& events)
          -> std::unique_ptr<flutter::StreamHandlerError<flutter::EncodableValue>> {
        m_eventSink = events.release();
        return nullptr;
      },
      [this](const flutter::EncodableValue* arguments)
          -> std::unique_ptr<flutter::StreamHandlerError<flutter::EncodableValue>> {
        m_eventSink = nullptr;
        return nullptr;
      });

  m_eventChannel->SetStreamHandler(std::move(m_streamHandler));
}

void FlutterCallkitIncomingPlugin::sendCallEvent(
    const CallEvent::Event event,
    const EncodableMap& body) {
  if (!m_eventSink) return;
  flutter::EncodableMap msg;
  msg[flutter::EncodableValue("event")] = flutter::EncodableValue(CallEvent::toString(event).toStdString());
  msg[flutter::EncodableValue("body")] = body;
  qInfo() << "Sending Event";
  m_eventSink->Success(flutter::EncodableValue(msg));
}

EncodableValue FlutterCallkitIncomingPlugin::onShowCallkitIncoming(const EncodableValue *call_arguments) {
  if (!call_arguments) {
    return EncodableValue("Invalid arguments, expected EncodableValue with a Map");
  }
  AuroraParams params = AuroraParams::fromEncodableValue(*call_arguments);
  QVariantMap variantParams = params.toQVariantMap();
  qInfo() << variantParams;
  m_callManager->startIncomingCall(variantParams);
  return EncodableValue("OK");
}

EncodableValue FlutterCallkitIncomingPlugin::onStartCall(const EncodableValue *call_arguments) {
  if (!call_arguments) {
    return EncodableValue("Invalid arguments, expected EncodableValue with a Map");
  }

  AuroraParams params = AuroraParams::fromEncodableValue(*call_arguments);
  QVariantMap variantParams = params.toQVariantMap();
  qInfo() << variantParams;
  m_callManager->startOutgoingCall(variantParams);
  return EncodableValue("OK");
}

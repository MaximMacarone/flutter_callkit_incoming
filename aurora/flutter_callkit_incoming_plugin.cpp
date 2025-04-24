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

  /// Listen to event callback from [FlutterCallkitIncoming].
  ///
  /// FlutterCallkitIncoming.onEvent.listen((event) {
  /// Event.ACTION_CALL_INCOMING - Received an incoming call
  /// Event.ACTION_CALL_START - Started an outgoing call
  /// Event.ACTION_CALL_ACCEPT - Accepted an incoming call
  /// Event.ACTION_CALL_DECLINE - Declined an incoming call
  /// Event.ACTION_CALL_ENDED - Ended an incoming/outgoing call
  /// Event.ACTION_CALL_TIMEOUT - Missed an incoming call
  /// Event.ACTION_CALL_CALLBACK - only Android (click action `Call back` from missed call notification)
  /// Event.ACTION_CALL_TOGGLE_HOLD - only iOS
  /// Event.ACTION_CALL_TOGGLE_MUTE - only iOS
  /// Event.ACTION_CALL_TOGGLE_DMTF - only iOS
  /// Event.ACTION_CALL_TOGGLE_GROUP - only iOS
  /// Event.ACTION_CALL_TOGGLE_AUDIO_SESSION - only iOS
  /// Event.DID_UPDATE_DEVICE_PUSH_TOKEN_VOIP - only iOS
  /// }

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

          const EncodableValue* args_ptr = call.arguments();

          result->Success(FlutterCallkitIncomingPlugin::onShowCallkitIncoming(args_ptr));

        } else if (call.method_name().compare(Methods::ShowMissCallNotification) == 0) {

        } else if (call.method_name().compare(Methods::HideCallkitIncoming) == 0) {

        } else if (call.method_name().compare(Methods::StartCall) == 0) {

          const EncodableValue* args_ptr = call.arguments();

          result->Success(FlutterCallkitIncomingPlugin::onStartCall(args_ptr));

        } else if (call.method_name().compare(Methods::MuteCall) == 0) {

        } else if (call.method_name().compare(Methods::IsMuted) == 0) {

        } else if (call.method_name().compare(Methods::HoldCall) == 0) {

        } else if (call.method_name().compare(Methods::EndCall) == 0) {

        } else if (call.method_name().compare(Methods::SetCallConnected) == 0) {

        } else if (call.method_name().compare(Methods::EndAllCalls) == 0) {

        } else if (call.method_name().compare(Methods::ActiveCalls) == 0) {

        } else if (call.method_name().compare(Methods::GetDevicePushTokenVoIP) == 0) {

        } else if (call.method_name().compare(Methods::SilenceEvents) == 0) {

        } else if (call.method_name().compare(Methods::RequestNotificationPermission) == 0) {

        } else if (call.method_name().compare(Methods::RequestFullIntentPermission) == 0) {

        } else {
          result->Success();
        }
	    });
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
  m_eventSink->Success(flutter::EncodableValue(msg));
}

EncodableValue FlutterCallkitIncomingPlugin::onShowCallkitIncoming(const EncodableValue* call_arguments) {
  if (std::holds_alternative<EncodableMap>(*call_arguments)) {
    const auto& rootMap = std::get<EncodableMap>(*call_arguments);
    auto it = rootMap.find(EncodableValue("aurora"));
    if (it != rootMap.end() && std::holds_alternative<EncodableMap>(it->second)) {
      const auto& auroraMap = std::get<EncodableMap>(it->second);
      std::optional<AuroraParams> auroraParams = ParseAuroraParams(auroraMap);
      if (auroraParams) {
        m_callManager->startIncomingCall(auroraParams->toVariantMap());
        return(EncodableValue("OK"));
      } else {
        return(EncodableValue("Aurora parsing failed"));
      }
    } else {
      return(EncodableValue("No 'aurora' field or it is not a map"));
    }
  } else {
    return(EncodableValue("Invalid argument format"));
  }
}

EncodableValue FlutterCallkitIncomingPlugin::onStartCall(const EncodableValue* call_arguments) {
  if (std::holds_alternative<EncodableMap>(*call_arguments)) {
    const auto& rootMap = std::get<EncodableMap>(*call_arguments);
    auto it = rootMap.find(EncodableValue("aurora"));
    if (it != rootMap.end() && std::holds_alternative<EncodableMap>(it->second)) {
      const auto& auroraMap = std::get<EncodableMap>(it->second);
      std::optional<AuroraParams> auroraParams = ParseAuroraParams(auroraMap);
      if (auroraParams) {
        m_callManager->startOutgoingCall(auroraParams->toVariantMap());
        return(EncodableValue("OK"));
      } else {
        return(EncodableValue("Aurora parsing failed"));
      }
    } else {
      return(EncodableValue("No 'aurora' field or it is not a map"));
    }
  } else {
    return(EncodableValue("Invalid argument format"));
  }
}

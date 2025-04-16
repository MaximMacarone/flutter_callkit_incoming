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

  m_callManager = std::make_unique<CallManager>(nullptr);
  m_callManager->registerCallManager();

  RegisterMethodHandler();
  RegisterEventChannel();
}

void FlutterCallkitIncomingPlugin::RegisterMethodHandler() {
  m_methodChannel->SetMethodCallHandler(
      [this](const MethodCall &call, std::unique_ptr<MethodResult> result) {
        if (call.method_name().compare(Methods::ShowCallkitIncoming) == 0) {
          const EncodableValue* args_ptr = call.arguments();

          if (std::holds_alternative<EncodableMap>(*args_ptr)) {
            const auto& rootMap = std::get<EncodableMap>(*args_ptr);
            auto it = rootMap.find(EncodableValue("aurora"));
            if (it != rootMap.end() && std::holds_alternative<EncodableMap>(it->second)) {
              const auto& auroraMap = std::get<EncodableMap>(it->second);
              std::optional<AuroraParams> auroraParams = ParseAuroraParams(auroraMap);
              if (auroraParams) {
                m_callManager->startIncomingCall(auroraParams->toVariantMap());
                result->Success();
              } else {
                result->Error("Aurora parsing failed");
              }
            } else {
              result->Error("No 'aurora' field or it is not a map");
            }
          } else {
            result->Error("Invalid argument format");
          }






          //result->Success();
        } else {
          // Unknown method
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

EncodableValue FlutterCallkitIncomingPlugin::onStartIncomingCall(const EncodableValue flutterParams, std::unique_ptr<MethodResult> result) {
  std::cout << "BEFORE if (std::holds_alternative<EncodableMap>(flutterParams)) {";
  if (std::holds_alternative<EncodableMap>(flutterParams)) {
    const auto& rootMap = std::get<EncodableMap>(flutterParams);
    auto it = rootMap.find(EncodableValue("aurora"));
    std::cout << "AFTER auto it = rootMap.find(EncodableValue(aurora));";
    if (it != rootMap.end() && std::holds_alternative<EncodableMap>(it->second)) {
      const auto& auroraMap = std::get<EncodableMap>(it->second);
      std::cout << "parsing";
      std::optional<AuroraParams> auroraParams = ParseAuroraParams(auroraMap);
      std::cout << "parsed";
      if (auroraParams) {
        std::cout << "Before starting call";
        //m_callManager->startIncomingCall(auroraParams->toVariantMap());
        std::cout << "After starting call";
        result->Success();
      } else {
        std::cout << "Aurora parsing failed" << std::endl;
        result->Error("Aurora parsing failed");
      }
    } else {
      std::cout << "No 'aurora' field or it is not a map" << std::endl;
      result->Error("No 'aurora' field or it is not a map");
    }
  } else {
    result->Error("Invalid argument format");
  }
}

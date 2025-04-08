#include <flutter_callkit_incoming/flutter_callkit_incoming_plugin.h>
#include <iostream>
#include <QDebug>

namespace Channels {
constexpr auto Methods = "flutter_callkit_incoming";
} // namespace Channels

namespace Methods {
constexpr auto StartIncomingCall = "startIncomingCall";
} // namespace Methods

void FlutterCallkitIncomingPlugin::RegisterWithRegistrar(
    flutter::PluginRegistrar *registrar) {
  auto methodChannel = std::make_unique<MethodChannel>(
      registrar->messenger(), Channels::Methods,
      &flutter::StandardMethodCodec::GetInstance());
  
      auto eventChannel = std::make_unique<flutter::EventChannel<flutter::EncodableValue>>(
        registrar->messenger(),
        "flutter_callkit_incoming_events",
        &flutter::StandardMethodCodec::GetInstance());
    
    auto plugin = std::make_unique<FlutterCallkitIncomingPlugin>(
        std::move(methodChannel), std::move(eventChannel));
    
    registrar->AddPlugin(std::move(plugin));

  // Register plugin
  registrar->AddPlugin(std::move(plugin));
}

FlutterCallkitIncomingPlugin::FlutterCallkitIncomingPlugin(
    std::unique_ptr<flutter::MethodChannel<flutter::EncodableValue>> methodChannel,
    std::unique_ptr<flutter::EventChannel<flutter::EncodableValue>> eventChannel)
    : m_methodChannel(std::move(methodChannel)),
      m_eventChannel(std::move(eventChannel)) {

  m_callManager = std::make_unique<CallManager>(nullptr);

  RegisterMethodHandler();
  RegisterEventChannel();
}

void FlutterCallkitIncomingPlugin::RegisterMethodHandler() {
  m_methodChannel->SetMethodCallHandler(
      [this](const MethodCall &call, std::unique_ptr<MethodResult> result) {
        std::cout << "Searching for method\n";
        std::cout << call.method_name() << "\n";
        if (call.method_name().compare("requestNotificationPermission") == 0) {
          const auto* args = std::get_if<flutter::EncodableMap>(call.arguments());
          if (args) {
            auto it = args->find(flutter::EncodableValue("aurora"));
            if (it != args->end()) {
              const auto* auroraMap = std::get_if<flutter::EncodableMap>(&it->second);
              if (auroraMap) {
                QVariantMap properties = convertAuroraParams(*auroraMap);
                bool incoming = properties.value("incoming").toBool();
                  
                m_callManager->startIncomingCall(properties);

                result->Success(flutter::EncodableValue("OK"));
                return;
              }
            }
            result->Error("Error");
          } else {
            result->Error("Error");
          }
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

QVariantMap FlutterCallkitIncomingPlugin::convertAuroraParams(const flutter::EncodableMap& auroraMap) {
  QVariantMap result;

  auto getString = [&](const std::string& key) -> QString {
    auto it = auroraMap.find(flutter::EncodableValue(key));
    if (it != auroraMap.end()) {
      return QString::fromStdString(std::get<std::string>(it->second));
    }
    return {};
  };

  auto getBool = [&](const std::string& key) -> bool {
    auto it = auroraMap.find(flutter::EncodableValue(key));
    if (it != auroraMap.end()) {
      return std::get<bool>(it->second);
    }
    return false;
  };

  auto getOptionalBool = [&](const std::string& key) -> QVariant {
    auto it = auroraMap.find(flutter::EncodableValue(key));
    if (it != auroraMap.end()) {
      return QVariant(std::get<bool>(it->second));
    }
    return QVariant();
  };

  auto getInt = [&](const std::string& key) -> int {
    auto it = auroraMap.find(flutter::EncodableValue(key));
    if (it != auroraMap.end()) {
      return std::get<int>(it->second);
    }
    return 0;
  };

  result["incoming"] = getBool("incoming");
  result["localHandle"] = getString("localHandle");
  result["localName"] = getString("localName");
  result["remoteHandle"] = getString("remoteHandle");
  result["remoteName"] = getString("remoteName");
  result["holdable"] = getOptionalBool("holdable");
  result["uri"] = getString("uri");
  result["status"] = getInt("status");

  return result;
}

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
        std::cout << call.method_name() << "\n";
        if (call.method_name().compare("showCallkitIncoming") == 0) {
          const EncodableValue* args_ptr = call.arguments();
		  onStartIncomingCall(*args_ptr, std::move(result));
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
    if (std::holds_alternative<EncodableMap>(flutterParams)) {
        const EncodableMap& argsMap = std::get<EncodableMap>(flutterParams);
        auto auroraParams = argsMap.find("aurora");
        if (auroraParams != argsMap.end()) {
            const EncodableValue& auroraValue = auroraParams->second;

            const EncodableMap& auroraMap = std::get<EncodableMap>(auroraValue);

			auto localName = std::get_if<std::string>(&auroraMap.find("localName")->second);
			auto localHandle = std::get_if<std::string>(&auroraMap.find("localHandle")->second);
			auto remoteName = std::get_if<std::string>(&auroraMap.find("remoteName")->second);
			auto remoteHandle = std::get_if<std::string>(&auroraMap.find("remoteHandle")->second);
			auto incoming = std::get_if<std::string>(&auroraMap.find("incoming")->second);
			auto status = std::get_if<std::string>(&auroraMap.find("status")->second);

			if (localName && localHandle && remoteName && remoteHandle && incoming && status) {
                std::cout << "localName: " << *localName
                          << ", localHandle: " << *localHandle
                          << ", remoteName: " << *remoteName
                          << ", remoteHandle: " << *remoteHandle
                          << ", incoming: " << *incoming
                          << ", status: " << *status << std::endl;
            } else {
                std::cout << "One or more expected keys were not found in the 'aurora' map." << std::endl;
                result->Error("Invalid Arguments", "Expected values not found in the 'aurora' map.");
                //return EncodableValue();  // Возвращаем пустое значение
            }

            // for (const auto& entry : auroraMap) {
            //     const std::string* key_str = std::get_if<std::string>(&entry.first);
            //     std::string key = key_str ? *key_str : "unknown";

            //     std::string value_str;
            //     if (std::holds_alternative<std::string>(entry.second)) {
            //         value_str = std::get<std::string>(entry.second);
            //     } else if (std::holds_alternative<bool>(entry.second)) {
            //         bool b = std::get<bool>(entry.second);
            //         value_str = b ? "true" : "false";
            //     } else if (std::holds_alternative<int>(entry.second)) {
            //         value_str = std::to_string(std::get<int>(entry.second));
            //     } else if (std::holds_alternative<int64_t>(entry.second)) {
            //         value_str = std::to_string(std::get<int64_t>(entry.second));
            //     } else {
            //         value_str = "non-string type";
            //     }
            //     std::cout << "Aurora Key: " << key << ", Value: " << value_str << std::endl;
            // }
        } else {
            std::cout << "No 'aurora' key found in the arguments." << std::endl;
        }
	} else {
		result->Error("Err");
	}
}

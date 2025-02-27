#include <flutter_callkit_incoming/flutter_callkit_incoming_plugin.h>

namespace Channels {
constexpr auto Methods = "flutter_callkit_incoming";
} // namespace Channels

namespace Methods {
constexpr auto PlatformVersion = "getPlatformVersion";
constexpr auto 
} // namespace Methods

void FlutterCallkitIncomingPlugin::RegisterWithRegistrar(
    flutter::PluginRegistrar *registrar) {
  // Create MethodChannel with StandardMethodCodec
  auto methodChannel = std::make_unique<MethodChannel>(
      registrar->messenger(), Channels::Methods,
      &flutter::StandardMethodCodec::GetInstance());

  // Create plugin
  std::unique_ptr<FlutterCallkitIncomingPlugin> plugin(
      new FlutterCallkitIncomingPlugin(std::move(methodChannel)));

  // Register plugin
  registrar->AddPlugin(std::move(plugin));
}

FlutterCallkitIncomingPlugin::FlutterCallkitIncomingPlugin(
    std::unique_ptr<MethodChannel> methodChannel)
    : m_methodChannel(std::move(methodChannel)) {
  // Create MethodHandler
  RegisterMethodHandler();
}

void FlutterCallkitIncomingPlugin::RegisterMethodHandler() {
  m_methodChannel->SetMethodCallHandler(
      [this](const MethodCall &call, std::unique_ptr<MethodResult> result) {
        if (call.method_name().compare(Methods::PlatformVersion) == 0) {
          result->Success(onGetPlatformVersion());
        } else {
          result->Success();
        }
      });
}

EncodableValue
FlutterCallkitIncomingPlugin::onGetPlatformVersion() {
  return "Aurora OS.";
}

EncodableValue
FlutterCallkitIncomingPlugin::

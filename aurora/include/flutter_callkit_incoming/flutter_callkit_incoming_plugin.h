#ifndef FLUTTER_PLUGIN_FLUTTER_CALLKIT_INCOMING_PLUGIN_H
#define FLUTTER_PLUGIN_FLUTTER_CALLKIT_INCOMING_PLUGIN_H

#include <flutter_callkit_incoming/globals.h>

#include <flutter/plugin_registrar.h>
#include <flutter/encodable_value.h>
#include <flutter/method_channel.h>
#include <flutter/standard_method_codec.h>

// Flutter encodable
typedef flutter::EncodableValue EncodableValue;
typedef flutter::EncodableMap EncodableMap;
typedef flutter::EncodableList EncodableList;
// Flutter methods
typedef flutter::MethodChannel<EncodableValue> MethodChannel;
typedef flutter::MethodCall<EncodableValue> MethodCall;
typedef flutter::MethodResult<EncodableValue> MethodResult;

class PLUGIN_EXPORT FlutterCallkitIncomingPlugin final : public flutter::Plugin
{
public:
    static void RegisterWithRegistrar(flutter::PluginRegistrar* registrar);

private:
    // Creates a plugin that communicates on the given channel.
    FlutterCallkitIncomingPlugin(
        std::unique_ptr<MethodChannel> methodChannel
    );

    // Methods register handlers channels
    void RegisterMethodHandler();

    // Methods MethodCall
    EncodableValue onGetPlatformVersion();

    // Variables
    std::unique_ptr<MethodChannel> m_methodChannel;
};

#endif /* FLUTTER_PLUGIN_FLUTTER_CALLKIT_INCOMING_PLUGIN_H */

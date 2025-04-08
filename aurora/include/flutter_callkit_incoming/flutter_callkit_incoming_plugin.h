#ifndef FLUTTER_PLUGIN_FLUTTER_CALLKIT_INCOMING_PLUGIN_H
#define FLUTTER_PLUGIN_FLUTTER_CALLKIT_INCOMING_PLUGIN_H

#include </home/maxim/AuroraProjects/flutter_callkit_incoming/aurora/include/flutter_callkit_incoming/globals.h>

#include <flutter/plugin_registrar.h>
#include <flutter/encodable_value.h>
#include <flutter/method_channel.h>
#include <flutter/event_channel.h>
#include <flutter/event_stream_handler.h>
#include <flutter/event_stream_handler_functions.h>
#include <flutter/event_sink.h>
#include <flutter/standard_method_codec.h>

#include "/home/maxim/AuroraProjects/flutter_callkit_incoming/aurora/include/flutter_callkit_incoming/callmanager.h"

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

    // Creates a plugin that communicates on the given channel.
    FlutterCallkitIncomingPlugin(
        std::unique_ptr<flutter::MethodChannel<flutter::EncodableValue>> methodChannel,
        std::unique_ptr<flutter::EventChannel<flutter::EncodableValue>> eventChannel
    );

private:

    // Methods register handlers channels
    void RegisterMethodHandler();
    void RegisterEventChannel();

    // Methods MethodCall
    EncodableValue onStartIncomingCall();

    // Variables
    std::unique_ptr<MethodChannel> m_methodChannel;
    std::unique_ptr<flutter::EventChannel<flutter::EncodableValue>> m_eventChannel;
    std::unique_ptr<flutter::StreamHandlerFunctions<flutter::EncodableValue>> m_streamHandler;
    flutter::EventSink<flutter::EncodableValue>* m_eventSink = nullptr;
    std::unique_ptr<CallManager> m_callManager;

    //Utility
    QVariantMap convertAuroraParams(const flutter::EncodableMap& auroraMap);
};

#endif /* FLUTTER_PLUGIN_FLUTTER_CALLKIT_INCOMING_PLUGIN_H */

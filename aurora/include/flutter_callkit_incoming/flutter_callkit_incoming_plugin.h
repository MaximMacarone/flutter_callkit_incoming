#ifndef FLUTTER_PLUGIN_FLUTTER_CALLKIT_INCOMING_PLUGIN_H
#define FLUTTER_PLUGIN_FLUTTER_CALLKIT_INCOMING_PLUGIN_H

#include <flutter/plugin_registrar.h>
#include <flutter/encodable_value.h>
#include <flutter/method_channel.h>
#include <flutter/event_channel.h>
#include <flutter/event_stream_handler.h>
#include <flutter/event_stream_handler_functions.h>
#include <flutter/event_sink.h>
#include <flutter/standard_method_codec.h>

#include "globals.h"
#include "callmanager.h"
#include "aurora_params.h"

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
    EncodableValue onStartIncomingCall(const EncodableValue flutterParams, std::unique_ptr<MethodResult> result);
    EncodableValue onShowMissCallNotification();
    EncodableValue onHideCallKitIncoming();
    EncodableValue onStartCall();
    EncodableValue onMuteCall();
    EncodableValue onIsMuted();
    EncodableValue onHoldCall();
    EncodableValue onEndCall();
    EncodableValue onSetCallConnected();
    EncodableValue onEndAllCalls();
    EncodableValue onActiveCalls();
    EncodableValue pnGetDevicePushTokenVoIP();
    EncodableValue onSilenceEvents();
    EncodableValue onUnsilenceEvents();
    EncodableValue onRequestNotificationPermission();
    EncodableValue onRequestFullIntentPermission();

    // Variables
    std::unique_ptr<MethodChannel> m_methodChannel;
    std::unique_ptr<flutter::EventChannel<flutter::EncodableValue>> m_eventChannel;
    std::unique_ptr<flutter::StreamHandlerFunctions<flutter::EncodableValue>> m_streamHandler;
    flutter::EventSink<flutter::EncodableValue>* m_eventSink = nullptr;
    std::unique_ptr<CallManager> m_callManager;

    //Utility
};

#endif /* FLUTTER_PLUGIN_FLUTTER_CALLKIT_INCOMING_PLUGIN_H */

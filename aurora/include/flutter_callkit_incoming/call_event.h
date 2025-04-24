#pragma once

#include <QString>

struct CallEvent {
  enum Event {
    ACTION_CALL_INCOMING,
    ACTION_CALL_START,
    ACTION_CALL_ACCEPT,
    ACTION_CALL_DECLINE,
    ACTION_CALL_ENDED,
    ACTION_CALL_TIMEOUT,
    ACTION_CALL_CALLBACK,               // Android only
    ACTION_CALL_TOGGLE_HOLD,           // iOS only
    ACTION_CALL_TOGGLE_MUTE,           // iOS only
    ACTION_CALL_TOGGLE_DMTF,           // iOS only
    ACTION_CALL_TOGGLE_GROUP,          // iOS only
    ACTION_CALL_TOGGLE_AUDIO_SESSION,  // iOS only
    DID_UPDATE_DEVICE_PUSH_TOKEN_VOIP  // iOS only
  };

  static QString toString(Event event) {
    switch (event) {
      case ACTION_CALL_INCOMING:            return QStringLiteral("com.hiennv.flutter_callkit_incoming.ACTION_CALL_INCOMING");
      case ACTION_CALL_START:               return QStringLiteral("com.hiennv.flutter_callkit_incoming.ACTION_CALL_START");
      case ACTION_CALL_ACCEPT:              return QStringLiteral("com.hiennv.flutter_callkit_incoming.ACTION_CALL_ACCEPT");
      case ACTION_CALL_DECLINE:             return QStringLiteral("com.hiennv.flutter_callkit_incoming.ACTION_CALL_DECLINE");
      case ACTION_CALL_ENDED:               return QStringLiteral("com.hiennv.flutter_callkit_incoming.ACTION_CALL_ENDED");
      case ACTION_CALL_TIMEOUT:             return QStringLiteral("com.hiennv.flutter_callkit_incoming.ACTION_CALL_TIMEOUT");
      case ACTION_CALL_CALLBACK:            return QStringLiteral("com.hiennv.flutter_callkit_incoming.ACTION_CALL_CALLBACK");
      case ACTION_CALL_TOGGLE_HOLD:         return QStringLiteral("com.hiennv.flutter_callkit_incoming.ACTION_CALL_TOGGLE_HOLD");
      case ACTION_CALL_TOGGLE_MUTE:         return QStringLiteral("com.hiennv.flutter_callkit_incoming.ACTION_CALL_TOGGLE_MUTE");
      case ACTION_CALL_TOGGLE_DMTF:         return QStringLiteral("com.hiennv.flutter_callkit_incoming.ACTION_CALL_TOGGLE_DMTF");
      case ACTION_CALL_TOGGLE_GROUP:        return QStringLiteral("com.hiennv.flutter_callkit_incoming.ACTION_CALL_TOGGLE_GROUP");
      case ACTION_CALL_TOGGLE_AUDIO_SESSION:return QStringLiteral("com.hiennv.flutter_callkit_incoming.ACTION_CALL_TOGGLE_AUDIO_SESSION");
      case DID_UPDATE_DEVICE_PUSH_TOKEN_VOIP:
                                           return QStringLiteral("com.hiennv.flutter_callkit_incoming.DID_UPDATE_DEVICE_PUSH_TOKEN_VOIP");
    }
    return QStringLiteral("com.hiennv.flutter_callkit_incoming.UNKNOWN_CALL_EVENT");
  }
};

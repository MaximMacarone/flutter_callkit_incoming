import 'dart:async';

import 'package:flutter/material.dart';
import 'package:flutter_callkit_incoming/entities/entities.dart';
import 'package:flutter_callkit_incoming/flutter_callkit_incoming.dart';
import 'package:flutter_callkit_incoming_example/navigation_service.dart';

class CallEventManager {
  static final CallEventManager _instance = CallEventManager._internal();
  factory CallEventManager() => _instance;
  CallEventManager._internal();

  StreamSubscription<CallEvent?>? _callSub;
  NavigationService? _navigationService;

  void Function(String text)? onShowSnackbar;
  // void Function(Map<String, dynamic> callData)? onShowIncomingCallScreen;
  // void Function(Map<String, dynamic> callData)? onShowActiveCallScreen;

  void Function(CallEvent event)? onCallHoldToggled;

  void init({
    required NavigationService navigationService,
    required void Function(String text) onShowSnackbar,
  }) {
    this._navigationService = navigationService;
    this.onShowSnackbar = onShowSnackbar;

    _callSub ??= FlutterCallkitIncoming.onEvent.listen(_onCallEvent);
  }

  void dispose() {
    _callSub?.cancel();
    _callSub = null;
  }

  void _onCallEvent(CallEvent? event) {
    if (event == null) return;
    debugPrint('Received call event: $event');
    switch (event.event) {
      case Event.actionCallAccept:
        _navigationService?.pushReplacementNamed("/active_call", arguments: event.body);
        onShowSnackbar?.call('Вызов принят');
        //onShowActiveCallScreen?.call(event.body);
        break;
      case Event.actionCallDecline:
        _navigationService?.popUntilFirst();
        onShowSnackbar?.call('Вызов отклонён');
        break;
      case Event.actionCallEnded:
        _navigationService?.popUntilFirst();
        onShowSnackbar?.call('Вызов завершён');
        break;
      case Event.actionCallIncoming:
        onShowSnackbar?.call('Входящий вызов');
        _navigationService?.pushNamed("/incoming_call", arguments: event.body);
        break;
      case Event.actionDidUpdateDevicePushTokenVoip:
        // TODO: Handle this case.
        break;
      case Event.actionCallStart:
        // TODO: Handle this case.
        break;
      case Event.actionCallTimeout:
        // TODO: Handle this case.
        break;
      case Event.actionCallCallback:
        // TODO: Handle this case.
        break;
      case Event.actionCallToggleHold:
        onCallHoldToggled?.call(event);
        break;
      case Event.actionCallToggleMute:
        // TODO: Handle this case.
        break;
      case Event.actionCallToggleDmtf:
        // TODO: Handle this case.
        break;
      case Event.actionCallToggleGroup:
        // TODO: Handle this case.
        break;
      case Event.actionCallToggleAudioSession:
        // TODO: Handle this case.
        break;
      case Event.actionCallCustom:
        // TODO: Handle this case.
        break;
    }
  }
}

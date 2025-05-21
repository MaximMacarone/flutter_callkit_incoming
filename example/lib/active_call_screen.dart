import 'dart:async';
import 'dart:convert';

import 'package:flutter/material.dart';
import 'package:flutter_callkit_incoming/entities/entities.dart';
import 'package:flutter_callkit_incoming/flutter_callkit_incoming.dart';
import 'package:flutter_callkit_incoming_example/call_event_manager.dart';
import 'package:web_socket_channel/web_socket_channel.dart';
import 'main.dart';

class ActiveCallScreen extends StatefulWidget {
  final String name;
  final String phone;
  final String callId;
  final WebSocketChannel wsChannel;

  ActiveCallScreen({
    Key? key,
    required this.name,
    required this.phone,
    required this.callId,
    required this.wsChannel
  }) : super(key: key);

  @override
  State<ActiveCallScreen> createState() => _ActiveCallScreenState();
}

class _ActiveCallScreenState extends State<ActiveCallScreen> {
  late Timer _timer;
  int _seconds = 0;
  bool _onHold = false;
  //late StreamSubscription _callEventSubscription;

  @override
  void initState() {
    super.initState();
    _startTimer();

    CallEventManager().onCallHoldToggled = _handleCallHeld;
  }

  // void endCall() {
  //       final payload = [
  //     'end_call',
  //     <String, String> {
  //       'id': widget.callId
  //     }
  //   ];
  //   final json = jsonEncode(payload);
  //   widget.wsChannel.sink.add(json);
  // }

  void _startTimer() {
    _timer = Timer.periodic(const Duration(seconds: 1), (timer) {
      setState(() {
        _seconds++;
      });
    });
  }

  void _handleCallHeld(CallEvent event) {
    final eventName = event.event;
    if (eventName == Event.actionCallToggleHold) {
      final eventBody = event.body;
      if (eventBody['id'] == widget.callId) {
        setState(() {
          _onHold = eventBody["isHold"];
        });
      }
    }
  }

  @override
  void dispose() {
    _timer.cancel();
    super.dispose();
    //_callEventSubscription.cancel();
  }

  String _formatDuration(int seconds) {
    final minutes = (seconds ~/ 60).toString().padLeft(2, '0');
    final secs = (seconds % 60).toString().padLeft(2, '0');
    return '$minutes:$secs';
  }

  void _toggleHold() {
    setState(() => _onHold = !_onHold);

    if (_onHold) {
      _timer.cancel();
    } else {
      _startTimer();
    }

    FlutterCallkitIncoming.holdCall(widget.callId, _onHold, isOnHold: _onHold);
  }

  void _endCall() {
    final payload = [
      'end_call',
      <String, String> {
        'id': widget.callId
      }
    ];
    final json = jsonEncode(payload);
    widget.wsChannel.sink.add(json);
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      backgroundColor: Colors.white,
      body: Center(
        child: Padding(
          padding: const EdgeInsets.all(32),
          child: Column(
            mainAxisAlignment: MainAxisAlignment.center,
            children: [
              const CircleAvatar(
                radius: 50,
                child: Icon(Icons.person, size: 50),
              ),
              const SizedBox(height: 20),
              Text(
                widget.name,
                style: const TextStyle(fontSize: 28, fontWeight: FontWeight.bold),
              ),
              const SizedBox(height: 8),
              Text(
                widget.phone,
                style: const TextStyle(fontSize: 18),
              ),
              const SizedBox(height: 30),
              Text(
                _formatDuration(_seconds),
                style: const TextStyle(fontSize: 24, fontWeight: FontWeight.w600),
              ),
              const SizedBox(height: 40),
              Row(
                mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                children: [
                  ElevatedButton.icon(
                    style: ElevatedButton.styleFrom(backgroundColor: Colors.blue),
                    icon: Icon(_onHold ? Icons.play_arrow : Icons.pause),
                    label: Text(_onHold ? 'Возобновить' : 'Удержать'),
                    onPressed: _toggleHold,
                  ),
                  ElevatedButton.icon(
                    style: ElevatedButton.styleFrom(backgroundColor: Colors.red),
                    icon: const Icon(Icons.call_end),
                    label: const Text('Завершить'),
                    onPressed: _endCall,
                  ),
                ],
              ),
            ],
          ),
        ),
      ),
    );
  }
}

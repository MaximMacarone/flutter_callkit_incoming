import 'package:flutter/material.dart';
import 'package:flutter_callkit_incoming/flutter_callkit_incoming.dart';

class IncomingCallScreen extends StatelessWidget {
  final String name;
  final String phone;
  final String callId;

  const IncomingCallScreen({
    Key? key,
    required this.name,
    required this.phone,
    required this.callId,
  }) : super(key: key);

  void _acceptCall() {
    FlutterCallkitIncoming.setCallConnected(callId);
  }

  void _declineCall() {
    FlutterCallkitIncoming.endCall(callId);
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
                name,
                style: const TextStyle(fontSize: 28, fontWeight: FontWeight.bold),
              ),
              const SizedBox(height: 8),
              Text(
                phone,
                style: const TextStyle(fontSize: 18),
              ),
              const SizedBox(height: 40),
              Row(
                mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                children: [
                  ElevatedButton.icon(
                    style: ElevatedButton.styleFrom(backgroundColor: Colors.red),
                    icon: const Icon(Icons.call_end),
                    label: const Text('Отклонить'),
                    onPressed: _declineCall,
                  ),
                  ElevatedButton.icon(
                    style: ElevatedButton.styleFrom(backgroundColor: Colors.green),
                    icon: const Icon(Icons.call),
                    label: const Text('Принять'),
                    onPressed: _acceptCall,
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

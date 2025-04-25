import 'dart:async';

import 'package:flutter/material.dart';
import 'package:flutter_callkit_incoming/entities/entities.dart';
import 'package:flutter_callkit_incoming/flutter_callkit_incoming.dart';
import 'package:uuid/uuid.dart';

import 'contact.dart';

void main() => runApp(ContactApp());

class ContactApp extends StatefulWidget {
  @override
  _ContactAppState createState() => _ContactAppState();
}

class _ContactAppState extends State<ContactApp> {
  final List<Contact> contacts = Contact.mockContacts;

  static final String userName = 'Максим';
  static final String userPhone = '+7 900 000-00-00';

  Map<String, dynamic>? _activeCall;

  final _messengerKey = GlobalKey<ScaffoldMessengerState>();
  StreamSubscription<CallEvent?>? _callSub;

  @override
  void initState() {
    super.initState();
    _callSub = FlutterCallkitIncoming.onEvent.listen(_onCallEvent);
  }

  @override
  void dispose() {
    _callSub?.cancel();
    super.dispose();
  }

  void _onCallEvent(CallEvent? event) {
    if (event == null) return;
    debugPrint('Received call event: $event');
    switch (event.event) {
      case Event.actionCallAccept:
        _showSnackbar('Вызов принят');
        break;
      case Event.actionCallDecline:
        _showSnackbar('Вызов отклонён');
        break;
      case Event.actionCallEnded:
        _showSnackbar('Вызов завершён');
        break;
      case Event.actionDidUpdateDevicePushTokenVoip:
        // TODO: Handle this case.
        break;
      case Event.actionCallIncoming:
        _showSnackbar('Входящий вызов');
        // TODO: Handle this case.
        break;
      case Event.actionCallStart:
        _showSnackbar('Начат вызов');
        // TODO: Handle this case.
        break;
      case Event.actionCallTimeout:
        // TODO: Handle this case.
        break;
      case Event.actionCallCallback:
        // TODO: Handle this case.
        break;
      case Event.actionCallToggleHold:
        // TODO: Handle this case.
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

  void _showSnackbar(String text) {
    if (!mounted) return;
    final messenger = _messengerKey.currentState;
    messenger?.hideCurrentSnackBar();
    messenger?.showSnackBar(SnackBar(content: Text(text)));
  }

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      scaffoldMessengerKey: _messengerKey,
      title: 'Контакты',
      theme: ThemeData(primarySwatch: Colors.teal),
      home: Scaffold(
        appBar: AppBar(title: const Text('Контакты')),
        body: Column(
          children: [
            Card(
              margin: const EdgeInsets.all(12),
              child: ListTile(
                leading: const CircleAvatar(
                  radius: 24,
                  child: Icon(Icons.person),
                ),
                title: Text(userName),
                subtitle: Text(userPhone),
                trailing: Text('Контактов: ${contacts.length}'),
                onTap: _startIncomingCall,
              ),
            ),
            ElevatedButton.icon(
              onPressed: _showActiveCall,
              icon: const Icon(Icons.call),
              label: const Text('Показать активный звонок'),
            ),
            if (_activeCall != null)
              Card(
                margin: const EdgeInsets.symmetric(horizontal: 12, vertical: 8),
                color: Colors.green.shade50,
                child: ListTile(
                  leading: const Icon(Icons.call),
                  title: Text(_activeCall!['nameCaller'] ?? 'Неизвестно'),
                  subtitle: Text(_activeCall!['handle'] ?? ''),
                  trailing: const Text('Активный'),
                ),
              ),
            Expanded(
              child: ListView.builder(
                itemCount: contacts.length,
                itemBuilder: (context, index) {
                  final contact = contacts[index];
                  return ListTile(
                    leading: const Icon(Icons.person_outline),
                    title: Text(contact.name),
                    subtitle: Text(contact.phone),
                    onTap: () => _callContact(contact),
                  );
                },
              ),
            ),
          ],
        ),
      ),
    );
  }

  void _callContact(Contact contact) {
    _showSnackbar('Звонок: ${contact.name} (${contact.phone})');
    debugPrint('Calling ${contact.name} at ${contact.phone}');
    _makeCall(contact);
  }

  void _makeCall(Contact contact) {
    final params = CallKitParams(
      id: Uuid().v4().toString(),
      nameCaller: contact.name,
      handle: contact.phone,
      extra: {'userID': contact.id},
      aurora: const AuroraParams(
        localName: "Maxim",
        localHandle: "+7 900 000-00-00",
        holdable: true,
        uri: null,
      ),
    );
    FlutterCallkitIncoming.startCall(params);
  }

    void _showActiveCall() async {
    try {
      final calls = await FlutterCallkitIncoming.activeCalls();
      debugPrint('Active calls: $calls');

      if (calls is List && calls.isNotEmpty) {
        setState(() {
          _activeCall = Map<String, dynamic>.from(calls.first);
        });
        _showSnackbar('Найден активный звонок');
      } else {
        setState(() => _activeCall = null);
        _showSnackbar('Нет активных звонков');
      }
    } catch (e) {
      debugPrint('Ошибка при получении активных звонков: $e');
      _showSnackbar('Ошибка при получении звонков');
    }
  }

  void _startIncomingCall() {
    final contact = Contact.mockContacts[0];
    final params = CallKitParams(
      id: Uuid().v4().toString(),
      nameCaller: contact.name,
      handle: contact.phone,
      extra: {'userID': contact.id},
      aurora: const AuroraParams(
        localName: "Maxim",
        localHandle: "+7 900 000-00-00",
        holdable: true,
        uri: null,
      ),
    );
    FlutterCallkitIncoming.showCallkitIncoming(params);
  }
}

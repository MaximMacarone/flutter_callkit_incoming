import "dart:async";

import 'package:flutter/material.dart';
import 'package:flutter_callkit_incoming/entities/entities.dart';
import 'package:flutter_callkit_incoming/flutter_callkit_incoming.dart';
import 'package:uuid/uuid.dart';

import 'contact.dart';

void main() => runApp(ContactApp());



class ContactApp extends StatelessWidget {
  final List<Contact> contacts = Contact.mockContacts;

  static final String userName = 'Максим';
  static final String userPhone = '+7 900 000-00-00';

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
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
                    onTap: () => _callContact(context, contact),
                  );
                },
              ),
            ),
          ],
        ),
      ),
    );
  }

  void _callContact(BuildContext context, Contact contact) {
    final snackBar = SnackBar(
      content: Text('Звонок: ${contact.name} (${contact.phone})'),
      duration: const Duration(seconds: 2),
    );

    ScaffoldMessenger.of(context).showSnackBar(snackBar);
    debugPrint('Calling ${contact.name} at ${contact.phone}');
  }
}

  Future<void> requestNotificationPermission() async {
    await FlutterCallkitIncoming.requestNotificationPermission({
      "rationaleMessagePermission":
          "Notification permission is required, to show notification.",
      "postNotificationMessageRequired":
          "Notification permission is required, Please allow notification permission from setting."
    });
  }

  // Future<dynamic> initCurrentCall() async {
  //   await requestNotificationPermission();
  //   //check current call from pushkit if possible
  //   var calls = await FlutterCallkitIncoming.activeCalls();
  //   if (calls is List) {
  //     if (calls.isNotEmpty) {
  //       print('DATA: $calls');
  //       _currentUuid = calls[0]['id'];
  //       return calls[0];
  //     } else {
  //       _currentUuid = "";
  //       return null;
  //     }
  //   }
  // }

  Future<void> makeCall(Contact contact) async {

      final callParams = CallKitParams(
        id: Uuid().toString(),
        nameCaller: contact.name,
        handle: contact.phone,
        extra: {
          "userID": contact.id
        },
        aurora: const AuroraParams(
          localName: "Maxim",
          localHandle: "+7 900 000-00-00",
          holdable: true,
          uri: null,
          status: CallStatus.dialing
        )
      );
      await FlutterCallkitIncoming.showCallkitIncoming(callParams);
  }

  // Future<void> endCurrentCall() async {
  //   initCurrentCall();
  //   await FlutterCallkitIncoming.endCall(_currentUuid!);
  // }

  // Future<void> startOutGoingCall() async {
  //   _currentUuid = _uuid.v4();
  //   final params = CallKitParams(
  //     id: _currentUuid,
  //     nameCaller: 'Hien Nguyen',
  //     handle: '0123456789',
  //     type: 1,
  //     extra: <String, dynamic>{'userId': '1a2b3c4d'},
  //     ios: const IOSParams(handleType: 'number'),
  //     aurora: const AuroraParams(incoming: false, localHandle: "+79155364844", localName: "Maxim Makarenkov", remoteHandle: "+79991234567", remoteName: "Nail Nuriev", holdable: true, uri: null, status: CallStatus.dialing)
  //   );
  //   await FlutterCallkitIncoming.startCall(params);
  // }

  Future<void> activeCalls() async {
    var calls = await FlutterCallkitIncoming.activeCalls();
    print(calls);
  }

  Future<void> endAllCalls() async {
    await FlutterCallkitIncoming.endAllCalls();
  }

  Future<void> getDevicePushTokenVoIP() async {
    var devicePushTokenVoIP =
        await FlutterCallkitIncoming.getDevicePushTokenVoIP();
    print(devicePushTokenVoIP);
  }

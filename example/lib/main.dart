import 'dart:async';
import 'dart:convert';

import 'package:flutter/material.dart';
import 'package:flutter_callkit_incoming/entities/entities.dart';
import 'package:flutter_callkit_incoming/flutter_callkit_incoming.dart';
import 'package:flutter_callkit_incoming_example/call_event_manager.dart';
import 'package:flutter_callkit_incoming_example/login_screen.dart';
import 'package:flutter_callkit_incoming_example/navigation_service.dart';
import 'package:uuid/uuid.dart';
import 'package:web_socket_channel/web_socket_channel.dart';

import 'contact.dart';
import 'incoming_screen.dart';
import 'active_call_screen.dart';

void main() {
  WidgetsFlutterBinding.ensureInitialized();
  final navService = NavigationService();

  // CallEventManager().init(
  //   navigationService: navService,
  //   webSocketChannel: null,
  //   onShowSnackbar: (_) {}
  // );
  runApp(MyApp(navigationService: navService));
}

class MyApp extends StatelessWidget {
  final NavigationService navigationService;
  const MyApp({super.key, required this.navigationService});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      navigatorKey: navigationService.navigatorKey,
      initialRoute: '/login',
      onGenerateRoute: (settings) {
        switch (settings.name) {
          case '/login':
            return MaterialPageRoute(
              builder: (_) => LoginScreen(onLogin: (id, name, phone) {
                navigationService.pushReplacementNamed(
                  '/contacts',
                  arguments: {'id': id, 'name': name, 'phone': phone},
                );
              }),
            );
          case '/contacts':
            final args = settings.arguments as Map<String, dynamic>;
            return MaterialPageRoute(
              builder: (_) => ContactApp(
                userID:  args['id'] as String,
                userName: args['name'] as String,
                userPhone: args['phone'] as String,
                navigationService: navigationService,
              ),
            );

          case '/incoming_call':
            final args = settings.arguments as Map<String, dynamic>;
            return MaterialPageRoute(
              fullscreenDialog: true,
              builder: (_) => IncomingCallScreen(
                name: args['nameCaller'] as String,
                phone: args['handle'] as String,
                callId: args['id'] as String,
              ),
            );

          case '/active_call':
            final args = settings.arguments as Map<String, dynamic>;
            return MaterialPageRoute(
              fullscreenDialog: true,
              builder: (_) => ActiveCallScreen(
                name: args['nameCaller'] as String,
                phone: args['handle'] as String,
                callId: args['id'] as String,
              ),
            );

          default:
            return null;
        }
      },
    );
  }
}

class ContactApp extends StatefulWidget {
  final String userID;
  final String userName;
  final String userPhone;
  final NavigationService navigationService;

  ContactApp({
    super.key,
    required this.userID,
    required this.userName,
    required this.userPhone,
    required this.navigationService,
  });

  @override
  State<ContactApp> createState() => _ContactAppState();
}

class _ContactAppState extends State<ContactApp> {
  List<Contact> contacts = [];
  bool _connected = false;
  final _messengerKey = GlobalKey<ScaffoldMessengerState>();
  late WebSocketChannel _wsChannel;

  @override
  void initState() {
    super.initState();
    _connectWebSocket();
    CallEventManager().init(
      navigationService: widget.navigationService,
      webSocketChannel: _wsChannel,
      onShowSnackbar: _showSnackbar,
    );

  }

  @override
  void dispose() {
    print("disposing");
    CallEventManager().dispose();
    _wsChannel.sink.close();
    super.dispose();
  }

  void _showSnackbar(String text) {
    _messengerKey.currentState
      ?..hideCurrentSnackBar()
      ..showSnackBar(SnackBar(content: Text(text)));
  }

  void _connectWebSocket() {
    final uri = Uri.parse('ws://192.168.1.229:8000/ws/${widget.userID}');
    print("Trying to connect");
    _wsChannel = WebSocketChannel.connect(uri);

    _wsChannel.stream.listen(
      (message) {
        final decoded = jsonDecode(message);

        if (decoded is List && decoded.length == 2) {
          final type = decoded[0];
          final payload = decoded[1];

          if(type == "update_contacts") {
            print("updating contacts");
            final List<Contact> onlineContacts = (payload as List).map((e) => Contact(id: e["id"], name: e["name"], phone: e["phone"])).toList();
            setState(() {
              contacts = onlineContacts;
            });
          }
          if (type == "incoming_call") {
            print("incoming call");
            final params = CallKitParams.fromJson(payload);
            print(params);
            _startIncomingCall(params: params);
          }
        }
      },
      onDone: () {
        print('Connection closed');
        setState(() {
          contacts = [];
          _connected = false;
        });
      },
      onError: (error) {
        print("Connection error: $error");
        setState(() {
          contacts = [];
          _connected = false;
        });
      }
    );
    setState(() {
      _connected = true;
    });
  }

  Future<void> _makeCall(Contact contact) async {
    final params = CallKitParams(
      id: Uuid().v4(),
      nameCaller: contact.name,
      handle: contact.phone,
      extra: {
        'remote_id': contact.id,
        'local_id': widget.userID,
        },
      aurora: AuroraParams(
        localName: widget.userName,
        localHandle: widget.userPhone,
        holdable: true,
        uri: null,
      ),
    );
    await FlutterCallkitIncoming.startCall(params);
    wsMakeCall(params: params);
  }

  void _startIncomingCall({required CallKitParams params}) {
    FlutterCallkitIncoming.showCallkitIncoming(params);
  }

  @override
  Widget build(BuildContext context) {
    return ScaffoldMessenger(
      key: _messengerKey,
      child: Scaffold(
        appBar: AppBar(title: Text('Контакты — ${widget.userName}')),
        body: Column(
          children: [
            Card(
              margin: const EdgeInsets.all(12),
              child: ListTile(
                leading: const CircleAvatar(child: Icon(Icons.person)),
                title: Text(widget.userName),
                subtitle: Text(widget.userPhone),
                trailing: Text('Контактов: ${contacts.length}'),
              ),
            ),
            Expanded(
              child: ListView.builder(
                itemCount: contacts.length,
                itemBuilder: (_, i) {
                  final c = contacts[i];
                  return ListTile(
                    leading: const Icon(Icons.person_outline),
                    title: Text(c.name),
                    subtitle: Text(c.phone),
                    onTap: () => _makeCall(c),
                  );
                },
              ),
            ),
            Padding(
              padding: const EdgeInsets.all(16.0),
              child: Column(
                children: [
                  Text( _connected ? "Подключено" : "Нет соединения с сервером",
                  style: const TextStyle(fontSize: 18),
                  ),
                  if (!_connected)
                  SizedBox(
                    child: ElevatedButton(
                      onPressed: _connectWebSocket,
                      child: const Text("Переподключиться"),
                    ),
                  )
                ],
              )
            ),
          ],
        ),
      ),
    );
  }

  void wsMakeCall({
    required CallKitParams params
  }) {
    final jsonParams = params.toJson();
    _wsChannel.sink.add(jsonEncode([
      "make_call", jsonParams
    ]));
  }
}

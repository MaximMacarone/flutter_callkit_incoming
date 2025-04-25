import 'package:flutter_callkit_incoming/entities/entities.dart';
import 'package:uuid/uuid.dart';

class Contact {
  final String id;
  final String name;
  final String phone;

  Contact({required this.id, required this.name, required this.phone});

  static final mockContacts =  [
    Contact(
      id: Uuid().v4().toString(),
      name: 'Алиса',
      phone: '+7 900 111-22-33'

      ),
    Contact(
      id: Uuid().v4().toString(),
      name: 'Боб',
      phone: '+7 900 222-33-44'
      ),
    Contact(
      id: Uuid().v4().toString(),
      name: 'Чарли',
      phone: '+7 900 333-44-55'),
    Contact(
      id: Uuid().v4().toString(),
      name: 'Дейв',
      phone: '+7 900 444-55-66'),
  ];
}

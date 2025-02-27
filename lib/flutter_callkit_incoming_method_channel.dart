import 'package:flutter/foundation.dart';
import 'package:flutter/services.dart';

import 'flutter_callkit_incoming_platform_interface.dart';

/// An implementation of [FlutterCallkitIncomingPlatform] that uses method channels.
class MethodChannelFlutterCallkitIncoming extends FlutterCallkitIncomingPlatform {
  /// The method channel used to interact with the native platform.
  @visibleForTesting
  final methodChannel = const MethodChannel('flutter_callkit_incoming');

  @override
  Future<String?> getPlatformVersion() async {
    final version = await methodChannel.invokeMethod<String>('getPlatformVersion');
    return version;
  }
}

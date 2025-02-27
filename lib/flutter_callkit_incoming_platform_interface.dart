import 'package:plugin_platform_interface/plugin_platform_interface.dart';

import 'flutter_callkit_incoming_method_channel.dart';

abstract class FlutterCallkitIncomingPlatform extends PlatformInterface {
  /// Constructs a FlutterCallkitIncomingPlatform.
  FlutterCallkitIncomingPlatform() : super(token: _token);

  static final Object _token = Object();

  static FlutterCallkitIncomingPlatform _instance = MethodChannelFlutterCallkitIncoming();

  /// The default instance of [FlutterCallkitIncomingPlatform] to use.
  ///
  /// Defaults to [MethodChannelFlutterCallkitIncoming].
  static FlutterCallkitIncomingPlatform get instance => _instance;

  /// Platform-specific implementations should set this with their own
  /// platform-specific class that extends [FlutterCallkitIncomingPlatform] when
  /// they register themselves.
  static set instance(FlutterCallkitIncomingPlatform instance) {
    PlatformInterface.verifyToken(instance, _token);
    _instance = instance;
  }

  Future<String?> getPlatformVersion() {
    throw UnimplementedError('platformVersion() has not been implemented.');
  }
}

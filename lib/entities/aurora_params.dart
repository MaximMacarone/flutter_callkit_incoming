import 'package:json_annotation/json_annotation.dart';

part 'aurora_params.g.dart';

enum CallStatus {
  unknown,
  disconnected,
  dialing,    // outcoming call
  ringing,    // incoming call
  rejecting,
  accepting,
  active,
  held,
}

@JsonSerializable(explicitToJson: true)
class AuroraParams {
  final bool incoming;
  final String localHandle;
  final String localName;
  final String remoteHandle;
  final String remoteName;
  final bool? holdable;
  final String? uri;
  CallStatus status;

  AuroraParams(
    this.incoming,
    this.localHandle,
    this.localName,
    this.remoteHandle,
    this.remoteName,
    this.holdable,
    this.uri,
    this.status);

      factory AuroraParams.fromJson(Map<String, dynamic> json) =>
      _$AuroraParamsFromJson(json);

  Map<String, dynamic> toJson() => _$AuroraParamsToJson(this);
}

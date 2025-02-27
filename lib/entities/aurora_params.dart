import 'package:json_annotation/json_annotation.dart';

@JsonSerializable(explicitToJson: true)
class AuroraParams {
const AuroraParams({
  this.holdable,
  this.incoming,
  this.localHandle,
  this.localName,
  this.remoteHandle,
  this.remoteName,
  this.status
});

  final bool? holdable;
  final bool? incoming;
  final String? localHandle;
  final String? localName;
  final String? remoteHandle;
  final String? remoteName;
  final int? status;
}

// GENERATED CODE - DO NOT MODIFY BY HAND

part of 'aurora_params.dart';

// **************************************************************************
// JsonSerializableGenerator
// **************************************************************************

AuroraParams _$AuroraParamsFromJson(Map<String, dynamic> json) => AuroraParams(
      json['incoming'] as bool,
      json['localHandle'] as String,
      json['localName'] as String,
      json['remoteHandle'] as String,
      json['remoteName'] as String,
      json['holdable'] as bool?,
      json['uri'] as String?,
      $enumDecode(_$CallStatusEnumMap, json['status']),
    );

Map<String, dynamic> _$AuroraParamsToJson(AuroraParams instance) =>
    <String, dynamic>{
      'incoming': instance.incoming,
      'localHandle': instance.localHandle,
      'localName': instance.localName,
      'remoteHandle': instance.remoteHandle,
      'remoteName': instance.remoteName,
      'holdable': instance.holdable,
      'uri': instance.uri,
      'status': _$CallStatusEnumMap[instance.status]!,
    };

const _$CallStatusEnumMap = {
  CallStatus.unknown: 'unknown',
  CallStatus.disconnected: 'disconnected',
  CallStatus.dialing: 'dialing',
  CallStatus.ringing: 'ringing',
  CallStatus.rejecting: 'rejecting',
  CallStatus.accepting: 'accepting',
  CallStatus.active: 'active',
  CallStatus.held: 'held',
};

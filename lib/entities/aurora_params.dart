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

class AuroraParams {
  final bool incoming;
  final String localHandle;
  final String localName;
  final String remoteHandle;
  final String remoteName;
  final bool? holdable;
  final String? uri;
  final CallStatus status;

  const AuroraParams({
    required this.incoming,
    required this.localHandle,
    required this.localName,
    required this.remoteHandle,
    required this.remoteName,
    required this.holdable,
    required this.uri,
    required this.status,
  });

  factory AuroraParams.fromJson(Map<String, dynamic> json) {
    return AuroraParams(
      incoming: json['incoming'] as bool,
      localHandle: json['localHandle'] as String,
      localName: json['localName'] as String,
      remoteHandle: json['remoteHandle'] as String,
      remoteName: json['remoteName'] as String,
      holdable: json['holdable'] as bool?,
      uri: json['uri'] as String?,
      status: CallStatus.values[int.parse(json['status'].toString())],
    );
  }

  Map<String, dynamic> toJson() {
    return {
      'incoming': incoming,
      'localHandle': localHandle,
      'localName': localName,
      'remoteHandle': remoteHandle,
      'remoteName': remoteName,
      'holdable': holdable,
      'uri': uri,
      'status': status.index, // 👈 теперь точно int
    };
  }
}

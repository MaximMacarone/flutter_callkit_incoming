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

  final String localHandle;
  final String localName;
  final bool holdable;
  final String? uri;

  const AuroraParams({
    required this.localHandle,
    required this.localName,
    required this.holdable,
    required this.uri,
  });

  factory AuroraParams.fromJson(Map<String, dynamic> json) {
    return AuroraParams(
      localHandle: json['localHandle'] as String,
      localName: json['localName'] as String,
      holdable: json['holdable'] as bool,
      uri: json['uri'] as String?,
    );
  }

  Map<String, dynamic> toJson() {
    return {
      'localHandle': localHandle,
      'localName': localName,
      'holdable': holdable,
      'uri': uri,
    };
  }
}

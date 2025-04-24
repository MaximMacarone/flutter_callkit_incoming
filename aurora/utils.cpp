#include "flutter_callkit_incoming/utils.h"

flutter::EncodableValue QVariantToEncodableValue(const QVariant& v) {
  using EV = flutter::EncodableValue;
  switch (v.type()) {
    case QVariant::Bool: return EV(v.toBool());
    case QVariant::Int:
    case QVariant::UInt:
    case QVariant::LongLong:
    case QVariant::ULongLong:
      return EV(static_cast<int>(v.toLongLong()));
    case QVariant::Double: return EV(v.toDouble());
    case QVariant::String: return EV(v.toString().toStdString());
    case QVariant::Map: {
      flutter::EncodableMap map;
      for (auto it = v.toMap().constBegin(); it != v.toMap().constEnd(); ++it) {
        map[EV(it.key().toStdString())] = QVariantToEncodableValue(it.value());
      }
      return EV(map);
    }
    case QVariant::List: {
      flutter::EncodableList list;
      for (const auto& item : v.toList()) {
        list.push_back(QVariantToEncodableValue(item));
      }
      return EV(list);
    }
    default:
      return EV(); // null
  }
}

flutter::EncodableMap QVariantMapToEncodableMap(const QVariantMap& in) {
  flutter::EncodableMap out;
  for (auto it = in.constBegin(); it != in.constEnd(); ++it) {
    out[flutter::EncodableValue(it.key().toStdString())] =
        QVariantToEncodableValue(it.value());
  }
  return out;
}

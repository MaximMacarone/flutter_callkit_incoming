 #include "include/flutter_callkit_incoming/aurora_params.h"
 #include <QDebug>

AuroraParams AuroraParams::fromEncodableMap(const flutter::EncodableMap& map) {
    AuroraParams params;
    auto getString = [&](const char* key) -> QString {
      auto it = map.find(flutter::EncodableValue(key));
      if (it != map.end() && std::holds_alternative<std::string>(it->second)) {
        return QString::fromStdString(std::get<std::string>(it->second));
      }
      return {};
    };

    auto getBool = [&](const char* key) -> bool {
      auto it = map.find(flutter::EncodableValue(key));
      if (it != map.end() && std::holds_alternative<bool>(it->second)) {
        return std::get<bool>(it->second);
      }
      return false;
    };

    auto getInt = [&](const std::string& key) -> std::optional<int> {
        auto it = map.find(flutter::EncodableValue(key));
        if (it != map.end() && std::holds_alternative<int>(it->second)) {
            return std::get<int>(it->second);
        }
        return std::nullopt;
    };

    params.localHandle = getString("localHandle");
    params.localName = getString("localName");
    params.holdable = getBool("holdable");
    
    auto uriValueIt = map.find(flutter::EncodableValue("uri"));
    if (uriValueIt != map.end() && std::holds_alternative<std::string>(uriValueIt->second)) {
        params.uri = QString::fromStdString(std::get<std::string>(uriValueIt->second));
    } else {
        params.uri = std::nullopt;
    }

    params.status = getInt("status").value_or(0);

    params.id = getString("id");
    params.nameCaller = getString("nameCaller");
    params.handle = getString("handle");
    params.appName = getString("appName");

    auto extraIt = map.find(flutter::EncodableValue("extra"));
    if (extraIt != map.end() && std::holds_alternative<flutter::EncodableMap>(extraIt->second)) {
      const auto& extraMap = std::get<flutter::EncodableMap>(extraIt->second);

      for (const auto& [key, value] : extraMap) {
        if (std::holds_alternative<std::string>(key)) {
          QString k = QString::fromStdString(std::get<std::string>(key));
          if (std::holds_alternative<std::string>(value)) {
            params.extra[k] = QString::fromStdString(std::get<std::string>(value));
          } else if (std::holds_alternative<int64_t>(value)) {
            params.extra[k] = static_cast<qint64>(std::get<int64_t>(value));
          } else if (std::holds_alternative<int>(value)) {
            params.extra[k] = static_cast<qint64>(std::get<int>(value));
          } else if (std::holds_alternative<bool>(value)) {
            params.extra[k] = std::get<bool>(value);
          }
        }
      }
    }

    return params;
}

AuroraParams AuroraParams::fromEncodableValue(const flutter::EncodableValue& value) {
    AuroraParams params;

    if (!std::holds_alternative<flutter::EncodableMap>(value)) {
        return params;
    }

    const auto& fullMap = std::get<flutter::EncodableMap>(value);

    flutter::EncodableMap auroraMap;

    auto it = fullMap.find(flutter::EncodableValue("aurora"));
    if (it != fullMap.end() && std::holds_alternative<flutter::EncodableMap>(it->second)) {
        auroraMap = std::get<flutter::EncodableMap>(it->second);

        for (const auto& key : {"id", "nameCaller", "handle", "extra"}) {
            auto outerIt = fullMap.find(flutter::EncodableValue(key));
            if (outerIt != fullMap.end()) {
                auroraMap[flutter::EncodableValue(key)] = outerIt->second;
            }
        }
    } else {
        auroraMap = fullMap;
    }

    return AuroraParams::fromEncodableMap(auroraMap);
}


QVariantMap AuroraParams::toQVariantMap() const {
    QVariantMap map;
    map["LocalHandle"] = localHandle;
    map["LocalName"] = localName;
    map["Holdable"] = holdable;
    if (uri.has_value()) {
        map["uri"] = uri.value();
    }
    map["Status"] = status;

    map["Id"] = id;
    map["RemoteName"] = nameCaller;
    map["RemoteHandle"] = handle;
    map["Extra"] = extra;
    map["AppName"] = appName;

    return map;
}

flutter::EncodableList AuroraParams::toCallkitParams(DBusManagerStruct managedObjects) {

  flutter::EncodableList out;
  for (auto it = managedObjects.constBegin(); it != managedObjects.constEnd(); ++it) {
    const QDBusObjectPath& path        = it.key();
    const VariantMapMap&   ifaces      = it.value();

    auto itIface = ifaces.find(QStringLiteral("ru.auroraos.Call.Call1"));
    if (itIface == ifaces.end()) continue;

    QVariantMap props = itIface.value();
    flutter::EncodableMap json;

    json[flutter::EncodableValue("id")] = flutter::EncodableValue(props.value("Id").toString().toStdString());
    json[flutter::EncodableValue("nameCaller")] = flutter::EncodableValue(props.value("RemoteName").toString().toStdString());
    json[flutter::EncodableValue("handle")] = flutter::EncodableValue(props.value("RemoteHandle").toString().toStdString());

    // extra
    flutter::EncodableMap extra;
    for (auto const& k : props.value("Extra").toMap().keys()) {
      QVariant v = props.value("Extra").toMap().value(k);
      if (v.type() == QVariant::String) {
        extra[flutter::EncodableValue(k.toStdString())] = flutter::EncodableValue(v.toString().toStdString());
      } else if (v.type() == QVariant::Bool) {
        extra[flutter::EncodableValue(k.toStdString())] = flutter::EncodableValue(v.toBool());
      } else if (v.canConvert<int>()) {
        extra[flutter::EncodableValue(k.toStdString())] = flutter::EncodableValue(v.toInt());
      }
    }
    json[flutter::EncodableValue("extra")] = flutter::EncodableValue(extra);

    flutter::EncodableMap aurora;
    aurora[flutter::EncodableValue("localName")] = flutter::EncodableValue(props.value("LocalName").toString().toStdString());
    aurora[flutter::EncodableValue("localHandle")] = flutter::EncodableValue(props.value("LocalHandle").toString().toStdString());
    aurora[flutter::EncodableValue("holdable")] = flutter::EncodableValue(props.value("Holdable").toBool());

    aurora[flutter::EncodableValue("uri")] = flutter::EncodableValue(); // null
    json[flutter::EncodableValue("aurora")] = flutter::EncodableValue(aurora);

    json[flutter::EncodableValue("appName")] = flutter::EncodableValue();
    json[flutter::EncodableValue("avatar")] = flutter::EncodableValue();
    json[flutter::EncodableValue("type")] = flutter::EncodableValue();
    json[flutter::EncodableValue("normalHandle")] = flutter::EncodableValue();
    json[flutter::EncodableValue("duration")] = flutter::EncodableValue();
    json[flutter::EncodableValue("textAccept")] = flutter::EncodableValue();
    json[flutter::EncodableValue("textDecline")] = flutter::EncodableValue();
    json[flutter::EncodableValue("missedCallNotification")] = flutter::EncodableValue();
    json[flutter::EncodableValue("headers")] = flutter::EncodableValue();
    json[flutter::EncodableValue("android")] = flutter::EncodableValue();
    json[flutter::EncodableValue("ios")] = flutter::EncodableValue();

    out.push_back(flutter::EncodableValue(json));
  }
  return out;
}

flutter::EncodableMap AuroraParams::toEncodableMap() {
  using flutter::EncodableMap;
  using flutter::EncodableValue;

  // // вспомогательная функция из предыдущего примера
  // auto variantToEncodable = [&](const QVariant& v) {
  //   // реализация как раньше: поддержка QString, bool, int, double, списков, карт
  //   // …
  // };

  EncodableMap result;

  // 1) Первые пять полей — на самый верх
  result[EncodableValue("id")]         = EncodableValue(id.toStdString());
  result[EncodableValue("appName")]    = EncodableValue(appName.toStdString());
  result[EncodableValue("nameCaller")] = EncodableValue(nameCaller.toStdString());
  result[EncodableValue("handle")]     = EncodableValue(handle.toStdString());

  // extra → Map<String, dynamic>
  EncodableMap extraMap;
  for (auto it = extra.cbegin(); it != extra.cend(); ++it) {
    extraMap[EncodableValue(it.key().toStdString())] = AuroraParams::variantToEncodable(it.value());
  }
  result[EncodableValue("extra")] = EncodableValue(extraMap);

  // 2) Собираем вложенный объект "aurora"
  EncodableMap auroraMap;
  auroraMap[EncodableValue("localHandle")] = EncodableValue(localHandle.toStdString());
  auroraMap[EncodableValue("localName")]   = EncodableValue(localName.toStdString());
  auroraMap[EncodableValue("holdable")]    = EncodableValue(holdable);
  auroraMap[EncodableValue("status")]      = EncodableValue(status);

  // uri — опционально, null если нет
  if (uri.has_value() && !uri->isNull()) {
    auroraMap[EncodableValue("uri")] = EncodableValue(uri->toStdString());
  } else {
    auroraMap[EncodableValue("uri")] = EncodableValue();
  }

  // Кладём готовый auroraMap в корень под ключом "aurora"
  result[EncodableValue("aurora")] = EncodableValue(auroraMap);

  return result;
}

flutter::EncodableValue AuroraParams::variantToEncodable(const QVariant& v) {
  using flutter::EncodableMap;
  using flutter::EncodableList;
  using flutter::EncodableValue;

  if (v.isNull()) {
    return EncodableValue();
  }
  switch (v.type()) {
    case QMetaType::QString:
      return EncodableValue(v.toString().toStdString());
    case QMetaType::Bool:
      return EncodableValue(v.toBool());
    case QMetaType::Int:
    case QMetaType::LongLong:
    case QMetaType::UInt:
    case QMetaType::ULongLong:
      return EncodableValue(static_cast<int>(v.toLongLong()));
    case QMetaType::Double:
      return EncodableValue(v.toDouble());
    case QMetaType::QVariantList: {
      EncodableList list;
      for (auto &elem : v.toList()) {
        list.push_back(variantToEncodable(elem));
      }
      return EncodableValue(list);
    }
    case QMetaType::QVariantMap: {
      EncodableMap m;
      for (auto it = v.toMap().cbegin(); it != v.toMap().cend(); ++it) {
        m[EncodableValue(it.key().toStdString())] = variantToEncodable(it.value());
      }
      return EncodableValue(m);
    }
    default:
      // fallback: строковое представление
      return EncodableValue(v.toString().toStdString());
  }
}

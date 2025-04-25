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

        // Добавляем общие поля из верхнего уровня
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

    return map;
}

// #include <string>
// #include <iostream>
// #include <optional>
// #include <flutter/encodable_value.h>
// #include <flutter/standard_method_codec.h>

// #include <QVariantMap>

// struct AuroraParams {
//     bool incoming;
//     std::string localHandle;
//     std::string localName;
//     std::string remoteHandle;
//     std::string remoteName;
//     bool holdable;
//     std::optional<std::string> uri;
//     int status;

//     QVariantMap toVariantMap() const {
//         QVariantMap map;
//         map["Incoming"] = incoming;
//         map["LocalName"] = QString::fromStdString(localName);
//         map["LocalHandle"] = QString::fromStdString(localHandle);
//         map["RemoteHandle"] = QString::fromStdString(remoteHandle);
//         map["RemoteName"] = QString::fromStdString(remoteName);
//         map["Holdable"] = holdable;
//         map["Status"] = status;

//         if (uri.has_value()) {
//             map["Uri"] = QString::fromStdString(uri.value());
//         }

//         return map;
//     }
// };

// std::optional<AuroraParams> ParseAuroraParams(const flutter::EncodableMap& params);

#pragma once

#include <QString>
#include <QVariantMap>
#include <flutter/encodable_value.h>
#include <flutter/standard_method_codec.h>

struct AuroraParams {
    QString localHandle;
    QString localName;
    bool holdable;
    QString uri;
    QString status;
  
    // Из общих полей CallKitParams, помеченных как //
    QString id;
    QString nameCaller;
    QString handle;
    QMap<QString, QVariant> extra;

    AuroraParams fromEncodableMap(const flutter::EncodableMap& map);
    QVariantMap toQVariantMap() const;
};

#include <string>
#include <iostream>
#include <optional>
#include <flutter/encodable_value.h>
#include <flutter/standard_method_codec.h>

#include <QVariantMap>

struct AuroraParams {
    bool incoming;
    std::string localHandle;
    std::string localName;
    std::string remoteHandle;
    std::string remoteName;
    bool holdable;
    std::optional<std::string> uri;
    int status;

    QVariantMap toVariantMap() const {
        QVariantMap map;
        map["incoming"] = incoming;
        map["localHandle"] = QString::fromStdString(localHandle);
        map["localName"] = QString::fromStdString(localName);
        map["remoteHandle"] = QString::fromStdString(remoteHandle);
        map["remoteName"] = QString::fromStdString(remoteName);
        map["holdable"] = holdable;
        map["status"] = status;

        if (uri.has_value()) {
            map["uri"] = QString::fromStdString(uri.value());
        }

        return map;
    }
};

std::optional<AuroraParams> ParseAuroraParams(const flutter::EncodableMap& params);

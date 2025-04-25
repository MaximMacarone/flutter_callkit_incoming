// #include <string>
// #include <iostream>
// #include <optional>
// #include <flutter/encodable_value.h>
// #include <flutter/standard_method_codec.h>

#pragma once

#include <QString>
#include <QVariantMap>
#include <flutter/encodable_value.h>
#include <flutter/standard_method_codec.h>
#include "dbustypes.h"

struct AuroraParams {
    QString localHandle;
    QString localName;
    bool holdable;
    std::optional<QString> uri;
    int status = 0;
  
    QString id;
    QString appName;
    QString nameCaller;
    QString handle;
    QMap<QString, QVariant> extra;

    static AuroraParams fromEncodableMap(const flutter::EncodableMap& map);
    static AuroraParams fromEncodableValue(const flutter::EncodableValue& value);
    QVariantMap toQVariantMap() const;
    static flutter::EncodableList toCallkitParams(DBusManagerStruct managedObjects);
};

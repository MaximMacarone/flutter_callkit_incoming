#pragma once
#include <QVariant>
#include <flutter/encodable_value.h>

flutter::EncodableValue QVariantToEncodableValue(const QVariant& v);
flutter::EncodableMap QVariantMapToEncodableMap(const QVariantMap& in);

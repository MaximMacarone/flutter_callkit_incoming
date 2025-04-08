// SPDX-FileCopyrightText: 2024 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

#ifndef DBUSTYPES_H
#define DBUSTYPES_H

#include <QDBusObjectPath>
#include <QVariantMap>

typedef QMap<QString, QVariantMap> VariantMapMap;
Q_DECLARE_METATYPE(VariantMapMap)

typedef QMap<QDBusObjectPath, VariantMapMap> DBusManagerStruct;
Q_DECLARE_METATYPE(DBusManagerStruct)

#endif // DBUSTYPES_H

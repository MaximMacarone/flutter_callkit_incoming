#include "include/flutter_callkit_incoming/aurora_params.h"

std::optional<AuroraParams> ParseAuroraParams(const flutter::EncodableMap& auroraMap) {
    AuroraParams params;

    auto getStr = [&](const std::string& key) -> std::optional<std::string> {
        auto it = auroraMap.find(flutter::EncodableValue(key));
        if (it != auroraMap.end() && std::holds_alternative<std::string>(it->second)) {
            return std::get<std::string>(it->second);
        }
        return std::nullopt;
    };

    auto getBool = [&](const std::string& key) -> std::optional<bool> {
        auto it = auroraMap.find(flutter::EncodableValue(key));
        if (it != auroraMap.end() && std::holds_alternative<bool>(it->second)) {
            return std::get<bool>(it->second);
        }
        return std::nullopt;
    };

    auto getInt = [&](const std::string& key) -> std::optional<int> {
        auto it = auroraMap.find(flutter::EncodableValue(key));
        if (it != auroraMap.end() && std::holds_alternative<int>(it->second)) {
            return std::get<int>(it->second);
        }
        return std::nullopt;
    };

    params.incoming     = getBool("incoming").value_or(false);
    params.localHandle  = getStr("localHandle").value_or("");
    params.localName    = getStr("localName").value_or("");
    params.remoteHandle = getStr("remoteHandle").value_or("");
    params.remoteName   = getStr("remoteName").value_or("");
    params.holdable     = getBool("holdable").value_or(false);
    params.uri          = getStr("uri");
    params.status       = getInt("status").value_or(0);

    return params;
}

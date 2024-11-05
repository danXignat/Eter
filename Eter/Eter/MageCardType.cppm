export module MageCardType;

import <unordered_map>;
import <string>;
import <cstdint>;

namespace base {
    export enum class MageCardType : uint8_t {
        Fire,
        Earth,
        Air,
        Water
    };

    const std::unordered_map<MageCardType, std::string_view> mapMageTypes = {
        {MageCardType::Fire, "Fire"},
        {MageCardType::Earth, "Earth"},
        {MageCardType::Air, "Air"},
        {MageCardType::Water, "Water"}
    };

    export std::string_view typesToStrings(MageCardType type) {
        return mapMageTypes.at(type);
    }
}
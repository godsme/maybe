//
// Created by Darwin Yuan on 2021/8/2.
//

#ifndef MAYBE_AFDF9E261600492B8999CFDBD25E7A24
#define MAYBE_AFDF9E261600492B8999CFDBD25E7A24

#include <type_traits>
template<typename ENUM_TYPE, ENUM_TYPE UPPER_BOUNDARY_>
struct Enum {
    static_assert(std::is_enum_v<ENUM_TYPE>);
    using EnumType = ENUM_TYPE;
    using IntType = std::underlying_type_t<ENUM_TYPE>;
    constexpr static bool IS_SIGNED = false;
    constexpr static IntType LOWER_BOUNDARY = 0;
    constexpr static IntType UPPER_BOUNDARY = UPPER_BOUNDARY_;

    constexpr Enum(EnumType value) : value{value} {}

    constexpr operator EnumType() const {
        return value;
    }

    constexpr auto IsValid() const -> bool {
        return value <= (EnumType)UPPER_BOUNDARY;
    }

protected:
    EnumType value;
};

#endif //MAYBE_AFDF9E261600492B8999CFDBD25E7A24

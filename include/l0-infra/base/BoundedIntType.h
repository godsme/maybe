//
// Created by Darwin Yuan on 2021/8/2.
//

#ifndef MAYBE_FDB1B24ACF36488AA36D5A50D736C5EE
#define MAYBE_FDB1B24ACF36488AA36D5A50D736C5EE

#include <limits>
#include <type_traits>

template<typename INT_TYPE, INT_TYPE UPPER_BOUNDARY_ = std::numeric_limits<INT_TYPE>::max(), INT_TYPE LOWER_BOUNDARY_ = std::numeric_limits<INT_TYPE>::min()>
struct BoundedIntType {
    static_assert(std::is_integral_v<INT_TYPE>);
    using IntType = INT_TYPE;
    constexpr static bool IS_SIGNED = std::is_signed_v<INT_TYPE>;
    constexpr static IntType LOWER_BOUNDARY = LOWER_BOUNDARY_;
    constexpr static IntType UPPER_BOUNDARY = UPPER_BOUNDARY_;

    constexpr BoundedIntType(INT_TYPE value) : value{value} {}

    constexpr operator IntType() const {
        return value;
    }

    constexpr auto IsValid() const -> bool {
        return value >= LOWER_BOUNDARY && value <= UPPER_BOUNDARY;
    }

protected:
    INT_TYPE value;
};

#endif //MAYBE_FDB1B24ACF36488AA36D5A50D736C5EE

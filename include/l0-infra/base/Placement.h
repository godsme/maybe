//
// Created by Darwin Yuan on 2021/8/1.
//

#ifndef MAYBE_D967B5CC539C44BE9272834BA1740EEA
#define MAYBE_D967B5CC539C44BE9272834BA1740EEA

#include <l0-infra/base/detail/Ref_Placement.h>
#include <l0-infra/base/detail/Value_Placement.h>
#include <type_traits>

template<typename T>
using Placement = std::conditional_t<std::is_reference_v<T>,
        Ref_Placement<std::remove_reference_t<T>>,
        Value_Placement<T>>;

#endif //MAYBE_D967B5CC539C44BE9272834BA1740EEA

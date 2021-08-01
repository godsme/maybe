//
// Created by Darwin Yuan on 2021/8/1.
//

#ifndef PLACEMENT_D967B5CC539C44BE9272834BA1740EEA
#define PLACEMENT_D967B5CC539C44BE9272834BA1740EEA

#include <l0-infra/base/detail/Ref_Placement.h>
#include <l0-infra/base/detail/Val_Placement.h>
#include <type_traits>

template<typename T>
using Placement = std::conditional_t<std::is_reference_v<T>,
        Ref_Placement<std::remove_reference_t<T>>,
        Val_Placement<T>>;

#endif //PLACEMENT_D967B5CC539C44BE9272834BA1740EEA

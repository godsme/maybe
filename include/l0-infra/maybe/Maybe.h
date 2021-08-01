//
// Created by Darwin Yuan on 2021/8/1.
//

#ifndef MAYBE_5B22D47F4DD54C17BAE28C385C86B3FC
#define MAYBE_5B22D47F4DD54C17BAE28C385C86B3FC

struct Nothing {};

constexpr Nothing nothing{};

template<typename T>
struct Maybe {
private:

    bool present;
};

#endif //MAYBE_5B22D47F4DD54C17BAE28C385C86B3FC

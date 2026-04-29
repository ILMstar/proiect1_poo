#pragma once

#include <ctime>
#include <random>

inline std::mt19937& global_rng() {
    static std::mt19937 gen(static_cast<unsigned>(std::time(nullptr)));
    return gen;
}

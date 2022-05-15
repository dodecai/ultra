#pragma once

#include <random>

class Random {
public:
    static void Init() {
        sRandomEngine.seed(std::random_device()());
    }

    static float Float() {
        return (float)sRandomsDistribution(sRandomEngine) / (float)std::numeric_limits<uint32_t>::max();
    }

private:
    static inline std::mt19937 sRandomEngine;
    static inline std::uniform_int_distribution<std::mt19937::result_type> sRandomsDistribution;
};

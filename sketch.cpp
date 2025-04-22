#include "sketch.h"
#include <random>
#include <chrono>

Sketch::Sketch(int r_) : r(r_), A(r_, 0) {
    std::mt19937_64 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<uint64_t> dist(0, P - 1);
    a = dist(rng);
    b = dist(rng);
    c = dist(rng);
    d = dist(rng);
}

uint64_t Sketch::random_odd_64() {
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<uint64_t> dist;
    return dist(gen) | 1ULL;
}

uint64_t Sketch::k(uint32_t x) const {
    uint64_t k = x;
    k = (a * k + b) % P;
    k = (k * x + c) % P;
    k = (k * x + d) % P;
    return k;
}

int Sketch::g(uint32_t x) const {
    uint64_t val = k(x);
    return (val & 1) ? 1 : -1;
}

int Sketch::h(uint32_t x) const {
    uint64_t val = k(x);
    return (val >> 1) & (r - 1);
}

void Sketch::Update(uint32_t i, int64_t delta) {
    int index = h(i);
    int sign = g(i);
    A[index] += sign * delta;
}

uint64_t Sketch::Query() const {
    uint64_t result = 0;
    for (int64_t val : A) {
        result += static_cast<uint64_t>(val * val);
    }
    return result;
}

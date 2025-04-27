#include "sketch.h"
#include <random>
#include <chrono>
#include <iostream>

Sketch::Sketch(int r_) : r(r_), A(r_, 0)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint64_t> dist(0, P - 1);
    a = dist(gen);
    b = dist(gen);
    c = dist(gen);
    d = dist(gen);
}

uint64_t Sketch::k(uint32_t x) const
{
    uint64_t k = x;
    k = (a * k + b) % P;
    k = (k * x + c) % P;
    k = (k * x + d) % P;
    return k;
}

std::pair<uint64_t, uint64_t> Sketch::hg(uint32_t x) const
{
    uint64_t val = k(x);
    return {(val >> 1) & (r - 1), 2 * (val & 1) - 1};
}

int Sketch::bad_k(uint32_t x) const
{
    return (a * x + b) >> 33;
}

std::pair<uint64_t, uint64_t> Sketch::bad_hg(uint32_t x) const
{
    uint64_t val = bad_k(x);
    return {(val >> 1) & (r - 1), 2 * (val & 1) - 1};
}

void Sketch::Update(uint32_t i, int64_t delta)
{
    auto hg_pair = hg(i);
    int index = hg_pair.first;
    int sign = hg_pair.second;
    A[index] += sign * delta;
}

void Sketch::Bad_Update(uint32_t i, int64_t delta)
{
    auto hg_pair = bad_hg(i);
    int index = hg_pair.first;
    int sign = hg_pair.second;
    A[index] += sign * delta;
}

uint64_t Sketch::Query() const
{
    uint64_t result = 0;
    for (int64_t val : A)
    {
        result += static_cast<uint64_t>(val * val);
    }
    return result;
}

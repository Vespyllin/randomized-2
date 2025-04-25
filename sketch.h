#ifndef SKETCH_H
#define SKETCH_H

#include <vector>
#include <cstdint>

class Sketch
{
public:
    explicit Sketch(int r_);
    void Update(uint32_t i, int64_t delta);
    void Bad_Update(uint32_t i, int64_t delta);
    uint64_t Query() const;
    int h(uint32_t x) const;
    int g(uint32_t x) const;

    int bad_k(uint32_t x) const;
    int bad_h(uint32_t x) const;
    int bad_g(uint32_t x) const;

private:
    int r;
    std::vector<int64_t> A;
    uint64_t a, b, c, d;
    static const uint64_t P = (1ULL << 31) - 1;

    uint64_t k(uint32_t x) const;
};

#endif

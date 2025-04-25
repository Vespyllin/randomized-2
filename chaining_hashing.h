#ifndef CHAINING_HASHING_H
#define CHAINING_HASHING_H

#include <vector>
#include <list>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <random>

class ChainingHashTable
{
private:
    // Table stores key-value pairs where key is uint32_t and value is int64_t
    std::vector<std::list<std::pair<uint32_t, int64_t>>> table;
    size_t table_size;
    size_t largest_list_size;

    // Mersenne prime (2^31 - 1)
    static const uint32_t P = 2147483647;

    // Coefficients for the polynomial hash function
    uint32_t a, b, c, d;

    uint32_t multiply_shift_uint32(uint32_t x, uint32_t l, uint32_t a) const;

public:
    // Constructor accepts size n for the hash table and initializes random coefficients
    ChainingHashTable(size_t size);

    // Insert method updates or adds (key, delta) to the table
    void update(uint32_t key, int64_t delta);

    // Search for a key in the table
    bool search(uint32_t key) const;

    uint64_t query();
};

#endif

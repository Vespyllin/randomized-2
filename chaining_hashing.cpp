#include "chaining_hashing.h"
#include <iostream>
#include <list>
#include <vector>
#include <random>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <algorithm> // For std::shuffle

// Initialize the ChainingHashTable with random coefficients for the multiply-shift hash
ChainingHashTable::ChainingHashTable(size_t size)
    : table_size(size), table(size), largest_list_size(0)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint32_t> dis(1, P - 1);

    a = dis(gen);
    b = dis(gen);
    c = dis(gen);
    d = dis(gen);
}

uint32_t ChainingHashTable::multiply_shift_uint32(uint32_t x, uint32_t l, uint32_t a) const
{
    return (a * x) >> (32 - l);
}

void ChainingHashTable::update(uint32_t key, int64_t delta)
{
    size_t index = multiply_shift_uint32(key, 32, a | 1) % table_size;
    // size_t index = multiply_shift_uint32(key, 22, a | 1);

    // Search if key already exists in the table
    for (auto &item : table[index])
    {
        if (item.first == key)
        {
            item.second += delta;
            return;
        }
    }

    table[index].emplace_back(key, delta);
    largest_list_size = std::max(largest_list_size, table[index].size());
}

bool ChainingHashTable::search(uint32_t key) const
{
    size_t index = multiply_shift_uint32(key, 32, a | 1) % table_size;

    const std::list<std::pair<uint32_t, int64_t>> &chain = table[index];

    for (const auto &item : chain)
    {
        if (item.first == key)
        {
            return true;
        }
    }

    return false;
}

uint64_t ChainingHashTable::query()
{
    uint64_t norm = 0;
    for (const auto &chain : table)
    {
        for (const auto &item : chain)
        {
            norm += item.second * item.second;
        }
    }
    return norm;
}
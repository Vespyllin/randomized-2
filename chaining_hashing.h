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

    // Hash function k(x) = (a * x) % p
    uint32_t k(uint32_t x) const;

public:
    // Constructor accepts size n for the hash table and initializes random coefficients
    ChainingHashTable(size_t size);

    // Insert method updates or adds (key, delta) to the table
    void insert(uint32_t key, int64_t delta);

    // Search for a key in the table
    bool search(uint32_t key) const;

    // Get the largest list size
    size_t getLargestListSize() const;

    // Print all the key-value pairs in the table
    void printTable() const;

    // Record the largest list size after each insertion into a CSV file
    void recordLargestListSizeData(const std::vector<uint32_t>& keys, const std::vector<int64_t>& deltas, const std::string& filename);

    // Compute h(i) and g(i)
    int32_t h(uint32_t i, size_t R) const;
    int32_t g(uint32_t i) const;
};

#endif

#include "chaining_hashing.h"
#include <iostream>
#include <list>
#include <vector>
#include <random>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <algorithm>  // For std::shuffle

// Initialize the ChainingHashTable with random coefficients for the multiply-shift hash
ChainingHashTable::ChainingHashTable(size_t size)
    : table_size(size), table(size), largest_list_size(0)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint32_t> dis(1, P - 1);  // Random odd a

    // Randomly choose coefficient a as an odd number in the range [1, P-1]
    a = dis(gen);
    // You can choose b, c, d as you see fit, but for simplicity, let's use random values
    b = dis(gen);
    c = dis(gen);
    d = dis(gen);
}

uint32_t ChainingHashTable::k(uint32_t x) const
{
    // Efficient Multiply-Shift hash function: k(x) = (a * x) % p
    uint64_t result = a * uint64_t(x) % P;
    return result % P;
}

void ChainingHashTable::insert(uint32_t key, int64_t delta)
{
    size_t index = k(key) % table_size;

    // Search if key already exists in the table
    for (auto& item : table[index]) {
        if (item.first == key) {
            item.second += delta;
            return;
        }
    }

    table[index].emplace_back(key, delta);
    largest_list_size = std::max(largest_list_size, table[index].size());
}

bool ChainingHashTable::search(uint32_t key) const
{
    size_t index = k(key) % table_size;
    const std::list<std::pair<uint32_t, int64_t>>& chain = table[index];

    for (const auto& item : chain)
    {
        if (item.first == key)
        {
            return true;
        }
    }

    return false;
}

size_t ChainingHashTable::getLargestListSize() const
{
    return largest_list_size;
}

void ChainingHashTable::printTable() const
{
    for (size_t i = 0; i < table_size; ++i)
    {
        std::cout << "Index " << i << ": ";
        for (const auto& item : table[i])
        {
            std::cout << "(" << item.first << ", " << item.second << ") -> ";
        }
        std::cout << "nullptr" << std::endl;
    }
}

void ChainingHashTable::recordLargestListSizeData(const std::vector<uint32_t>& keys, const std::vector<int64_t>& deltas, const std::string& filename)
{
    std::ofstream output_file(filename);

    if (!output_file.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
        return;
    }

    output_file << "n,LargestListSize\n";

    for (size_t i = 0; i < keys.size(); ++i) {
        insert(keys[i], deltas[i]);
        output_file << (i + 1) << "," << getLargestListSize() << "\n";
    }

    output_file.close();
}

int32_t ChainingHashTable::h(uint32_t i, size_t R) const
{
    uint32_t k_i = k(i);

    // Use the next R bits for h(i)
    return (k_i >> 1) & ((1 << R) - 1);  // Extract R bits for h(i)
}

int32_t ChainingHashTable::g(uint32_t i) const
{
    uint32_t k_i = k(i);

    // Use the least significant bit for g(i)
    return 2 * (k_i & 1) - 1;  // g(i) = -1 or +1 based on LSB of k(i)
}

int main()
{
    size_t table_size = 100;
    ChainingHashTable table(table_size);

    std::vector<uint32_t> keys;
    for (size_t i = 0; i < table_size; ++i) {
        keys.push_back((i * i) % table_size);
    }

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(keys.begin(), keys.end(), g);

    std::vector<int64_t> deltas(keys.size(), 1);

    // Record the largest list size into a CSV file
    table.recordLargestListSizeData(keys, deltas, "largest_linked_list_sizes.csv");

    std::cout << "Data recorded to largest_linked_list_sizes.csv" << std::endl;

    // Compute h(i) and g(i) for some keys
    for (size_t i = 0; i < 10; ++i) {
        std::cout << "For key " << keys[i] << ": h(i) = " << table.h(keys[i], 3) << ", g(i) = " << table.g(keys[i]) << std::endl;
    }

    return 0;
}

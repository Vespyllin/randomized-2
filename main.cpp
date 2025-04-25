#include <iostream>
#include <chrono>
#include "sketch.h"
#include "chaining_hashing.h"

uint32_t multiply_shift_uint32(uint32_t x, uint32_t l, uint32_t a)
{
    return (a * x) >> (32 - l);
}

void hash_benchmarks()
{
    const int iter = 1'000'000;
    Sketch sketch(1024); // r = 2^10
    uint32_t random_a = 10413;

    // Test h(i)
    auto start = std::chrono::high_resolution_clock::now();
    for (uint32_t i = 0; i < iter; ++i)
    {
        volatile int result = sketch.h(i);
        (void)result;
    }
    auto end = std::chrono::high_resolution_clock::now();
    double h_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    // Test g(i)
    start = std::chrono::high_resolution_clock::now();
    for (uint32_t i = 0; i < iter; ++i)
    {
        volatile int result = sketch.g(i);
        (void)result;
    }
    end = std::chrono::high_resolution_clock::now();
    double g_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    // Test m(i)
    start = std::chrono::high_resolution_clock::now();
    for (uint32_t i = 0; i < iter; ++i)
    {
        volatile uint32_t result = multiply_shift_uint32(i, 32, random_a);
        (void)result;
    }
    end = std::chrono::high_resolution_clock::now();
    double m_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    // Print results
    std::cout << "Total time:\n";
    std::cout << "h(i): " << h_time << "ms\n";
    std::cout << "g(i): " << g_time << "ms\n";
    std::cout << "m(i): " << m_time << "ms\n";
}

void norm_benchmarks()
{
    // uint64_t iter = 1'000'000'000; // 10^9 updates
    uint64_t iter = 1'000'000;

    std::cout << "Testing Chain Runtime" << std::endl;

    for (size_t N = 6; N <= 28; N++)
    {
        auto r = 1 << 5;
        ChainingHashTable chain(r);
        double chain_time = 0.0;

        uint64_t n = 1 << N;

        auto start = std::chrono::high_resolution_clock::now();
        for (int64_t i = 0; i < iter; ++i)
        {
            uint32_t key = i % n;
            chain.update(key, 1);
        }
        auto end = std::chrono::high_resolution_clock::now();
        chain_time += std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
        std::cout << "Chain time for N = " << N << " : " << chain_time << "s\n";
    }

    std::vector<int> sketch_sizes = {7, 10, 20};
    for (auto &sketch_size : sketch_sizes)
    {
        uint64_t r = 1 << sketch_size;
        Sketch sketch(r);

        std::cout << "Testing Sketch Runtime" << std::endl;
        for (size_t N = 6; N <= 28; N++)
        {
            double sketch_time = 0.0;
            uint64_t n = 1 << N;

            auto start = std::chrono::high_resolution_clock::now();
            for (int64_t i = 0; i < iter; ++i)
            {
                uint32_t key = i % n;
                sketch.Update(key, 1);
            }
            auto end = std::chrono::high_resolution_clock::now();
            sketch_time += std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
            std::cout << "Sketch time for N = " << N << " R = " << sketch_size << " : " << sketch_time << "s\n";
        }
    }
}

void error_benchmarks(bool good)
{
    uint64_t updates = 1000;
    uint64_t max_iter = 100;

    std::vector<int> sketch_sizes = {3, 4, 5, 6, 7, 8, 9, 10};
    std::cout << "Testing Avg. Sketch Error" << std::endl;

    for (auto &sketch_size : sketch_sizes)
    {
        double error = 0.0;
        uint64_t r = 1 << sketch_size;

        for (size_t iter = 0; iter < max_iter; iter++)
        {
            Sketch sketch(r);
            ChainingHashTable chain(r);

            for (uint32_t i = 1; i <= updates; ++i)
            {
                chain.update(i, i * i);
                if (good)
                    sketch.Update(i, i * i);
                else
                    sketch.Bad_Update(i, i * i);
            }

            auto f = chain.query();
            auto sf = sketch.Query();

            error += double(f > sf ? (f - sf) : (sf - f)) / double(f);
        }
        std::cout << "avg error @R = " << sketch_size << ": " << error / double(max_iter) << std::endl;
    }

    std::cout << "\nTesting Max Sketch Error" << std::endl;

    updates = 1000;
    max_iter = 10000;
    for (auto &sketch_size : sketch_sizes)
    {
        double max_error = 0.0;
        uint64_t r = 1 << sketch_size;

        for (size_t iter = 0; iter < max_iter; iter++)
        {
            Sketch sketch(r);
            ChainingHashTable chain(r);

            for (uint32_t i = 1; i <= updates; ++i)
            {
                chain.update(i, i * i);
                if (good)
                    sketch.Update(i, i * i);
                else
                    sketch.Bad_Update(i, i * i);
            }

            auto f = chain.query();
            auto sf = sketch.Query();

            double error = double(f > sf ? (f - sf) : (sf - f)) / double(f);
            max_error = error > max_error ? error : max_error;
        }
        std::cout << "max error @R = " << sketch_size << ": " << max_error << std::endl;
        break;
    }
}

int main()
{
    hash_benchmarks();
    std::cout << std::endl;
    norm_benchmarks();
    std::cout << std::endl;
    error_benchmarks(true);
    std::cout << std::endl;
    error_benchmarks(false);
    return 0;
}

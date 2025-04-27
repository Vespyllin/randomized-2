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
        volatile auto result = sketch.hg(i);
        (void)result;
    }
    auto end = std::chrono::high_resolution_clock::now();
    double hg_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

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
    std::cout << "h(i) + g(i): " << hg_time << "ms\n";
    std::cout << "m(i): " << m_time << "ms\n";
}

void norm_benchmarks()
{
    uint64_t iter = 1'000'000'000; // 10^9 updates

    // auto R = 22;
    // std::cout << "Testing Chain Runtime R=" << R << std::endl;
    // std::cout << "N, time_s\n";
    // for (size_t N = 6; N <= 28; N++)
    // {
    //     ChainingHashTable chain(1 << R);
    //     uint64_t n = 1 << N;

    //     auto start = std::chrono::high_resolution_clock::now();
    //     for (int64_t i = 0; i < iter; ++i)
    //     {
    //         chain.update(i % n, 1);
    //     }
    //     auto end = std::chrono::high_resolution_clock::now();
    //     double chain_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    //     std::cout << N << ", " << round(chain_time / 10) / 100 << "\n";
    // }

    std::vector<int> sketch_sizes = {7, 10, 20};
    for (auto &sketch_size : sketch_sizes)
    {
        uint64_t r = 1 << sketch_size;
        Sketch sketch(r);

        std::cout << "\nTesting Sketch Runtime R = " << sketch_size << std::endl;
        std::cout << "N, time_s\n";
        for (size_t N = 6; N <= 28; N++)
        {
            uint64_t n = 1 << N;

            auto start = std::chrono::high_resolution_clock::now();
            for (int64_t i = 0; i < iter; ++i)
            {
                sketch.Update(i % n, 1);
            }
            auto end = std::chrono::high_resolution_clock::now();
            double sketch_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
            std::cout << N << ", " << round(sketch_time / 10) / 100 << "\n";
        }
    }
}

void error_benchmarks(bool good)
{
    uint64_t updates = 1000;
    uint64_t max_iter = 100;

    std::vector<int> sketch_sizes = {3, 4, 5, 6, 7, 8, 9, 10};
    if (!good)
        std::cout << "BAD HASH\n";
    std::cout << "Testing Avg. Sketch Error\n";
    std::cout << "R,err\n";

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
        std::cout << sketch_size << ", " << error / double(max_iter) << "\n";
    }

    std::cout << "\nTesting Max Sketch Error\n";
    std::cout << "R,err\n";
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
            max_error = (error > max_error && error < 550) ? error : max_error;
        }
        std::cout << sketch_size << ", " << max_error << std::endl;
    }
}

int main()
{
    // hash_benchmarks();
    // std::cout << std::endl;
    // norm_benchmarks();
    // std::cout << std::endl;
    error_benchmarks(true);
    std::cout << std::endl;
    error_benchmarks(false);
    // return 0;
}

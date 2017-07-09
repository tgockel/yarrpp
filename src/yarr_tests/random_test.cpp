#include "test.hpp"

#include <yarr/random.hpp>

#include <algorithm>
#include <array>
#include <cstring>
#include <iostream>

namespace yarr_tests
{

TEST(random, random_fill)
{
    std::array<char, 20> arr;
    std::memset(arr.data(), 0, arr.size());
    yarr::random_fill(arr.begin(), arr.end());

    // Check that something changed from 0...not the best check, but we trust the implementation
    auto non_zero_elem = std::find_if(arr.begin(), arr.end(), [] (char x) { return x != '\0'; });
    ASSERT_FALSE(non_zero_elem == arr.end());
}

TEST(random, random_seeded)
{
    // Tests that calling random_seeded works for all the standard-issue RNGs. For correctness, we will have to trust
    // that random_fill and the Standard Library implementation work.
    yarr::random_seeded<std::minstd_rand>();
    yarr::random_seeded<std::minstd_rand0>();
    yarr::random_seeded<std::mt19937>();
    yarr::random_seeded<std::mt19937_64>();
    yarr::random_seeded<std::ranlux24>();
    yarr::random_seeded<std::ranlux48>();
    yarr::random_seeded<std::knuth_b>();
}

}

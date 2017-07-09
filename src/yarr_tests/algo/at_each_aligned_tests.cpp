#include <yarr_tests/test.hpp>

#include <yarr/algo/at_each_aligned.hpp>

#include <cstdint>
#include <cstring>

namespace yarr_tests
{

TEST(at_each_aligned, b8)
{
    std::uint64_t res[1];
    yarr::at_each_aligned<std::uint64_t, std::uint8_t>
    (
        reinterpret_cast<char*>(res), reinterpret_cast<char*>(res + 1),
        [] (std::uint64_t* p64) { *p64 = 0xdeadfacebeefcafeULL; },
        [] (std::uint8_t*  p8)  { GTEST_FAIL() << "Write should be aligned"; }
    );

    ASSERT_EQ(0xdeadfacebeefcafeULL, res[0]);
}

/** Fill range `[begin, end)` in by the size of the written stride. In other words, a range aligned on 8 bytes would
 *  have \c "\x08\x08\x08\x08\x08\x08\x08\x08" written, while a single byte would have \c "\x01".
**/
static void fill_in_strides(char* begin, char* end)
{
    std::size_t bytes_processed = 0;
    yarr::at_each_aligned<std::uint64_t, std::uint32_t, std::uint16_t, std::uint8_t>
    (
        begin, end,
        [&] (std::uint64_t* p64) { bytes_processed += 8; *p64 = 0x0808080808080808ULL; },
        [&] (std::uint32_t* p32) { bytes_processed += 4; *p32 = 0x04040404; },
        [&] (std::uint16_t* p16) { bytes_processed += 2; *p16 = 0x0202; },
        [&] (std::uint8_t*  p8)  { bytes_processed += 1; *p8  = 0x01; }
    );
    ASSERT_EQ(std::distance(begin, end), bytes_processed);
}

/** Checks that range `[begin, end)` has bytes corresponding with the expected written alignment. This is the opposite
 *  of \c fill_in_strides.
**/
static void assert_in_strides(const char* begin, const char* end)
{
    yarr::at_each_aligned<std::uint64_t, std::uint32_t, std::uint16_t, std::uint8_t>
    (
        begin, end,
        [] (const std::uint64_t* p64) { ASSERT_EQ(0x0808080808080808ULL, *p64); },
        [] (const std::uint32_t* p32) { ASSERT_EQ(0x04040404,            *p32); },
        [] (const std::uint16_t* p16) { ASSERT_EQ(0x0202,                *p16); },
        [] (const std::uint8_t*  p8)  { ASSERT_EQ(0x01,                  *p8);  }
    );
}

TEST(at_each_aligned, offset_begin)
{
    alignas(16) char buffer[32];
    std::memset(buffer, 0, sizeof buffer);

    fill_in_strides(  buffer + 1, buffer + sizeof buffer);
    assert_in_strides(buffer + 1, buffer + sizeof buffer);

    char expected[] = "\x00\x01\x02\x02\x04\x04\x04\x04"
                      "\x08\x08\x08\x08\x08\x08\x08\x08"
                      "\x08\x08\x08\x08\x08\x08\x08\x08"
                      "\x08\x08\x08\x08\x08\x08\x08\x08";
    static_assert(sizeof buffer == sizeof expected - 1, "");

    ASSERT_EQ(0, std::memcmp(expected, buffer, sizeof buffer));
}

TEST(at_each_aligned, offset_end)
{
    alignas(16) char buffer[32];
    std::memset(buffer, 0, sizeof buffer);

    fill_in_strides(  buffer, buffer + sizeof buffer - 1);
    assert_in_strides(buffer, buffer + sizeof buffer - 1);

    char expected[] = "\x08\x08\x08\x08\x08\x08\x08\x08"
                      "\x08\x08\x08\x08\x08\x08\x08\x08"
                      "\x08\x08\x08\x08\x08\x08\x08\x08"
                      "\x04\x04\x04\x04\x02\x02\x01\x00";
    static_assert(sizeof buffer == sizeof expected - 1, "");

    ASSERT_EQ(0, std::memcmp(expected, buffer, sizeof buffer));
}

// Write a big chunk of data to a buffer.
TEST(at_each_aligned, big_chunk)
{
    alignas(512) char buffer[512];
    std::memset(buffer, 0, sizeof buffer);

    yarr::at_each_aligned<char[256], char>
    (
        buffer, buffer + sizeof buffer,
        [] (char (*p)[256]) { std::memset(p, '\x5c', 256); },
        [] (char*)          { GTEST_FAIL() << "Write should be aligned"; }
    );

    char expected[512];
    std::memset(expected, '\x5c', sizeof expected);

    ASSERT_EQ(0, std::memcmp(expected, buffer, sizeof buffer));
}

}

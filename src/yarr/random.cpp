#include "random.hpp"

#include <yarr/algo/at_each_aligned.hpp>

#include <cerrno>
#include <cstdint>
#include <system_error>

#include <linux/version.h>
#include <linux/random.h>
#include <sys/syscall.h>
#include <unistd.h>

namespace yarr
{

#if LINUX_VERSION_CODE > KERNEL_VERSION(3, 17, 0)

static long getrandom(void* buf, std::size_t buf_len, unsigned int flags)
{
    return syscall(SYS_getrandom, buf, buf_len, 0);
}

void random_fill(char* begin, char* end)
{
    while (begin < end)
    {
        long bytes_read = getrandom(begin, end - begin, 0);
        if (bytes_read > 0)
        {
            begin += bytes_read;
            continue;
        }
        else if (errno == EINTR)
        {
            continue;
        }
        else
        {
            throw std::system_error(errno, std::system_category());
        }
    }
}

#else // older versions of Linux

void random_fill(char* begin, char* end)
{
    std::random_device rng;
    std::uniform_int_distribution<std::uint32_t> dist;

    at_each_aligned<std::uint32_t, std::uint16_t, std::uint8_t>
    (
        begin, end,
        [&] (std::uint32_t* p) { *p = dist(rng); },
        [&] (std::uint16_t* p) { *p = dist(rng); },
        [&] (std::uint8_t* p)  { *p = dist(rng); }
    );
}

#endif

}

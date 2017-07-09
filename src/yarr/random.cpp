#include "random.hpp"

#include <cerrno>
#include <system_error>

#include <linux/random.h>
#include <sys/syscall.h>
#include <unistd.h>

namespace yarr
{

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

}

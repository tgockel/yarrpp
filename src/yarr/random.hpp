/** \file
 *  Facilities for random number generation and seeding RNGs.
 *
 *  \see http://www.gockelhut.com/cpp-pirate/random-seed.html
**/
#include <yarr/config.hpp>

#include <cstdint>
#include <random>

namespace yarr
{

/** Fill the range `[begin, end)` with random data from a random device. **/
void random_fill(char* begin, char* end);

/** A seed sequence which pulls data from a random device via \c random_fill. This is a partial implementation of the
 *  seed sequence concept and can be used as one in most cases, but it lacks functions that allow for storing or loading
 *  repeatable state.
 *
 *  \see random_seeded
**/
class random_seed_seq
{
public:
    using result_type = std::uint32_t;

public:
    void generate(result_type* begin, result_type* end)
    {
        random_fill(reinterpret_cast<char*>(begin), reinterpret_cast<char*>(end));
    }
};

/** Creates a random number engine seeded from a secure seed sequence (\c random_seed_seq).
 *
 *  \tparam TRandomNumberEngine The type of seeded random number engine to create (for example, \c std::mt19937,
 *   \c std::ranlux48, or \c std::minstd_rand).
 *
 *  \example
 *  \code
 *  auto rng = random_seeded<std::mt19937>();
 *  \endcode
**/
template <typename TRandomNumberEngine>
TRandomNumberEngine random_seeded()
{
    random_seed_seq rss;
    return TRandomNumberEngine(rss);
}

}

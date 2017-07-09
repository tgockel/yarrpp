#include "at_each_aligned.hpp"

#include <yarr/optimization_macros.hpp>

#include <utility>

namespace yarr
{

namespace detail
{

template <typename... TFArgs>
struct at_each_aligned_impl;

template <typename TFArg, typename... TFArgRest>
struct at_each_aligned_impl<TFArg, TFArgRest...>
{
    static constexpr std::size_t byte_align = sizeof(TFArg);

    template <typename TChar, typename FApply, typename... FApplyRest>
    YARR_ALWAYS_INLINE
    static TChar* step(TChar* first, TChar* last, const FApply& apply, const FApplyRest&... apply_rest)
    {
        if (reinterpret_cast<std::uintptr_t>(first) % byte_align == 0
           && first + byte_align <= last
           )
        {
            apply(reinterpret_cast<TFArg*>(first));
            return first + byte_align;
        }
        else
        {
            return at_each_aligned_impl<TFArgRest...>::step(first, last, apply_rest...);
        }
    }
};

template <typename T>
struct at_each_aligned_impl<T>
{
    static_assert(sizeof(T) == 1, "You must provide a single byte type to at_each_aligned");

    template <typename TChar, typename FApply>
    YARR_ALWAYS_INLINE
    static TChar* step(TChar* first, TChar*, const FApply& apply)
    {
        apply(reinterpret_cast<T*>(first));
        return first + 1;
    }
};

template <typename... TFArgs, typename TChar, typename... FApply>
void at_each_aligned(TChar* first, TChar* last, FApply&&... transform)
{
    for ( ; first < last; /* inline */)
    {
        first = at_each_aligned_impl<TFArgs...>::step(first, last, transform...);
    }
}

}

template <typename... TFArgs, typename... FApply>
void at_each_aligned(char* first, char* last, FApply&&... transform)
{
    detail::at_each_aligned<TFArgs...>(first, last, std::forward<FApply>(transform)...);
}

template <typename... TFArgs, typename... FApply>
void at_each_aligned(const char* first, const char* last, FApply&&... transform)
{
    detail::at_each_aligned<const TFArgs...>(first, last, std::forward<FApply>(transform)...);
}

}

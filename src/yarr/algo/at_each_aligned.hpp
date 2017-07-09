#pragma once

#include <yarr/config.hpp>
#include <yarr/optimization_macros.hpp>

#include <cstdint>

namespace yarr
{

template <typename... TFArgs, typename... FApply>
void at_each_aligned(char* first, char* last, FApply&&... transform);

template <typename... TFArgs, typename... FApply>
void at_each_aligned(const char* first, const char* last, FApply&&... transform);

}

#include "at_each_aligned.ipp"

#pragma once

#include <atomic>
#include <cstdint>
#include <new>

#include "ArgusEngine/Core/Concepts.hpp"

namespace Argus::Core
{
template <PodType Metadata>
struct ControlHeader
{
    Metadata metadata;

    alignas(std::hardware_destructive_interference_size) std::atomic<std::uint64_t> writeIndex;
    alignas(std::hardware_destructive_interference_size) std::atomic<std::uint64_t> readIndex;

    static_assert(sizeof(std::atomic<std::uint64_t>) == sizeof(std::uint64_t));
    static_assert(std::atomic<std::uint64_t>::is_always_lock_free);
};
} // namespace Argus::Core

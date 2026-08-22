#pragma once

#include <type_traits>

namespace Argus::Core
{
template <typename T>
concept PodType = std::is_standard_layout_v<T> && std::is_trivially_copyable_v<T>;
} // namespace Argus::Core

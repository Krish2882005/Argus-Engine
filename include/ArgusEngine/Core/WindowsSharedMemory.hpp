#pragma once

#include <cstddef>
#include <expected>
#include <memory>
#include <span>
#include <string>

namespace Argus::Core
{
enum class SharedMemoryError
{
    AlreadyExists,
    MappingCreationFailed,
    ViewMappingFailed
};

struct WindowsHandleDeleter
{
    void operator()(void* handle) const noexcept;
};

struct WindowsMapViewDeleter
{
    void operator()(void* view) const noexcept;
};

class WindowsSharedMemory
{
public:
    enum class Mode
    {
        Create,
        Open
    };

    [[nodiscard]] static std::expected<WindowsSharedMemory, SharedMemoryError>
    Initialize(const std::wstring& name, size_t size, Mode mode) noexcept;

    [[nodiscard]] std::span<std::byte> GetView() const noexcept;

private:
    using UniqueHandle = std::unique_ptr<void, WindowsHandleDeleter>;
    using UniqueMapView = std::unique_ptr<void, WindowsMapViewDeleter>;

    WindowsSharedMemory(UniqueHandle handle, UniqueMapView mappedView, size_t size) noexcept;

    UniqueHandle m_handle;
    UniqueMapView m_mappedView;

    size_t m_size{0};
};
} // namespace Argus::Core

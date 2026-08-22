#include "ArgusEngine/Core/WindowsSharedMemory.hpp"

#include <Windows.h>

#include <cstddef>
#include <expected>
#include <span>
#include <string>
#include <utility>

namespace Argus::Core
{
void WindowsHandleDeleter::operator()(void* handle) const noexcept
{
    if (handle && handle != INVALID_HANDLE_VALUE)
    {
        ::CloseHandle(handle);
    }
}

void WindowsMapViewDeleter::operator()(void* view) const noexcept
{
    if (view)
    {
        ::UnmapViewOfFile(view);
    }
}

std::expected<WindowsSharedMemory, SharedMemoryError>
WindowsSharedMemory::Initialize(const std::wstring& name, size_t size, Mode mode) noexcept
{
    void* rawHandle = nullptr;

    if (mode == Mode::Create)
    {
        const ULARGE_INTEGER mappingSize{.QuadPart = size};

        rawHandle = ::CreateFileMappingW(
            INVALID_HANDLE_VALUE, nullptr, PAGE_READWRITE, mappingSize.HighPart, mappingSize.LowPart, name.c_str());

        if (rawHandle && ::GetLastError() == ERROR_ALREADY_EXISTS)
        {
            ::CloseHandle(rawHandle);
            return std::unexpected(SharedMemoryError::AlreadyExists);
        }
    }
    else
    {
        rawHandle = ::OpenFileMappingW(FILE_MAP_ALL_ACCESS, FALSE, name.c_str());
    }

    if (!rawHandle)
    {
        return std::unexpected(SharedMemoryError::MappingCreationFailed);
    }

    UniqueHandle handle(rawHandle);

    void* rawMappedView = ::MapViewOfFile(handle.get(), FILE_MAP_ALL_ACCESS, 0, 0, size);

    if (!rawMappedView)
    {
        return std::unexpected(SharedMemoryError::ViewMappingFailed);
    }

    UniqueMapView mappedView(rawMappedView);

    return WindowsSharedMemory(std::move(handle), std::move(mappedView), size);
}

std::span<std::byte> WindowsSharedMemory::GetView() const noexcept
{
    if (!m_mappedView)
    {
        return {};
    }
    return {static_cast<std::byte*>(m_mappedView.get()), m_size};
}

WindowsSharedMemory::WindowsSharedMemory(UniqueHandle handle, UniqueMapView mappedView, size_t size) noexcept
    : m_handle(std::move(handle)), m_mappedView(std::move(mappedView)), m_size(size)
{
}
} // namespace Argus::Core

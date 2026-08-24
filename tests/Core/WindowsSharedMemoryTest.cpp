#include <gtest/gtest.h>

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <string>

#include "ArgusEngine/Core/WindowsSharedMemory.hpp"

namespace Argus::Core::Tests
{
// NOLINTBEGIN(readability-convert-member-functions-to-static, misc-use-internal-linkage)
class WindowsSharedMemoryTest : public ::testing::Test
{
protected:
    static std::wstring GetUniqueName()
    {
        static std::atomic<uint32_t> counter{0};
        const auto* info = ::testing::UnitTest::GetInstance()->current_test_info();

        std::string name = std::string("Local\\Argus_") + info->name() + "_" + std::to_string(counter++);

        return {name.begin(), name.end()};
    }

    static constexpr size_t kDefaultSize = 64UZ * 1024UZ;
};

TEST_F(WindowsSharedMemoryTest, CreatesSuccessfully)
{
    auto result = WindowsSharedMemory::Initialize(GetUniqueName(), kDefaultSize, WindowsSharedMemory::Mode::Create);

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->GetView().size_bytes(), kDefaultSize);
    EXPECT_NE(result->GetView().data(), nullptr);
}

TEST_F(WindowsSharedMemoryTest, FailsCreatingAlreadyExisting)
{
    const auto name = GetUniqueName();

    auto first = WindowsSharedMemory::Initialize(name, kDefaultSize, WindowsSharedMemory::Mode::Create);
    ASSERT_TRUE(first.has_value());

    auto second = WindowsSharedMemory::Initialize(name, kDefaultSize, WindowsSharedMemory::Mode::Create);

    ASSERT_FALSE(second.has_value());
    EXPECT_EQ(second.error(), SharedMemoryError::AlreadyExists);
}

TEST_F(WindowsSharedMemoryTest, FailsOpeningNonExistent)
{
    auto result = WindowsSharedMemory::Initialize(GetUniqueName(), kDefaultSize, WindowsSharedMemory::Mode::Open);

    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error(), SharedMemoryError::MappingCreationFailed);
}

TEST_F(WindowsSharedMemoryTest, SharesDataBetweenInstances)
{
    const auto name = GetUniqueName();

    auto creator = WindowsSharedMemory::Initialize(name, kDefaultSize, WindowsSharedMemory::Mode::Create);
    ASSERT_TRUE(creator.has_value());

    auto creatorView = creator->GetView();
    creatorView.front() = std::byte{0xAA};
    creatorView.back() = std::byte{0xBB};

    auto opener = WindowsSharedMemory::Initialize(name, kDefaultSize, WindowsSharedMemory::Mode::Open);
    ASSERT_TRUE(opener.has_value());

    auto openerView = opener->GetView();
    EXPECT_EQ(openerView.front(), std::byte{0xAA});
    EXPECT_EQ(openerView.back(), std::byte{0xBB});
}
// NOLINTEND(readability-convert-member-functions-to-static, misc-use-internal-linkage)
} // namespace Argus::Core::Tests

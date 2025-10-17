#include "Profile.h"

#include <print>

// -----------------------------------------------------------------------------

enum class BOOL
{
    ValueOne,
    ValueTwo,

    Count,
};

using ContainerBool = profile::EnumArray<BOOL, bool, static_cast<size_t>(BOOL::Count)>;

enum class U32
{
    ValueOne,
    ValueTwo,

    Count,
};

using ContainerU32 = profile::EnumArray<U32, uint32_t, static_cast<size_t>(U32::Count)>;

// -----------------------------------------------------------------------------
// Application Profile
// -----------------------------------------------------------------------------

ContainerBool defaultBool{ { true, false } };
ContainerU32 defaultU32{ { 123u, 456u } };

class MyProfile : public profile::Profile<ContainerBool, ContainerU32>
{
public:
    MyProfile()
        : profile::Profile<ContainerBool, ContainerU32>(defaultBool, defaultU32)
    {
    }

    void dump(const char* title) const
    {
        std::println("=== {} ===", title);

        for (size_t i = 0; i < static_cast<size_t>(BOOL::Count); i++)
        {
            auto value = get(static_cast<BOOL>(i));
            std::println("BOOL[{}] = {}", i, value);
        }

        for (size_t i = 0; i < static_cast<size_t>(U32::Count); i++)
        {
            auto value = get(static_cast<U32>(i));
            std::println("U32[{}] = {}", i, value);
        }

        std::println();
    }
};

// -----------------------------------------------------------------------------
// Application Listeners
// -----------------------------------------------------------------------------

class Listener1 final : public profile::Listener<Listener1>
{
public:
    Listener1(profile::ProfileInternal* profile)
        : profile::Listener<Listener1>(profile, "Listener1")
    {
    }

    void notifyImpl(BOOL e, bool& value)
    {
        std::println("Listener1: BOOL[{}] changed to {}", static_cast<size_t>(e), value);
    }

    void notifyImpl(U32 e, uint32_t& value)
    {
        std::println("Listener1: U32[{}] changed to {}", static_cast<size_t>(e), value);
    }
};

class Listener2 final : public profile::Listener<Listener2>
{
public:
    Listener2(profile::ProfileInternal* profile)
        : profile::Listener<Listener2>(profile, "Listener2")
    {
    }

    void notifyImpl(BOOL e, bool& value)
    {
        std::println("Listener2: BOOL[{}] changed to {}", static_cast<size_t>(e), value);
    }

    void notifyImpl(U32 e, uint32_t& value)
    {
        std::println("Listener2: U32[{}] changed to {}", static_cast<size_t>(e), value);
    }
};

int main()
{
    MyProfile profile;

    Listener1 listener1(&profile);
    Listener2 listener2(&profile);

    profile.dump("default values");

    profile.set(BOOL::ValueOne, false);
    profile.set(U32::ValueTwo, 789u);

    profile.dump("updated values");

    return 0;
}

#include "strong.h"

#include "helpers.h"

namespace strong
{
    // -------------------------------------------------------------------------
    // Default Profile Values
    // -------------------------------------------------------------------------

    ContainerBool defaultBool{ { true, false } };
    ContainerU32 defaultU32{ { 123u, 456u } };

    // -------------------------------------------------------------------------
    // Application Profile
    // -------------------------------------------------------------------------

    MyProfile::MyProfile()
        : profile_strong::Profile<ContainerBool, ContainerU32>(defaultBool, defaultU32)
    {
    }

    void MyProfile::read()
    {
        // Simulate reading from storage.

        // Notify listeners about updated values.
        Section section("* Reading Data and Notifying Listeners");
        notifyAll<BOOL>();
        notifyAll<U32>();
    }

    void MyProfile::dump(const char* title) const
    {
        Section section(title);

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
    }

    // -------------------------------------------------------------------------
    // Application Listeners
    // -------------------------------------------------------------------------

    Listener1::Listener1(profile_strong::ProfileInternal* profile)
        : profile_strong::Listener<Listener1>(profile, "Listener1")
    {
    }

    void Listener1::onProfile(BOOL e, bool& value)
    {
        std::println("Listener1: BOOL[{}] changed to {}", static_cast<size_t>(e), value);
    }

    void Listener1::onProfile(U32 e, uint32_t& value)
    {
        std::println("Listener1: U32[{}] changed to {}", static_cast<size_t>(e), value);
    }

    Listener2::Listener2(profile_strong::ProfileInternal* profile)
        : profile_strong::Listener<Listener2>(profile, "Listener2")
    {
    }

    void Listener2::onProfile(BOOL e, bool& value)
    {
        std::println("Listener2: BOOL[{}] changed to {}", static_cast<size_t>(e), value);
    }

    void Listener2::onProfile(U32 e, uint32_t& value)
    {
        std::println("Listener2: U32[{}] changed to {}", static_cast<size_t>(e), value);
    }

    // -------------------------------------------------------------------------
    // Application Entry Point
    // -------------------------------------------------------------------------

    void test()
    {
        Section section("====================================\n"
                        "=      Strong Profile Example      =\n"
                        "====================================\n");

        MyProfile profile;

        Listener1 listener1(&profile);
        Listener2 listener2(&profile);

        std::println();

        profile.read();

        profile.dump("* Default Values");

        {
            Section section("* Update Profile Values");
            profile.set(BOOL::ValueOne, false);
            profile.set(U32::ValueTwo, 789u);
        }

        profile.dump("* Updated Values");
    }

} // namespace strong

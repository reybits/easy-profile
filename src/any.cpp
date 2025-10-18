#include "any.h"

#include "helpers.h"

namespace any
{
    // -------------------------------------------------------------------------
    // Default Profile Values
    // -------------------------------------------------------------------------

    const std::array<profile_any::EnumType, static_cast<size_t>(Value::Count)> defaultValues = {
        profile_any::EnumType{ "boolOne", true },
        profile_any::EnumType{ "boolTwo", false },
        profile_any::EnumType{ "u32One", 123u },
        profile_any::EnumType{ "u32Two", 456u },
    };

    // -------------------------------------------------------------------------
    // Small Utility
    // -------------------------------------------------------------------------

    void DumpValue(const char* title, Value e, const std::any& value)
    {
        if (value.has_value() == false)
        {
            std::println("{}: No Value {}", title, static_cast<int>(e));
            return;
        }

        switch (e)
        {
        case Value::BoolOne:
        case Value::BoolTwo:
            std::println("{}Value {} = {}", title, static_cast<int>(e), std::any_cast<bool>(value));
            break;

        case Value::U32One:
        case Value::U32Two:
            std::println("{}Value {} = {}", title, static_cast<int>(e), std::any_cast<uint32_t>(value));
            break;

        case Value::Count:
            break;
        }
    }

    // -------------------------------------------------------------------------
    // Application Profile
    // -------------------------------------------------------------------------

    MyProfile::MyProfile()
        : profile_any::Profile<Value, static_cast<size_t>(Value::Count)>(defaultValues)
    {
    }

    void MyProfile::read()
    {
        // Simulate reading from storage.

        // Notify listeners about updated values.
        Section section("* Reading Data and Notifying Listeners");
        notifyAll();
    }

    void MyProfile::dump(const char* title) const
    {
        Section section(title);

        for (size_t i = 0; i < static_cast<size_t>(Value::Count); i++)
        {
            DumpValue("", static_cast<Value>(i), get(static_cast<Value>(i)));
        }
    }

    // -------------------------------------------------------------------------
    // Application Listeners
    // -------------------------------------------------------------------------

    Listener1::Listener1(profile_any::Profile<Value, static_cast<size_t>(Value::Count)>* profile)
        : profile_any::Listener<Value, static_cast<size_t>(Value::Count)>(profile, "Listener1")
    {
    }

    void Listener1::onProfile(Value e, const std::any& value) const
    {
        DumpValue("onProfile on Listener1", e, value);
    }

    Listener2::Listener2(profile_any::Profile<Value, static_cast<size_t>(Value::Count)>* profile)
        : profile_any::Listener<Value, static_cast<size_t>(Value::Count)>(profile, "Listener2")
    {
    }

    void Listener2::onProfile(Value e, const std::any& value) const
    {
        DumpValue("onProfile on Listener1", e, value);
    }

    // -------------------------------------------------------------------------
    // Application Entry Point
    // -------------------------------------------------------------------------

    void test()
    {
        Section section("====================================\n"
                        "=       Any Profile Example        =\n"
                        "====================================\n");

        MyProfile profile;

        Listener1 listener1(&profile);
        Listener2 listener2(&profile);

        std::println();

        profile.read();

        profile.dump("* Default Values");

        {
            Section section("* Update Profile Values");
            profile.set(Value::BoolOne, false);
            profile.set(Value::U32Two, 789u);
        }

        profile.dump("* Updated Values");
    }

} // namespace any

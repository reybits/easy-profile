#include "any.h"

#include "helpers.h"

#include <string>

namespace any
{
    // -------------------------------------------------------------------------
    // Default Profile Values
    // -------------------------------------------------------------------------

    const MyProfile::Container defaultValues = { {
        { "boolOne", true },
        { "boolTwo", false },
        { "u32One", 123u },
        { "u32Two", 456u },
        { "strOne", std::string{ "strOne" } },
        { "strTwo", std::string{ "strTwo" } },
    } };

    // -------------------------------------------------------------------------
    // Small Utility
    // -------------------------------------------------------------------------

    void DumpValue(const char* title, Value e, const MyProfile::Type& value)
    {
        if (value.has_value() == false)
        {
            ::printf("%s: No Value.\n", title);
            return;
        }

        switch (e)
        {
        case Value::BoolOne:
        case Value::BoolTwo:
            ::printf("%s = %s\n", title, ToString(std::any_cast<bool>(value)));
            break;

        case Value::U32One:
        case Value::U32Two:
            ::printf("%s = %u\n", title, std::any_cast<uint32_t>(value));
            break;

        case Value::StrOne:
        case Value::StrTwo:
            ::printf("%s = %s\n", title, std::any_cast<std::string>(value).c_str());
            break;

        case Value::Count:
            break;
        }
    }

    void DumpValue(Value type, const MyProfile& profile)
    {
        DumpValue(profile.getName(type), type, profile.get(type));
    }

    // -------------------------------------------------------------------------
    // Application Profile
    // -------------------------------------------------------------------------

    MyProfile::MyProfile()
        : BaseProfile(defaultValues)
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
            auto e = static_cast<Value>(i);
            DumpValue(getName(e), e, get(static_cast<Value>(i)));
        }
    }

    // -------------------------------------------------------------------------
    // Application Listeners
    // -------------------------------------------------------------------------

    Listener1::Listener1(MyProfile* profile)
        : MyProfile::Listener(profile, "Listener1")
    {
    }

    void Listener1::onProfile(Value e, const MyProfile::Type& value)
    {
        auto profile = getProfile();
        std::string title = getName();
        title += ": ";
        title += profile->getName(e);
        DumpValue(title.c_str(), e, value);
    }

    Listener2::Listener2(MyProfile* profile)
        : MyProfile::Listener(profile, "Listener2")
    {
    }

    void Listener2::onProfile(Value e, const MyProfile::Type& value)
    {
        auto profile = getProfile();
        std::string title = getName();
        title += ": ";
        title += profile->getName(e);
        DumpValue(title.c_str(), e, value);
    }

    // -------------------------------------------------------------------------
    // Application Entry Point
    // -------------------------------------------------------------------------

    void test()
    {
        Section section("====================================\n"
                        "=         Any Profile Test         =\n"
                        "====================================\n");

        MyProfile profile;

        Listener1 listener1(&profile);
        Listener2 listener2(&profile);

        ::printf("\n");

        profile.read();

        profile.dump("* Default Values");

        {
            Section section("* Update Profile Values");
            profile.set(Value::BoolOne, false);
            profile.set(Value::U32Two, 789u);
            profile.set(Value::StrOne, std::string{ "NewStringOne" });
        }

        {
            Section section("* Read Profile Values");
            DumpValue(Value::BoolOne, profile);
            DumpValue(Value::U32Two, profile);
            DumpValue(Value::StrOne, profile);
        }

        profile.dump("* Updated Values");
    }

} // namespace any

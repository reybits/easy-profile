#include "variant.h"

#include "helpers.h"

#include <string>

namespace variant
{
    // -------------------------------------------------------------------------
    // Default Profile Values
    // -------------------------------------------------------------------------

    const MyProfile::Container defaultValues = { {
        { "boolOne", true },
        { "boolTwo", false },
        { "u32One", 123u },
        { "u32Two", 456u },
        { "strOne", std::string{ "StringOne" } },
        { "strTwo", std::string{ "StringTwo" } },
    } };

    // -------------------------------------------------------------------------
    // Small Utility
    // -------------------------------------------------------------------------

    void DumpValue(const char* title, Value e, const MyProfile::Type& value)
    {
        switch (e)
        {
        case Value::BoolOne:
        case Value::BoolTwo:
            ::printf("%s = %s\n", title, ToString(std::get<bool>(value)));
            break;

        case Value::U32One:
        case Value::U32Two:
            ::printf("%s = %u\n", title, std::get<uint32_t>(value));
            break;

        case Value::StrOne:
        case Value::StrTwo:
            ::printf("%s = %s\n", title, std::get<std::string>(value).c_str());
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
            // std::visit([i](auto&& v) {
            //     ::printf("Changed %zu: %u\n", i, v);
            // },
            //            e);
        }
    }

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
        // std::visit([e, this](auto&& v) {
        //     ::printf("onProfile on %s: Value %d = %u\n", getName(), static_cast<int>(e), v);
        // },
        //            value);
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
        // std::visit([e, this](auto&& v) {
        //     ::printf("onProfile on %s: Value %d = %u\n", getName(), static_cast<int>(e), v);
        // },
        //            value);
    }

    // -------------------------------------------------------------------------

    void test()
    {
        Section section("====================================\n"
                        "=       Variant Profile Test       =\n"
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

} // namespace variant

#include "def.h"

#include "helpers.h"

namespace def
{
    // -------------------------------------------------------------------------
    // Default Profile Values
    // -------------------------------------------------------------------------

    std::array<bool, 2> defaultBool = {
        true,
        false
    };

    std::array<uint32_t, 2> defaultU32{
        123u,
        456u
    };

    std::array<std::string, 2> defaultStr{
        std::string{ "StrOne" },
        std::string{ "StrTwo" }
    };

    // -------------------------------------------------------------------------
    // Small Utility
    // -------------------------------------------------------------------------

    template <typename Enum, typename T>
    void DumpValue(const char* title, Enum, const T& value)
    {
        if constexpr (std::is_same_v<Enum, BOOL> && std::is_same_v<T, bool>)
        {
            ::printf("BOOL[%s] = %s\n", title, value ? "true" : "false");
        }
        else if constexpr (std::is_same_v<Enum, U32> && std::is_same_v<T, uint32_t>)
        {
            ::printf("U32[%s] = %u\n", title, value);
        }
        else if constexpr (std::is_same_v<Enum, STR> && std::is_same_v<T, std::string>)
        {
            ::printf("STR[%s] = %s\n", title, value.c_str());
        }
        else
        {
            ::printf("[%s] = unknown.\n", title);
        }
    }

    template <typename Enum>
    void DumpValue(Enum type, const MyProfile& profile)
    {
        DumpValue("profile.getName(type)", type, profile.get(type));
    }

    template <typename Enum>
    std::string ToTitle(profile::Profile* profile, const char* name, Enum e)
    {
        std::string title = name;
        title += ": ";
        // title += profile->getName(e);
        return title;
    }

    // -------------------------------------------------------------------------
    // Application Profile
    // -------------------------------------------------------------------------

    MyProfile::MyProfile()
        : profile::Profile(defaultBool, defaultU32, defaultStr)
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

        for (size_t i = 0; i < static_cast<size_t>(BOOL::Count); i++)
        {
            auto value = get(static_cast<BOOL>(i));
            auto name = ""; // getName(static_cast<BOOL>(i));
            ::printf("BOOL[%s] = %s\n", name, value ? "true" : "false");
        }

        for (size_t i = 0; i < static_cast<size_t>(U32::Count); i++)
        {
            auto value = get(static_cast<U32>(i));
            auto name = ""; // getName(static_cast<U32>(i));
            ::printf("U32[%s] = %u\n", name, value);
        }

        for (size_t i = 0; i < static_cast<size_t>(STR::Count); i++)
        {
            auto value = get(static_cast<STR>(i));
            auto name = ""; // getName(static_cast<STR>(i));
            ::printf("STR[%s] = %s\n", name, value.c_str());
        }
    }

    // -------------------------------------------------------------------------
    // Application Listeners
    // -------------------------------------------------------------------------

    Listener1::Listener1(profile::Profile* profile)
        : profile::Profile::Listener(profile, "Listener1")
    {
    }

    void Listener1::onProfile(BOOL e, const bool& value)
    {
        auto title = ToTitle(getProfile(), getName(), e);
        ::printf("%s = %s\n", title.c_str(), value ? "true" : "false");
    }

    void Listener1::onProfile(U32 e, const uint32_t& value)
    {
        auto title = ToTitle(getProfile(), getName(), e);
        ::printf("%s = %u\n", title.c_str(), value);
    }

    void Listener1::onProfile(STR e, const std::string& value)
    {
        auto title = ToTitle(getProfile(), getName(), e);
        ::printf("%s = %s\n", title.c_str(), value.c_str());
    }

    Listener2::Listener2(profile::Profile* profile)
        : profile::Profile::Listener(profile, "Listener2")
    {
    }

    void Listener2::onProfile(BOOL e, const bool& value)
    {
        auto title = ToTitle(getProfile(), getName(), e);
        ::printf("%s = %s\n", title.c_str(), value ? "true" : "false");
    }

    void Listener2::onProfile(U32 e, const uint32_t& value)
    {
        auto title = ToTitle(getProfile(), getName(), e);
        ::printf("%s = %u\n", title.c_str(), value);
    }

    void Listener2::onProfile(STR e, const std::string& value)
    {
        auto title = ToTitle(getProfile(), getName(), e);
        ::printf("%s = %s\n", title.c_str(), value.c_str());
    }

    // -------------------------------------------------------------------------
    // Application Entry Point
    // -------------------------------------------------------------------------

    void test()
    {
        Section section("====================================\n"
                        "=        Strong Profile Test       =\n"
                        "====================================\n");

        MyProfile profile;

        Listener1 listener1(&profile);
        Listener2 listener2(&profile);

        ::printf("\n");

        profile.read();

        profile.dump("* Default Values");

        {
            Section section("* Update Profile Values");
            profile.set(BOOL::ValueOne, false);
            profile.set(U32::ValueTwo, 789u);
            profile.set(STR::ValueOne, std::string{ "NewStringOne" });
        }

        {
            Section section("* Read Profile Values");
            DumpValue(BOOL::ValueOne, profile);
            DumpValue(U32::ValueTwo, profile);
            DumpValue(STR::ValueOne, profile);
        }

        profile.dump("* Updated Values");
    }

} // namespace def

#include <array>
#include <cstdint>
#include <string>
#include <vector>

// -------------------------------------------------------------------------
// Application Profile Values
// -------------------------------------------------------------------------

enum class BOOL
{
    ValueOne,
    ValueTwo,

    Count
};

enum class U32
{
    ValueOne,
    ValueTwo,

    Count
};

enum class STR
{
    ValueOne,
    ValueTwo,

    Count
};

#define PROFILE_TYPES                                                 \
    PROFILE_TYPE(BOOL, Bool, bool, static_cast<size_t>(BOOL::Count))  \
    PROFILE_TYPE(U32, U32, uint32_t, static_cast<size_t>(U32::Count)) \
    PROFILE_TYPE(STR, Str, std::string, static_cast<size_t>(STR::Count))

#include "EasyProfile.h"

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

class Section final
{
public:
    Section(const char* title)
    {
        ::printf("%s\n", title);
    }

    ~Section()
    {
        ::printf("\n");
    }
};

template <typename Enum>
std::string ToTitle(easyprofile::Profile* profile, const char* name, Enum e)
{
    std::string title = name;
    title += ": ";
    // title += profile->getName(e);
    return title;
}

// -------------------------------------------------------------------------
// Application Profile
// -------------------------------------------------------------------------

class MyProfile : public easyprofile::Profile
{
public:
    // Setup profile with default values.
    MyProfile()
        : easyprofile::Profile(defaultBool, defaultU32, defaultStr)
    {
    }

    void read()
    {
        // Read profile values from storage, set them and optionally notify listeners.

        auto notifyListeners = false;

        for (uint32_t i = 0; i < static_cast<uint32_t>(BOOL::Count); i++)
        {
            set(static_cast<BOOL>(i), true, notifyListeners);
        }

        for (uint32_t i = 0; i < static_cast<uint32_t>(U32::Count); i++)
        {
            set(static_cast<U32>(i), (i + 1) * 10, notifyListeners);
        }

        for (uint32_t i = 0; i < static_cast<uint32_t>(STR::Count); i++)
        {
            set(static_cast<STR>(i), std::string("Value ") + std::to_string(i), notifyListeners);
        }
    }

    void dump(const char* title) const
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
};

// -------------------------------------------------------------------------
// Application Listeners
// -------------------------------------------------------------------------

class Listener1 final : public easyprofile::Profile::Listener
{
public:
    Listener1(easyprofile::Profile* profile)
        : easyprofile::Profile::Listener(profile, "Listener1")
    {
    }

    void onProfile(BOOL e, const bool& value)
    {
        auto title = ToTitle(getProfile(), getName(), e);
        ::printf("%s = %s\n", title.c_str(), value ? "true" : "false");
    }

    void onProfile(U32 e, const uint32_t& value)
    {
        auto title = ToTitle(getProfile(), getName(), e);
        ::printf("%s = %u\n", title.c_str(), value);
    }

    void onProfile(STR e, const std::string& value)
    {
        auto title = ToTitle(getProfile(), getName(), e);
        ::printf("%s = %s\n", title.c_str(), value.c_str());
    }
};

class Listener2 final : public easyprofile::Profile::Listener
{
public:
    Listener2(easyprofile::Profile* profile)
        : easyprofile::Profile::Listener(profile, "Listener2")
    {
    }

    void onProfile(BOOL e, const bool& value)
    {
        auto title = ToTitle(getProfile(), getName(), e);
        ::printf("%s = %s\n", title.c_str(), value ? "true" : "false");
    }

    void onProfile(U32 e, const uint32_t& value)
    {
        auto title = ToTitle(getProfile(), getName(), e);
        ::printf("%s = %u\n", title.c_str(), value);
    }

    void onProfile(STR e, const std::string& value)
    {
        auto title = ToTitle(getProfile(), getName(), e);
        ::printf("%s = %s\n", title.c_str(), value.c_str());
    }
};

// -------------------------------------------------------------------------
// Application Entry Point
// -------------------------------------------------------------------------

int main()
{
    Section section("====================================\n"
                    "=         Easy Profile Test        =\n"
                    "====================================\n");

    MyProfile profile;

    Listener1 listener1(&profile);
    Listener2 listener2(&profile);

    ::printf("\n");

    profile.dump("* Default Values");

    profile.read();
    profile.dump("* Stored Values");

    {
        Section section("* Update Profile Values");
        profile.set(BOOL::ValueOne, false);
        profile.set(U32::ValueTwo, 789u);
        profile.set(STR::ValueOne, std::string{ "NewStringOne" });
    }

    {
        Section section("* Read Profile Values");
        ::printf("BOOL::ValueOne: %s\n", profile.get(BOOL::ValueOne) ? "true" : "false");
        ::printf("U32::ValueTwo: %u\n", profile.get(U32::ValueTwo));
        ::printf("STR::ValueOne: %s\n", profile.get(STR::ValueOne).c_str());
    }

    profile.dump("* Updated Values");

    return 0;
}

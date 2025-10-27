# Easy Profile library

The `Profile` class encapsulates the configuration and state for a user or application profile. It manages profile-specific settings, such as name, parameters, and other customizable options.

## Example usage

Define your enums.

```cpp
enum class BOOL
{
    ValueOne,
    ValueTwo,

    Count,
};

enum class U32
{
    ValueOne,
    ValueTwo,

    Count,
};

enum class STR
{
    ValueOne,
    ValueTwo,

    Count
};
```

And define containers for them.

```cpp
#define PROFILE_TYPES                                                    \
    PROFILE_TYPE(BOOL, BOOL, uint32_t, static_cast<size_t>(BOOL::Count)) \
    PROFILE_TYPE(U32, U32, uint32_t, static_cast<size_t>(U32::Count))    \
    PROFILE_TYPE(STR, STR, std::string, static_cast<size_t>(STR::Count))

#include "EasyProfile.h"
```

Default values.

```cpp
const std::array<bool, 2> defaultBool = {
    true,
    false
};

const std::array<uint32_t, 2> defaultU32{
    123u,
    456u
};

const std::array<std::string, 2> defaultStr{
    std::string{ "StrOne" },
    std::string{ "StrTwo" }
};
```

Define your profile by inheriting from profile::Profile.

```cpp
class MyProfile : public profile::Profile
{
public:
    MyProfile()
        : BaseProfile(defaultBool, defaultU32, defaultStr)
    {
    }
};
```

Define a listener by inheriting from profile::Listener.

```cpp
class MyListener final : public profile::Profile::Listener
{
public:
    MyListener(profile::Profile* profile)
        : profile::Profile::Listener(profile, "MyListener")
    {
    }

    void MyListener::onProfile(BOOL e, const bool& value)
    {
        auto profile = getProfile();
        std::string title = getName();
        title += ": ";
        std::println("{} = {}", title, value ? "true" : "false");
    }

    void MyListener::onProfile(U32 e, const uint32_t& value)
    {
        auto profile = getProfile();
        std::string title = getName();
        title += ": ";
        std::println("{} = {}", title, value);
    }

    void MyListener::onProfile(STR e, const std::string& value)
    {
        auto profile = getProfile();
        std::string title = getName();
        title += ": ";
        std::println("{} = {}", title, value);
    }
};
```

Initialize profile and listener, and change some values.

```cpp
void InitilizeProfile()
{
    // Create profile instance.
    MyProfile myProfile;

    // Create listener instance and register it to the profile.
    MyListener myListener(&myProfile);

    // Change some values in the profile.
    myProfile.set(BOOL::ValueOne, false); // This will trigger MyListener's notifyImpl for BOOL.
    myProfile.set(U32::ValueTwo, 789u);   // This will trigger MyListener's notifyImpl for U32.
    myProfile.set(STR::ValueOne, std::string{ "NewStringOne" }); // This will trigger MyListener's notifyImpl for STR.

    std::println("BOOL ValueOne: {}", myProfile.get(BOOL::ValueOne));
    std::println("U32 ValueTwo: {}", myProfile.get(U32::ValueTwo));
    std::println("STR ValueOne: {}", myProfile.get(STR::ValueOne));
}
```

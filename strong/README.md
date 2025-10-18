# Easy Profile library

> WIP: This library is under development. The API and features may change in future releases.

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
#include <tuple>
#include <vector>

using ContainerBool = profile::Container<BOOL, bool, static_cast<size_t>(BOOL::Count)>;
using ContainerU32 = profile::Container<U32, uint32_t, static_cast<size_t>(U32::Count)>;
using ContainerStr = profile::Container<STR, std::string, static_cast<size_t>(STR::Count)>;
```

Default values.

```cpp
ContainerBool defaultBool = {
    { ContainerBool::Entry{ "boolOne", true },
      ContainerBool::Entry{ "boolOne", false } }
};

ContainerU32 defaultU32{
    { ContainerU32::Entry{ "u32One", 123u },
      ContainerU32::Entry{ "u32Two", 456u } }
};

ContainerStr defaultStr{
    { ContainerStr::Entry{ "StrOne", std::string{ "StrOne" } },
      ContainerStr::Entry{ "StrTwo", std::string{ "StrTwo" } } }
};
```

Define your profile by inheriting from profile::Profile.

```cpp
using BaseProfile = profile::Profile<ContainerBool, ContainerU32, ContainerStr>;

class MyProfile : public BaseProfile
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
class MyListener final : public MyProfile::Listener
{
public:
    MyListener(MyProfile* profile)
        : MyProfile::Listener(profile, "MyListener")
    {
    }

    void MyListener::onProfile(BOOL e, bool& value)
    {
        auto profile = getProfile();
        std::string title = getName();
        title += ": ";
        title += profile->getName(e);
        std::println("{} = {}", title, value ? "true" : "false");
    }

    void MyListener::onProfile(U32 e, uint32_t& value)
    {
        auto profile = getProfile();
        std::string title = getName();
        title += ": ";
        title += profile->getName(e);
        std::println("{} = {}", title, value);
    }

    void MyListener::onProfile(STR e, const std::string& value)
    {
        auto profile = getProfile();
        std::string title = getName();
        title += ": ";
        title += profile->getName(e);
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

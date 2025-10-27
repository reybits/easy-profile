# Easy Profile library

> WIP: This library is under development. The API and features may change in future releases.

The `Profile` class encapsulates the configuration and state for a user or application profile. It manages profile-specific settings, such as name, parameters, and other customizable options.

## Example usage

Define your enums.

```cpp
enum class Value
{
    BoolOne,
    BoolTwo,

    U32One,
    U32Two,

    StrOne,
    StrTwo,

    Count
};
```

And define default values and names for them.

```cpp
const MyProfile::Container defaultValues = { {
    { "boolOne", true },
    { "boolTwo", false },
    { "u32One", 123u },
    { "u32Two", 456u },
    { "strOne", std::string{ "strOne" } },
    { "strTwo", std::string{ "strTwo" } },
} };
```

Define your profile by inheriting from profile::Profile.

```cpp
#include <any>
#include <array>
#include <cassert>
#include <vector>

using BaseProfile = profile::Profile<Value, static_cast<size_t>(Value::Count)>;

class MyProfile : public BaseProfile
{
    MyProfile()
        : BaseProfile(defaultValues)
    {
    }
};
```

Define a listener by inheriting from profile::Listener.

```cpp
class Listener1 final : public MyProfile::Listener
{
public:
    MyListener(MyProfile* profile)
        : MyProfile::Listener(profile, "MyListener")
    {
    }

    void onProfile(Value e, const MyProfile::Any& value) override
    {
        switch (e)
        {
        case Value::BoolOne:
        case Value::BoolTwo:
            ::printf("%s: Value %d = %d\n", title, static_cast<int>(e), std::any_cast<bool>(value));
            break;

        case Value::U32One:
        case Value::U32Two:
            ::printf("%s: Value %d = %u\n", title, static_cast<int>(e), std::any_cast<uint32_t>(value));
            break;

        case Value::StrOne:
        case Value::StrTwo:
            ::printf("%s: Value %d = %s\n", title, static_cast<int>(e), std::any_cast<std::string>(value).c_str());
            break;

        case Value::Count:
            break;
        }
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
    // This will trigger MyListener's onProfile(Value e, const std::any& value).
    myProfile.set(Value::BoolOne, false);
    myProfile.set(Value::U32Two, 789u);
    myProfile.set(Value::StrOne, std::string{ "NewStringOne" });

    ::printf("BoolOne: %d\n", std::any_cast<bool>(myProfile.get(Value::BoolOne)));
    ::printf("U32Two: %u\n", std::any_cast<uint32_t>(myProfile.get(Value::U32Two)));
    ::printf("StrOne: %u\n", std::any_cast<std::string>(myProfile.get(Value::StrOne)));
}
```

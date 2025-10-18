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

    Count,
};
```

And define default values and names for them.

```cpp
const std::array<profile::EnumType, static_cast<size_t>(Value::Count)> defaultValues = {
    profile::EnumType{ "boolOne", true },
    profile::EnumType{ "boolTwo", false },
    profile::EnumType{ "u32One", 123u },
    profile::EnumType{ "u32Two", 456u },
};
```

Define your profile by inheriting from profile::Profile.

```cpp
class MyProfile : public profile::Profile<Value, static_cast<size_t>(Value::Count)>
{
    MyProfile()
        : profile::Profile<Value, static_cast<size_t>(Value::Count)>(defaultValues)
    {
    }
};
```

Define a listener by inheriting from profile::Listener.

```cpp
class MyListener final : public profile::Listener<Value, static_cast<size_t>(Value::Count)>
{
public:
    MyListener(profile::Profile<Value, static_cast<size_t>(Value::Count)>* profile)
        : profile::Listener<Value, static_cast<size_t>(Value::Count)>(profile, "Listener1")
    {
    }

    void onProfile(Value e, const std::any& value) const override
    {
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

    std::println("BoolOne: {}", std::any_cast<bool>(myProfile.get(Value::BoolOne)));
    std::println("U32Two: {}", std::any_cast<bool>(myProfile.get(Value::U32Two)));
}
```

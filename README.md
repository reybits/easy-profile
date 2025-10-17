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
```

And define containers for them.

```cpp
using ContainerBool = profile::EnumArray<BOOL, bool, static_cast<size_t>(BOOL::Count)>;
using ContainerU32 = profile::EnumArray<U32, uint32_t, static_cast<size_t>(U32::Count)>;
```

Default values.

```cpp
ContainerBool defaultBool{ { true, false } };
ContainerU32 defaultU32{ { 123u, 456u } };
```

Define your profile by inheriting from profile::Profile.

```cpp
class MyProfile : public profile::Profile<ContainerBool, ContainerU32>
{
    MyProfile()
        : profile::Profile<ContainerBool, ContainerU32>(defaultBool, defaultU32)
    {
    }
};
```

Define a listener by inheriting from profile::Listener.

```cpp
class MyListener final : public profile::Listener<MyListener>
{
public:
    MyListener(profile::ProfileInternal* profile)
        : profile::Listener<MyListener>(profile, "MyListener")
    {
    }

    void notifyImpl(BOOL e, const bool& value)
    {
        std::println("MyListener: BOOL[{}] changed to {}", static_cast<size_t>(e), value);
    }

    void notifyImpl(U32 e, const uint32_t& value)
    {
        std::println("MyListener: U32[{}] changed to {}", static_cast<size_t>(e), value);
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

    std::println("BOOL ValueOne: {}", myProfile.get(BOOL::ValueOne));
    std::println("U32 ValueTwo: {}", myProfile.get(U32::ValueTwo));
}
```

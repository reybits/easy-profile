# Easy Profile library

The main idea of this library is to provide a simple and efficient way to manage profile settings using enums as keys, enabling type-safe access and modification of profile data.
This implementation is blazing fast, easy to use, and highly extensible.

Default values and storage are managed on the developer side, giving you full control over memory management and data persistence.

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
    PROFILE_TYPE(BOOL, Bool, bool, static_cast<size_t>(BOOL::Count))     \
    PROFILE_TYPE(U32, U32, uint32_t, static_cast<size_t>(U32::Count))    \
    PROFILE_TYPE(STR, Str, std::string, static_cast<size_t>(STR::Count))

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

Define your profile by inheriting from easyprofile::Profile.

```cpp
class MyProfile : public easyprofile::Profile
{
public:
    MyProfile()
        : easyprofile::Profile(defaultBool, defaultU32, defaultStr)
    {
    }
};
```

Define a listener by inheriting from easyprofile::Profile::Listener.

```cpp
class MyListener final : public easyprofile::Profile::Listener
{
public:
    MyListener(easyprofile::Profile* profile)
        : easyprofile::Profile::Listener(profile, "MyListener")
    {
    }

    void onProfile(BOOL e, const bool& value) override
    {
        auto title = std::string(getName()) + ": ";
        ::printf("%s = %s\n", title.c_str(), value ? "true" : "false");
    }

    void onProfile(U32 e, const uint32_t& value) override
    {
        auto title = std::string(getName()) + ": ";
        ::printf("%s = %u\n", title.c_str(), value);
    }

    void onProfile(STR e, const std::string& value) override
    {
        auto title = std::string(getName()) + ": ";
        ::printf("%s = %s\n", title.c_str(), value.c_str());
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
    myProfile.set(BOOL::ValueOne, false); // This will trigger MyListener's onProfile for BOOL.
    myProfile.set(U32::ValueTwo, 789u);   // This will trigger MyListener's onProfile for U32.
    myProfile.set(STR::ValueOne, std::string{ "NewStringOne" }); // This will trigger MyListener's onProfile for STR.

    std::println("BOOL ValueOne: {}", myProfile.get(BOOL::ValueOne));
    std::println("U32 ValueTwo: {}", myProfile.get(U32::ValueTwo));
    std::println("STR ValueOne: {}", myProfile.get(STR::ValueOne));
}
```

## Use preprocessor

You can also use preprocessor to create human and debugger friendly source.

Create file `make_profile.h` with the following content:

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

#define PROFILE_TYPES                                                    \
    PROFILE_TYPE(BOOL, Bool, bool, static_cast<size_t>(BOOL::Count))     \
    PROFILE_TYPE(U32, U32, uint32_t, static_cast<size_t>(U32::Count))    \
    PROFILE_TYPE(STR, Str, std::string, static_cast<size_t>(STR::Count))

#include "EasyProfile.h"
```

Run the following command to generate `MyEasyProfile.h`:

```bash
g++ -E -P make_profile.cpp | clang-format > MyEasyProfile.h
```

Or with a copyright header comment and C++ include guard:

```bash
(echo "/*\n* Easy Profile library\n* by Andrey A. Ugolnik\n* https://github.com/reybits\n*/\n\n\#pragma once\n" ; g++ -E -P make_profile.cpp) | clang-format > MyEasyProfile.h
```

Include `MyEasyProfile.h` in your project and use it as usual.


```
by Andrey A. Ugolnik
and@reybits.dev
https://github.com/reybits
```


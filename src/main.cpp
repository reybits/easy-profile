#include "any/include/Profile.h"
// #include "strong/include/Profile.h"

#include <print>

// -----------------------------------------------------------------------------

class Section final
{
public:
    Section(const char* title)
    {
        std::println("* {}", title);
    }

    ~Section()
    {
        std::println();
    }
};

// -----------------------------------------------------------------------------

namespace strong
{
    void strong()
    {
        Section section("Strong Profile Example");
    }

} // namespace strong

// -----------------------------------------------------------------------------

namespace any
{
    // -----------------------------------------------------------------------------
    // Application Profile Values
    // -----------------------------------------------------------------------------

    enum class Value
    {
        BoolOne,
        BoolTwo,

        U32One,
        U32Two,

        Count,
    };

    // -----------------------------------------------------------------------------
    // Default Profile Values
    // -----------------------------------------------------------------------------

    const std::array<profile::EnumType, static_cast<size_t>(Value::Count)> defaultValues = {
        profile::EnumType{ "boolOne", true },
        profile::EnumType{ "boolTwo", false },
        profile::EnumType{ "u32One", 123u },
        profile::EnumType{ "u32Two", 456u },
    };

    // -----------------------------------------------------------------------------
    // Small Utility
    // -----------------------------------------------------------------------------

    void DumpValue(const char* title, Value e, const std::any& value)
    {
        if (value.has_value() == false)
        {
            std::println("{}: No Value {}", title, static_cast<int>(e));
            return;
        }

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

    // -----------------------------------------------------------------------------
    // Application Profile
    // -----------------------------------------------------------------------------

    class MyProfile : public profile::Profile<Value, static_cast<size_t>(Value::Count)>
    {
    public:
        MyProfile()
            : profile::Profile<Value, static_cast<size_t>(Value::Count)>(defaultValues)
        {
        }

        void read()
        {
            // Simulate reading from storage.

            // Notify listeners about updated values.
            Section section("Reading Data and Notifying Listeners");
            notifyAll();
        }

        void writ()
        {
            // Simulate writing to storage.
            Section section("Simulated Profile Write");
        }

        void dump(const char* title) const
        {
            Section section(title);

            for (size_t i = 0; i < static_cast<size_t>(Value::Count); i++)
            {
                DumpValue("", static_cast<Value>(i), get(static_cast<Value>(i)));
            }
        }
    };

    // -----------------------------------------------------------------------------
    // Application Listeners
    // -----------------------------------------------------------------------------

    class Listener1 final : public profile::Listener<Value, static_cast<size_t>(Value::Count)>
    {
    public:
        Listener1(profile::Profile<Value, static_cast<size_t>(Value::Count)>* profile)
            : profile::Listener<Value, static_cast<size_t>(Value::Count)>(profile, "Listener1")
        {
        }

        void onProfile(Value e, const std::any& value) const override
        {
            DumpValue("onProfile on Listener1", e, value);
        }
    };

    class Listener2 final : public profile::Listener<Value, static_cast<size_t>(Value::Count)>
    {
    public:
        Listener2(profile::Profile<Value, static_cast<size_t>(Value::Count)>* profile)
            : profile::Listener<Value, static_cast<size_t>(Value::Count)>(profile, "Listener2")
        {
        }

        void onProfile(Value e, const std::any& value) const override
        {
            DumpValue("onProfile on Listener1", e, value);
        }
    };

    void any()
    {
        Section section("Any Profile Example");

        MyProfile profile;

        Listener1 listener1(&profile);
        Listener2 listener2(&profile);

        std::println();

        profile.read();

        profile.dump("Dump Default Values");

        {
            Section section("Updating Profile Values");
            profile.set(Value::BoolOne, false);
            profile.set(Value::U32Two, 789u);
        }

        profile.dump("Dump Updated Values");
    }

} // namespace any

// -----------------------------------------------------------------------------

int main()
{
    any::any();

    strong::strong();

    return 0;
}

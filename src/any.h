#pragma once

#include "any/include/Profile.h"

namespace any
{
    // -------------------------------------------------------------------------
    // Application Profile Values
    // -------------------------------------------------------------------------

    enum class Value
    {
        BoolOne,
        BoolTwo,

        U32One,
        U32Two,

        Count,
    };

    // -------------------------------------------------------------------------
    // Application Profile
    // -------------------------------------------------------------------------

    class MyProfile : public profile_any::Profile<Value, static_cast<size_t>(Value::Count)>
    {
    public:
        MyProfile();

        void read();
        void dump(const char* title) const;
    };

    // -------------------------------------------------------------------------
    // Application Listeners
    // -------------------------------------------------------------------------

    class Listener1 final : public profile_any::Listener<Value, static_cast<size_t>(Value::Count)>
    {
    public:
        Listener1(profile_any::Profile<Value, static_cast<size_t>(Value::Count)>* profile);

        void onProfile(Value e, const std::any& value) const override;
    };

    class Listener2 final : public profile_any::Listener<Value, static_cast<size_t>(Value::Count)>
    {
    public:
        Listener2(profile_any::Profile<Value, static_cast<size_t>(Value::Count)>* profile);

        void onProfile(Value e, const std::any& value) const override;
    };

    // -------------------------------------------------------------------------
    // Application Entry Point
    // -------------------------------------------------------------------------

    void test();

} // namespace any

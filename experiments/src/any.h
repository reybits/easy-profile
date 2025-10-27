#pragma once

#include <any>
#include <array>
#include <cassert>
#include <vector>

namespace any
{
#include "any/include/Profile.h"

    // -------------------------------------------------------------------------
    // Application Profile Values
    // -------------------------------------------------------------------------

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

    // -------------------------------------------------------------------------
    // Application Profile
    // -------------------------------------------------------------------------

    using BaseProfile = profile::Profile<Value, static_cast<size_t>(Value::Count)>;

    class MyProfile : public BaseProfile
    {
    public:
        MyProfile();

        void read();
        void dump(const char* title) const;
    };

    // -------------------------------------------------------------------------
    // Application Listeners
    // -------------------------------------------------------------------------

    class Listener1 final : public MyProfile::Listener
    {
    public:
        Listener1(MyProfile* profile);

        void onProfile(Value e, const MyProfile::Type& value) override;
    };

    class Listener2 final : public MyProfile::Listener
    {
    public:
        Listener2(MyProfile* profile);

        void onProfile(Value e, const MyProfile::Type& value) override;
    };

    // -------------------------------------------------------------------------
    // Application Entry Point
    // -------------------------------------------------------------------------

    void test();

} // namespace any

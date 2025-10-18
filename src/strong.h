#pragma once

#include "strong/include/Profile.h"

namespace strong
{
    // -------------------------------------------------------------------------
    // Application Profile Values
    // -------------------------------------------------------------------------

    enum class BOOL
    {
        ValueOne,
        ValueTwo,

        Count,
    };

    using ContainerBool = profile_strong::EnumArray<BOOL, bool, static_cast<size_t>(BOOL::Count)>;

    enum class U32
    {
        ValueOne,
        ValueTwo,

        Count,
    };

    using ContainerU32 = profile_strong::EnumArray<U32, uint32_t, static_cast<size_t>(U32::Count)>;

    // -------------------------------------------------------------------------
    // Application Profile
    // -------------------------------------------------------------------------

    class MyProfile : public profile_strong::Profile<ContainerBool, ContainerU32>
    {
    public:
        MyProfile();

        void read();
        void dump(const char* title) const;
    };

    // -------------------------------------------------------------------------
    // Application Listeners
    // -------------------------------------------------------------------------

    class Listener1 final : public profile_strong::Listener<Listener1>
    {
    public:
        Listener1(profile_strong::ProfileInternal* profile);

        void onProfile(BOOL e, bool& value);
        void onProfile(U32 e, uint32_t& value);
    };

    class Listener2 final : public profile_strong::Listener<Listener2>
    {
    public:
        Listener2(profile_strong::ProfileInternal* profile);

        void onProfile(BOOL e, bool& value);
        void onProfile(U32 e, uint32_t& value);
    };

    // -------------------------------------------------------------------------
    // Application Entry Point
    // -------------------------------------------------------------------------

    void test();

} // namespace strong

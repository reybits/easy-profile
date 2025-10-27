#pragma once

#include <array>
#include <tuple>
#include <vector>

namespace strong
{
#include "simple/include/Profile.h"

    // -------------------------------------------------------------------------
    // Application Profile Values
    // -------------------------------------------------------------------------

    enum class BOOL
    {
        ValueOne,
        ValueTwo,

        Count
    };

    using ContainerBool = profile::Container<BOOL, bool, static_cast<size_t>(BOOL::Count)>;

    enum class U32
    {
        ValueOne,
        ValueTwo,

        Count
    };

    using ContainerU32 = profile::Container<U32, uint32_t, static_cast<size_t>(U32::Count)>;

    enum class STR
    {
        ValueOne,
        ValueTwo,

        Count
    };

    using ContainerStr = profile::Container<STR, std::string, static_cast<size_t>(STR::Count)>;

    // -------------------------------------------------------------------------
    // Application Profile
    // -------------------------------------------------------------------------

    using BaseProfile = profile::Profile<ContainerBool, ContainerU32, ContainerStr>;

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

    // class Listener1 final : public MyProfile::Listener
    // {
    // public:
    //     Listener1(MyProfile* profile);
    //
    //     void onProfile(BOOL e, bool& value);
    //     void onProfile(U32 e, uint32_t& value);
    //     void onProfile(STR e, const std::string& value);
    // };
    //
    // class Listener2 final : public MyProfile::Listener
    // {
    // public:
    //     Listener2(MyProfile* profile);
    //
    //     void onProfile(BOOL e, bool& value);
    //     void onProfile(U32 e, uint32_t& value);
    //     void onProfile(STR e, const std::string& value);
    // };

    // -------------------------------------------------------------------------
    // Application Entry Point
    // -------------------------------------------------------------------------

    void test();

} // namespace strong

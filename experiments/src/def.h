#pragma once

#include <array>
#include <cassert>
#include <string>
#include <vector>

namespace def
{
    // -------------------------------------------------------------------------
    // Application Profile Values
    // -------------------------------------------------------------------------

    enum class BOOL
    {
        ValueOne,
        ValueTwo,

        Count
    };

    enum class U32
    {
        ValueOne,
        ValueTwo,

        Count
    };

    enum class STR
    {
        ValueOne,
        ValueTwo,

        Count
    };

#define PROFILE_TYPES                                                 \
    PROFILE_TYPE(BOOL, Bool, bool, static_cast<size_t>(BOOL::Count))  \
    PROFILE_TYPE(U32, U32, uint32_t, static_cast<size_t>(U32::Count)) \
    PROFILE_TYPE(STR, Str, std::string, static_cast<size_t>(STR::Count))

#include "def/include/Profile.h"

    // -------------------------------------------------------------------------
    // Application Profile
    // -------------------------------------------------------------------------

    class MyProfile : public profile::Profile
    {
    public:
        MyProfile();

        void read();
        void dump(const char* title) const;
    };

    // -------------------------------------------------------------------------
    // Application Listeners
    // -------------------------------------------------------------------------

    class Listener1 final : public profile::Profile::Listener
    {
    public:
        Listener1(profile::Profile* profile);

        void onProfile(BOOL e, const bool& value) override;
        void onProfile(U32 e, const uint32_t& value) override;
        void onProfile(STR e, const std::string& value) override;
    };

    class Listener2 final : public profile::Profile::Listener
    {
    public:
        Listener2(profile::Profile* profile);

        void onProfile(BOOL e, const bool& value) override;
        void onProfile(U32 e, const uint32_t& value) override;
        void onProfile(STR e, const std::string& value) override;
    };

    // -------------------------------------------------------------------------
    // Application Entry Point
    // -------------------------------------------------------------------------

    void test();

} // namespace def

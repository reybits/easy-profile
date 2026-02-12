/**********************************************\
*
*  Easy Profile library
*  by Andrey A. Ugolnik
*  https://github.com/reybits
*
\**********************************************/

/**********************************************\
* Usage:
```cpp
enum class U32
{
    One,
    Two,
    Count
};

enum class STR
{
    One,
    Two,
    Count
};

#define PROFILE_TYPES                                                 \
    PROFILE_TYPE(U32, U32, uint32_t, static_cast<size_t>(U32::Count)) \
    PROFILE_TYPE(STR, STR, std::string, static_cast<size_t>(STR::Count))

#include "EasyProfile.h"
```
\**********************************************/

#pragma once

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <vector>

#ifndef ASSERT
#    ifndef NDEBUG
#        include <cassert>
#        define ASSERT(x) assert(x)
#    else
#        define ASSERT(x)
#    endif
#endif

#if !defined(PROFILE_TYPES)
#    error "PROFILE_TYPES must be defined before including EasyProfile.h"
#endif

namespace easyprofile
{
    class Profile
    {
    public:
        class Listener
        {
        public:
            virtual ~Listener()
            {
                m_profile->unsubscribe(this);
            }

#define PROFILE_TYPE(Enum, Name, Type, Size)          \
    virtual void onProfile(Enum e, const Type& value) \
    {                                                 \
        (void)e;                                      \
        (void)value;                                  \
    }

            PROFILE_TYPES

#undef PROFILE_TYPE

        public:
            Profile* getProfile() const
            {
                return m_profile;
            }

            const char* getName() const
            {
                return m_name;
            }

        protected:
            Listener(Profile* profile, const char* name)
                : m_profile(profile)
                , m_name(name)
            {
                m_profile->subscribe(this);
            }

        private:
            Listener(const Listener&) = delete;
            Listener& operator=(const Listener&) = delete;

            Profile* m_profile;
            const char* m_name;
        };

    public:
        Profile(const Profile&) = delete;
        Profile& operator=(const Profile&) = delete;

        virtual ~Profile() = default;

        void init(
#define PROFILE_TYPE(Enum, Name, Type, Size) \
    const std::array<Type, Size>&def##Name,

            PROFILE_TYPES

#undef PROFILE_TYPE

            int dummy
            = 0)
        {
            (void)dummy;

#define PROFILE_TYPE(Enum, Name, Type, Size) \
    m_container##Name = def##Name;

            PROFILE_TYPES

#undef PROFILE_TYPE

            m_dirtyFlags = 0u;
        }

    public:
#define PROFILE_TYPE(Enum, Name, Type, Size)              \
    const Type& get(Enum e) const                         \
    {                                                     \
        return m_container##Name[static_cast<size_t>(e)]; \
    }

        PROFILE_TYPES

#undef PROFILE_TYPE

        // ---------------------------------------------------------------------

    public:
#define PROFILE_TYPE(Enum, Name, Type, Size)                         \
    void set(Enum e, const Type& value, bool notifyListeners = true) \
    {                                                                \
        auto& v = m_container##Name[static_cast<size_t>(e)];         \
        if (v != value)                                              \
        {                                                            \
            m_dirtyFlags |= bit(DirtyBitIndex::Name);                \
            v = value;                                               \
            if (notifyListeners)                                     \
            {                                                        \
                notify(e, value);                                    \
            }                                                        \
        }                                                            \
    }

        PROFILE_TYPES

#undef PROFILE_TYPE

        // ---------------------------------------------------------------------

    public:
#define PROFILE_TYPE(Enum, Name, Type, Size) \
    Name,

        enum class DirtyBitIndex : uint32_t
        {
            PROFILE_TYPES
        };

#undef PROFILE_TYPE

        bool isDirty(DirtyBitIndex idx) const
        {
            return (m_dirtyFlags & bit(idx)) != 0u;
        }

        bool isDirty() const
        {
            return m_dirtyFlags != 0u;
        }

        uint32_t getDirty() const
        {
            return m_dirtyFlags;
        }

        void resetDirty(DirtyBitIndex idx)
        {
            m_dirtyFlags &= ~bit(idx);
        }

        void resetDirty()
        {
            m_dirtyFlags = 0u;
        }

    private:
        constexpr uint32_t bit(DirtyBitIndex idx) const
        {
            return 1u << static_cast<uint32_t>(idx);
        }

    private:
        void subscribe(Listener* listener)
        {
#if defined(DEBUG)
            auto it = std::find(m_listeners.begin(), m_listeners.end(), listener);
            ASSERT(it == m_listeners.end());
#endif
            m_listeners.push_back(listener);
            logListenerAdded(listener, m_listeners.size());
        }

        void unsubscribe(Listener* listener)
        {
            auto it = std::find(m_listeners.begin(), m_listeners.end(), listener);
            if (it != m_listeners.end())
            {
                m_listeners.erase(it);
                logListenerRemoved(listener, m_listeners.size());
            }
        }

    protected:
        virtual void logListenerAdded(const Listener* listener, size_t totalListeners) const
        {
            // Implement logging if needed
            // ::printf("Profile Listener '%s' added, total: %zu.\n", listener->getName(), totalListeners);
            (void)listener;
            (void)totalListeners;
        }

        virtual void logListenerRemoved(const Listener* listener, size_t totalListeners) const
        {
            // Implement logging if needed
            // ::printf("Profile Listener '%s' removed, remain: %zu.\n", listener->getName(), totalListeners);
            (void)listener;
            (void)totalListeners;
        }

    private:
#define PROFILE_TYPE(Enum, Name, Type, Size)     \
    void notify(Enum e, const Type& value) const \
    {                                            \
        for (auto* l : m_listeners)              \
        {                                        \
            l->onProfile(e, value);              \
        }                                        \
    }

        PROFILE_TYPES

#undef PROFILE_TYPE

    protected:
        explicit Profile() = default;

        Profile(
#define PROFILE_TYPE(Enum, Name, Type, Size) \
    const std::array<Type, Size>&def##Name,

            PROFILE_TYPES

#undef PROFILE_TYPE

            int dummy
            = 0)
            :
#define PROFILE_TYPE(Enum, Name, Type, Size) \
    m_container##Name(def##Name),

            PROFILE_TYPES

#undef PROFILE_TYPE
            m_listeners{}
        {
            (void)dummy;
        }

    private:
        uint32_t m_dirtyFlags = 0u;

#define PROFILE_TYPE(Enum, Name, Type, Size) \
    std::array<Type, Size> m_container##Name;

        PROFILE_TYPES

#undef PROFILE_TYPE

    private:
        std::vector<Listener*> m_listeners;
    };

} // namespace easyprofile

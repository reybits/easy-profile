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

// The single source of the version. Games carry copies of this header, so a
// game that needs a feature can demand it: #if EASYPROFILE_VERSION < 10100.
#define EASYPROFILE_VERSION_MAJOR 1
#define EASYPROFILE_VERSION_MINOR 2
#define EASYPROFILE_VERSION_PATCH 0
#define EASYPROFILE_VERSION ((EASYPROFILE_VERSION_MAJOR * 10000) + (EASYPROFILE_VERSION_MINOR * 100) + (EASYPROFILE_VERSION_PATCH))

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <type_traits>
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
    template<typename T>
    using ProfileParam = std::conditional_t<std::is_scalar_v<T>, T, const T&>;

    class Profile
    {
    public:
        class Listener
        {
        public:
            // When a listener starts hearing the profile. Most do from the
            // moment they exist; an owner with nothing to update yet - a page
            // before its UI is built - subscribes later, by itself.
            enum class Subscribe
            {
                Now,
                Later,
            };

            virtual ~Listener()
            {
                m_profile->unsubscribe(this);
            }

#define PROFILE_TYPE(Enum, Name, Type, Size)                 \
    virtual void onProfile(Enum e, ProfileParam<Type> value) \
    {                                                        \
        (void)e;                                             \
        (void)value;                                         \
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
            Listener(Profile* profile, const char* name, Subscribe when = Subscribe::Now)
                : m_profile(profile)
                , m_name(name)
            {
                if (when == Subscribe::Now)
                {
                    m_profile->subscribe(this);
                }
            }

            // The destructor unsubscribes either way, so a listener that never
            // subscribed, or already unsubscribed, needs nothing more.
            void subscribe()
            {
                m_profile->subscribe(this);
            }

            void unsubscribe()
            {
                m_profile->unsubscribe(this);
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
        // Outside a batch the listeners hear a change at once, which also drops
        // the entry from a batch still being closed: a listener answering it
        // would otherwise be heard twice.
#define PROFILE_TYPE(Enum, Name, Type, Size)                                \
    void set(Enum e, ProfileParam<Type> value, bool notifyListeners = true) \
    {                                                                       \
        const auto idx = static_cast<size_t>(e);                            \
        auto& v = m_container##Name[idx];                                   \
        if (v != value)                                                     \
        {                                                                   \
            m_dirtyFlags |= bit(DirtyBitIndex::Name);                       \
            v = value;                                                      \
            if (notifyListeners)                                            \
            {                                                               \
                if (m_batchDepth != 0u)                                     \
                {                                                           \
                    m_pending##Name[idx] = true;                            \
                }                                                           \
                else                                                        \
                {                                                           \
                    m_pending##Name[idx] = false;                           \
                    notify(e, value);                                       \
                }                                                           \
            }                                                               \
        }                                                                   \
    }

        PROFILE_TYPES

#undef PROFILE_TYPE

        // Holds the notifications back: between beginBatch() and endBatch()
        // set() only remembers what changed, and the endBatch() that closes
        // the outermost batch tells the listeners, each changed entry once and
        // with its latest value. So nobody hears a profile half updated.
        void beginBatch()
        {
            m_batchDepth++;
        }

        void endBatch()
        {
            ASSERT(m_batchDepth != 0u);
            m_batchDepth--;
            if (m_batchDepth == 0u)
            {
                notifyPending();
            }
        }

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
#define PROFILE_TYPE(Enum, Name, Type, Size)            \
    void notify(Enum e, ProfileParam<Type> value) const \
    {                                                   \
        for (auto* l : m_listeners)                     \
        {                                               \
            l->onProfile(e, value);                     \
        }                                               \
    }

        PROFILE_TYPES

#undef PROFILE_TYPE

        // Each entry is cleared before it is heard: a batch that a listener
        // runs in reply hears out the rest, and must not repeat this one.
        void notifyPending()
        {
#define PROFILE_TYPE(Enum, Name, Type, Size)                    \
    for (size_t i = 0; i < Size; i++)                           \
    {                                                           \
        if (m_pending##Name[i])                                 \
        {                                                       \
            m_pending##Name[i] = false;                         \
            notify(static_cast<Enum>(i), m_container##Name[i]); \
        }                                                       \
    }

            PROFILE_TYPES

#undef PROFILE_TYPE
        }

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
        uint32_t m_batchDepth = 0u;

#define PROFILE_TYPE(Enum, Name, Type, Size) \
    std::array<Type, Size> m_container##Name;

        PROFILE_TYPES

#undef PROFILE_TYPE

        // Entries changed inside a batch and not heard yet.
#define PROFILE_TYPE(Enum, Name, Type, Size) \
    std::array<bool, Size> m_pending##Name{};

        PROFILE_TYPES

#undef PROFILE_TYPE

    private:
        std::vector<Listener*> m_listeners;
    };

} // namespace easyprofile

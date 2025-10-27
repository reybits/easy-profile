/**********************************************\
*
*  Easy Profile library
*  by Andrey A. Ugolnik
*  https://github.com/reybits
*
\**********************************************/

#pragma once

#if !defined(PROFILE_TYPES)
#    error "PROFILE_TYPES must be defined before including DefProfile.h"
#endif

namespace profile
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

#define PROFILE_TYPE(Enum, Name, Type, Size) \
    virtual void onProfile(Enum e, const Type& value) = 0;

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
            Profile* m_profile;
            const char* m_name;
        };

    public:
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
        }

    public:
        void subscribe(Listener* listener)
        {
#if defined(DEBUG)
            auto it = std::find(m_listeners.begin(), m_listeners.end(), listener);
            assert(it == m_listeners.end());
#endif
            m_listeners.push_back(listener);
            ::printf("Profile Listener '%s' added, total: %zu.\n", listener->getName(), m_listeners.size());
        }

        void unsubscribe(Listener* listener)
        {
            auto it = std::find(m_listeners.begin(), m_listeners.end(), listener);
            if (it != m_listeners.end())
            {
                m_listeners.erase(it);
                ::printf("Profile Listener '%s' removed, remain: %zu.\n", listener->getName(), m_listeners.size());
            }
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

#define PROFILE_TYPE(Enum, Name, Type, Size)                 \
    void set(Enum e, const Type& value)                      \
    {                                                        \
        auto& v = m_container##Name[static_cast<size_t>(e)]; \
        if (v != value)                                      \
        {                                                    \
            v = value;                                       \
            notify(e, value);                                \
        }                                                    \
    }

        PROFILE_TYPES

#undef PROFILE_TYPE

        // ---------------------------------------------------------------------

        void notifyAll() const
        {
#define PROFILE_TYPE(Enum, Name, Type, Size)                           \
    for (size_t i = 0, size = m_container##Name.size(); i < size; i++) \
    {                                                                  \
        notify(static_cast<Enum>(i), m_container##Name[i]);            \
    }

            PROFILE_TYPES

#undef PROFILE_TYPE
        }

    private:
#define PROFILE_TYPE(Enum, Name, Type, Size)     \
    void notify(Enum e, const Type& value) const \
    {                                            \
        for (auto l : m_listeners)               \
        {                                        \
            l->onProfile(e, value);              \
        }                                        \
    }

        PROFILE_TYPES

#undef PROFILE_TYPE

    protected:
        explicit Profile() = default;

        explicit Profile(
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
#define PROFILE_TYPE(Enum, Name, Type, Size) \
    std::array<Type, Size> m_container##Name;

        PROFILE_TYPES

#undef PROFILE_TYPE

        std::vector<Listener*> m_listeners;
    };

} // namespace profile

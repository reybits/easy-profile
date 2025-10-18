/**********************************************\
*
*  Easy Profile library
*  by Andrey A. Ugolnik
*  https://github.com/reybits
*
\**********************************************/

#pragma once

#include <any>
#include <array>
#include <cassert>
#include <print>
#include <vector>

#define ASSERT(x) assert(x)

namespace profile
{
    struct EnumType
    {
        EnumType(const char* n, const std::any& def)
            : name(n)
            , defaultValue(def)
            , value(def)
        {
        }

        const char* name;
        const std::any defaultValue;
        std::any value;
    };

    // -------------------------------------------------------------------------

    template <typename Enum, size_t N>
    class Profile;

    // -------------------------------------------------------------------------

    template <typename Enum, size_t N>
    class Listener
    {
    public:
        virtual ~Listener()
        {
            m_profile->unsubscribe(this);
        }

        virtual void onProfile(Enum e, const std::any& value) const = 0;

    public:
        using ProfileType = Profile<Enum, N>;

        ProfileType* getProfie() const
        {
            return m_profile;
        }

        const char* getName() const
        {
            return m_name;
        }

    protected:
        Listener(ProfileType* profile, const char* name)
            : m_profile(profile)
            , m_name(name)
        {
            m_profile->subscribe(this);
        }

    private:
        ProfileType* m_profile;
        const char* m_name;
    };

    // -------------------------------------------------------------------------

    template <typename Enum, size_t N>
    class Profile
    {
    public:
        using ListenerType = profile::Listener<Enum, N>;
        using Container = std::array<EnumType, N>;

        Profile(const Container& container)
            : m_container(container)
        {
        }

        virtual ~Profile() = default;

    public:
        void subscribe(ListenerType* listener)
        {
#if defined(DEBUG)
            auto it = std::find_if(m_listeners.begin(), m_listeners.end(), [listener](ListenerType* l) {
                return listener == l;
            });
            ASSERT(it == m_listeners.end());
#endif

            m_listeners.push_back(listener);
            std::println("Profile Listener '{}' added, total: {}.", listener->getName(), m_listeners.size());
        }

        void unsubscribe(ListenerType* listener)
        {
            auto it = std::find_if(m_listeners.begin(), m_listeners.end(), [listener](ListenerType* l) {
                return listener == l;
            });

            if (it != m_listeners.end())
            {
                m_listeners.erase(it);
                std::println("Profile Listener '{}' removed, remain: {}.", listener->getName(), m_listeners.size());
            }
        }

    public:
        const std::any& get(Enum e) const
        {
            return m_container[static_cast<size_t>(e)].value;
        }

        void set(Enum e, const std::any& v)
        {
            m_container[static_cast<size_t>(e)].value = v;
            notify(e, v);
        }

    protected:
        void notifyAll() const
        {
            for (size_t i = 0; i < static_cast<size_t>(Enum::Count); i++)
            {
                auto type = static_cast<Enum>(i);
                auto& value = get(type);
                notify(type, value);
            }
        }

    private:
        void notify(Enum e, const std::any& value) const
        {
            for (auto l : m_listeners)
            {
                l->onProfile(e, value);
            }
        }

    private:
        std::vector<ListenerType*> m_listeners;
        Container m_container;
    };

} // namespace profile

/**********************************************\
*
*  Easy Profile library
*  by Andrey A. Ugolnik
*  https://github.com/reybits
*
\**********************************************/

#pragma once

#include <array>
#include <tuple>

// #include <vector>

namespace profile
{
    // template <typename Enum, typename Type>
    // class Listener
    // {
    // public:
    //     virtual ~Listener()
    //     {
    //         m_profile->unsubscribe(this);
    //     }
    //
    //     virtual void onProfile(Enum e, const Type& value) = 0;
    //
    // public:
    //     using ProfileType = Profile<Containers...>;
    //
    //     Profile* getProfile() const
    //     {
    //         return m_profile;
    //     }
    //
    //     const char* getName() const
    //     {
    //         return m_name;
    //     }
    //
    // protected:
    //     Listener(ProfileType* profile, const char* name)
    //         : m_profile(profile)
    //         , m_name(name)
    //     {
    //         m_profile->subscribe(this);
    //     }
    //
    // private:
    //     ProfileType* m_profile;
    //     const char* m_name;
    // };

    // -------------------------------------------------------------------------

    template <typename Enum, typename Type, size_t Size>
    struct Container
    {
        struct Entry
        {
            Entry(const char* n, const Type& def)
                : name(n)
                , defaultValue(def)
                , value(def)
            {
            }

            const char* name;
            const Type defaultValue;
            Type value;
        };

        // using EnumType = Enum;
        // using ValueType = Type;

        void notifyAll(Enum e, const Type& value) const
        {
            // for (auto listener : listeners)
            // {
            //     listener->onProfile(e, value);
            // }
        }

        std::array<Entry, Size> data;

        // std::vector<Listener<Enum, Type>*> listeners;
    };

    // -------------------------------------------------------------------------

    template <typename... Containers>
    class Profile
    {
    public:
        template <typename Enum, typename T>
        const T& get(Enum e) const
        {
            auto& container = std::get<Container<Enum, T, static_cast<size_t>(Enum::Count)>>(m_containers);
            return container.data[static_cast<size_t>(e)];
        }

        template <typename Enum, typename T>
        void set(Enum e, const T& value)
        {
            auto& container = std::get<Container<Enum, T, static_cast<size_t>(Enum::Count)>>(m_containers);
            if (container.data[static_cast<size_t>(e)].value != value)
            {
                container.data[static_cast<size_t>(e)].value = value;
                container.notifyAll(e, value);
            }
        }

    private:
        std::tuple<Containers...> m_containers;
    };

} // namespace profile

/**********************************************\
*
*  Easy Profile library
*  by Andrey A. Ugolnik
*  https://github.com/reybits
*
\**********************************************/

#pragma once

#include <tuple>
#include <vector>

namespace profile
{
    class Storage;

    template <typename Enum, typename T, size_t N>
    struct EnumArray
    {
        using EnumType = Enum;
        using ValueType = T;

        std::array<T, N> values;

        T& operator[](Enum e)
        {
            return values[static_cast<size_t>(e)];
        }

        const T& operator[](Enum e) const
        {
            return values[static_cast<size_t>(e)];
        }
    };

    // -------------------------------------------------------------------------

    class ListenerInternal;

    class ProfileInternal
    {
    public:
        virtual ~ProfileInternal() = default;

    public:
        void subscribe(ListenerInternal* listener);
        void unsubscribe(ListenerInternal* listener);

    protected:
        ProfileInternal() = default;

    protected:
        std::vector<ListenerInternal*> m_listeners;
    };

    // -------------------------------------------------------------------------

    class ListenerInternal
    {
    public:
        virtual ~ListenerInternal()
        {
            m_profile->unsubscribe(this);
        }

    public:
        ProfileInternal* getProfie() const
        {
            return m_profile;
        }

        const char* getName() const
        {
            return m_name;
        }

    protected:
        ListenerInternal(ProfileInternal* profile, const char* name)
            : m_profile(profile)
            , m_name(name)
        {
            m_profile->subscribe(this);
        }

    private:
        ProfileInternal* m_profile;
        const char* m_name;
    };

    // -------------------------------------------------------------------------

    template <typename Derived>
    class Listener : public ListenerInternal
    {
    public:
        template <typename Enum, typename T>
        void onProfile(Enum e, const T& value)
        {
            static_cast<Derived*>(this)->notifyImpl(e, value);
        }

    protected:
        Listener(ProfileInternal* profile, const char* name)
            : ListenerInternal(profile, name)
        {
        }
    };

    // -------------------------------------------------------------------------

    template <typename... Containers>
    class Profile : public ProfileInternal
    {
    public:
        explicit Profile(Containers... cs)
            : m_containers(cs...)
        {
        }

        virtual ~Profile() = default;

    public:
        template <typename Enum>
        decltype(auto) get(Enum e) const
        {
            return std::apply(
                [&](auto&&... cs) -> decltype(auto) { return getFromTuple(e, cs...); },
                m_containers);
        }

        template <typename Enum, typename Value>
        void set(Enum e, Value&& v)
        {
            std::apply(
                [&](auto&&... cs) { (setToTuple(e, std::forward<Value>(v), cs), ...); },
                m_containers);
        }

        template <typename Enum>
        void updateContainer() const
        {
            for (size_t i = 0; i < static_cast<size_t>(Enum::Count); i++)
            {
                auto type = static_cast<Enum>(i);
                auto value = get(type);
                notify(type, value);
            }
        }

    private:
        template <typename Enum, typename T>
        void notify(Enum e, const T& value) const
        {
            for (auto l : m_listeners)
            {
                // l->onProfile(e, value);
            }
        }

        template <typename Enum, typename C, typename... Rest>
        decltype(auto) getFromTuple(Enum e, const C& c, const Rest&... rest) const
        {
            if constexpr (std::is_same_v<typename C::EnumType, Enum>)
            {
                return c[e];
            }
            else
            {
                return getFromTuple(e, rest...);
            }
        }

        template <typename Enum, typename Value, typename C>
        void setToTuple(Enum e, Value&& v, C& c)
        {
            if constexpr (std::is_same_v<typename C::EnumType, Enum>)
            {
                if (c[e] != v)
                {
                    c[e] = std::forward<Value>(v);
                    notify(e, v);
                }
            }
        }

        template <typename Enum, typename Value, typename C, typename... Rest>
        void setToTuple(Enum e, Value&& v, C& c, Rest&... rest)
        {
            if constexpr (std::is_same_v<typename C::EnumType, Enum>)
            {
                if (c[e] != v)
                {
                    c[e] = std::forward<Value>(v);
                    notify(e, v);
                }
            }
            else
            {
                setToTuple(e, std::forward<Value>(v), rest...);
            }
        }

    protected:
        Profile() = default;

    private:
        std::tuple<Containers...> m_containers;
    };

} // namespace profile

/**********************************************\
*
*  Easy Profile library
*  by Andrey A. Ugolnik
*  https://github.com/reybits
*
\**********************************************/

#pragma once

namespace profile
{
    template <typename Enum, typename T, size_t N>
    struct Container
    {
        struct Entry
        {
            Entry(const char* n, const T& def)
                : name(n)
                , defaultValue(def)
                , value(def)
            {
            }

            const char* name;
            const T defaultValue;
            T value;
        };

        using EnumType = Enum;
        using ArrayType = std::array<Entry, N>;

        Container(const ArrayType& vals)
            : values(vals)
        {
        }

        ArrayType values;

        Entry& operator[](Enum e)
        {
            return values[static_cast<size_t>(e)];
        }

        const Entry& operator[](Enum e) const
        {
            return values[static_cast<size_t>(e)];
        }
    };

    // -------------------------------------------------------------------------

    template <typename... Containers>
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

            template <typename Enum, typename T>
            void onProfile(Enum e, const T& value)
            {
                // static_cast<Derived*>(this)->onProfile(e, value);
            }

        public:
            using ProfileType = Profile<Containers...>;

            ProfileType* getProfile() const
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

    public:
        void init(const Containers... containers)
        {
            m_containers = std::make_tuple(containers...);
        }

        virtual ~Profile() = default;

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
        template <typename Enum>
        decltype(auto) get(Enum e) const
        {
            return std::apply(
                       [&](auto&&... cs) -> decltype(auto) { return getFromTuple(e, cs...); },
                       m_containers)
                .value;
        }

        // ---------------------------------------------------------------------

        template <typename Enum>
        const char* getName(Enum e) const
        {
            // return m_container[static_cast<size_t>(e)].name;
            return std::apply(
                       [&](auto&&... cs) -> decltype(auto) { return getFromTuple(e, cs...); },
                       m_containers)
                .name;
        }

        // ---------------------------------------------------------------------

        template <typename Enum, typename Value>
        void set(Enum e, const Value& v)
        {
            std::apply(
                [&](auto&&... cs) { (setToTuple(e, v, cs), ...); },
                m_containers);
        }

        // ---------------------------------------------------------------------

        template <typename Enum>
        void reset(Enum e)
        {
            auto value = std::apply(
                             [&](auto&&... cs) -> decltype(auto) { return getFromTuple(e, cs...); },
                             m_containers)
                             .defaultValue;
            set(e, value);
            notify(e, value);
        }

        // TODO: Make it non-templated and call notify for all values in all containers.
        template <typename Enum>
        void notifyAll() const
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
                l->onProfile(e, value);
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
        void setToTuple(Enum e, const Value& v, C& c)
        {
            if constexpr (std::is_same_v<typename C::EnumType, Enum>)
            {
                if (c[e].value != v)
                {
                    c[e].value = v;
                    notify(e, v);
                }
            }
        }

        template <typename Enum, typename Value, typename C, typename... Rest>
        void setToTuple(Enum e, const Value& v, C& c, Rest&... rest)
        {
            if constexpr (std::is_same_v<typename C::EnumType, Enum>)
            {
                if (c[e].value != v)
                {
                    c[e].value = v;
                    notify(e, v);
                }
            }
            else
            {
                setToTuple(e, v, rest...);
            }
        }

    protected:
        explicit Profile() = default;

        explicit Profile(Containers... cs)
            : m_containers(cs...)
        {
        }

    private:
        std::tuple<Containers...> m_containers;
        std::vector<Listener*> m_listeners;
    };

} // namespace profile

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
    template <typename... Ts>
    struct Entry
    {
        using Type = std::variant<Ts...>;

        Entry(const char* n, const Type& def)
            : name(n)
            , defaultValue(def)
            , value(def)
        {
        }

        const char* name;
        const Type defaultValue;
        Type value;

        bool operator!=(const Entry& other) const noexcept
        {
            return value != other.value;
        }
    };

    // -------------------------------------------------------------------------

    template <typename Enum, size_t N, typename... Ts>
    class Profile
    {
    public:
        using Type = std::variant<Ts...>;
        using EntryType = Entry<Ts...>;
        using Container = std::array<EntryType, N>;

        class Listener
        {
        public:
            virtual ~Listener()
            {
                m_profile->unsubscribe(this);
            }

            virtual void onProfile(Enum e, const Type& value) = 0;

        public:
            using ProfileType = Profile<Enum, N, Ts...>;

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

    public:
        virtual ~Profile() = default;

        void init(const Container& container)
        {
            m_container = container;
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
        template <typename T>
        const T& get(Enum e) const
        {
            const auto& v = m_container[static_cast<size_t>(e)].value;
            assert(std::holds_alternative<T>(v));
            return std::get<T>(v);
        }

        const Type& get(Enum e) const
        {
            return m_container[static_cast<size_t>(e)].value;
        }

        // ---------------------------------------------------------------------

        const char* getName(Enum e) const
        {
            return m_container[static_cast<size_t>(e)].name;
        }

        // ---------------------------------------------------------------------

        template <typename T>
        void set(Enum e, const T& v)
        {
            auto& entry = m_container[static_cast<size_t>(e)];
            if (!std::holds_alternative<T>(entry.value))
            {
                ::printf("Type mismatch for enum %d.\n", static_cast<int>(e));
                return;
            }

            if (std::get<T>(entry.value) != v)
            {
                entry.value = v;
                notify(e, entry.value);
            }
        }

        // ---------------------------------------------------------------------

        void reset(Enum e)
        {
            auto& entry = m_container[static_cast<size_t>(e)];
            entry.value = entry.defaultValue;
            notify(e, entry.value);
        }

        void notifyAll() const
        {
            for (size_t i = 0; i < N; ++i)
            {
                notify(static_cast<Enum>(i), m_container[i].value);
            }
        }

    private:
        void notify(Enum e, const Type& value) const
        {
            for (auto l : m_listeners)
            {
                l->onProfile(e, value);
            }
        }

    protected:
        explicit Profile() = default;

        explicit Profile(const Container& container)
            : m_container(container)
        {
        }

    private:
        Container m_container;
        std::vector<Listener*> m_listeners;
    };

} // namespace profile

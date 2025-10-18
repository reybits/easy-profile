/**********************************************\
*
*  Easy Profile library
*  by Andrey A. Ugolnik
*  https://github.com/reybits
*
\**********************************************/

#include "../include/Profile.h"

#include <algorithm>
#include <cassert>
#include <print>

#define ASSERT(x) assert(x)

namespace profile_strong
{
    void ProfileInternal::subscribe(ListenerInternal* listener)
    {
#if defined(DEBUG)
        auto it = std::find_if(m_listeners.begin(), m_listeners.end(), [listener](ListenerInternal* l) {
            return listener == l;
        });
        ASSERT(it == m_listeners.end());
#endif

        m_listeners.push_back(listener);
        std::println("Profile Listener '{}' added, total: {}.", listener->getName(), m_listeners.size());
    }

    void ProfileInternal::unsubscribe(ListenerInternal* listener)
    {
        auto it = std::find_if(m_listeners.begin(), m_listeners.end(), [listener](ListenerInternal* l) {
            return listener == l;
        });

        if (it != m_listeners.end())
        {
            m_listeners.erase(it);
            std::println("Profile Listener '{}' removed, remain: {}.", listener->getName(), m_listeners.size());
        }
    }

} // namespace profile_strong

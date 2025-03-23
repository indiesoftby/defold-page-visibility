#ifndef PAGE_VISIBILITY_EXTENSION_HPP
#define PAGE_VISIBILITY_EXTENSION_HPP

#include <dmsdk/sdk.h> // Lua headers
#include <dmsdk/dlib/hashtable.h>

namespace PageVisibilityExt
{
    enum PageVisibilityState
    {
        STATE_UNKNOWN = -1,
        STATE_VISIBLE = 1,
        STATE_HIDDEN  = 2,
    };

    struct SetupLuaCallback
    {
        dmScript::LuaCallbackInfo* m_Callback;
        bool m_Once;

        SetupLuaCallback(dmScript::LuaCallbackInfo* callback, bool once)
        {
            if (!dmScript::IsCallbackValid(callback))
                return;

            if (!dmScript::SetupCallback(callback))
            {
                dmLogError("Failed to setup callback (has the calling script been destroyed?)");
                dmScript::DestroyCallback(callback);
                return;
            }

            m_Callback = callback;
            m_Once     = once;
        }

        bool Valid()
        {
            return m_Callback != NULL;
        }

        lua_State* GetLuaContext()
        {
            return dmScript::GetCallbackLuaContext(m_Callback);
        }

        void Call(lua_State* L, int nargs)
        {
            if (m_Callback)
            {
                dmScript::PCall(L, nargs + 1, 0);
            }
            else
            {
                lua_pop(L, nargs);
            }
        }

        ~SetupLuaCallback()
        {
            if (m_Callback)
            {
                dmScript::TeardownCallback(m_Callback);
                if (m_Once)
                {
                    dmScript::DestroyCallback(m_Callback);
                }
            }
        }
    };
} // namespace PageVisibilityExt

#endif // PAGE_VISIBILITY_EXTENSION_HPP

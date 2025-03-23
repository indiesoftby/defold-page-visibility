
#include "ext.hpp"

#if defined(DM_PLATFORM_HTML5)

extern "C"
{
    void PageVisibilityJS_SetListener(void (*callback)());
    int PageVisibilityJS_GetState();
}

/**
 * Helpers
 */

static int LuaError(lua_State* L, const char* error)
{
    lua_pushstring(L, error);
    lua_error(L);
    return 0;
}

/**
 * Functions
 */

static dmScript::LuaCallbackInfo* g_PageVisibilityUserCallback = 0;

static void PageVisibilityChangeListener()
{
    if (!g_PageVisibilityUserCallback)
    {
        return;
    }

    PageVisibilityExt::SetupLuaCallback lc(g_PageVisibilityUserCallback, false);
    if (!lc.Valid())
    {
        return;
    }

    lua_State* L = lc.GetLuaContext();
    lc.Call(L, 0);
}

static int PageVisibility_SetListener(lua_State* L)
{
    PageVisibilityJS_SetListener(PageVisibilityChangeListener);

    dmScript::LuaCallbackInfo* luaCallback = 0;
    if (lua_isfunction(L, 1))
    {
        luaCallback = dmScript::CreateCallback(L, 1);
    }

    if (g_PageVisibilityUserCallback)
    {
        dmScript::DestroyCallback(g_PageVisibilityUserCallback);
    }
    g_PageVisibilityUserCallback = luaCallback;

    return 0;
}

static int PageVisibility_State(lua_State* L)
{
    lua_pushnumber(L, PageVisibilityJS_GetState());
    return 1;
}

// Functions exposed to Lua
static const luaL_reg Ext_methods[] = {
    { "set_listener", PageVisibility_SetListener },
    // Properties
    { "state", PageVisibility_State },

    /* Sentinel: */
    { NULL, NULL }
};

static void LuaInit(lua_State* L)
{
    int top = lua_gettop(L);

    // Register lua names
    luaL_register(L, "page_visibility", Ext_methods);

#define SETCONSTANT(name, val) \
    lua_pushnumber(L, (lua_Number)val); \
    lua_setfield(L, -2, #name);

    SETCONSTANT(STATE_VISIBLE, PageVisibilityExt::STATE_VISIBLE);
    SETCONSTANT(STATE_HIDDEN, PageVisibilityExt::STATE_HIDDEN);

    lua_pop(L, 1);
    assert(top == lua_gettop(L));
}

#endif

static dmExtension::Result InitializeExt(dmExtension::Params* params)
{
#if defined(DM_PLATFORM_HTML5)
    LuaInit(params->m_L);
#endif

    return dmExtension::RESULT_OK;
}

static dmExtension::Result AppInitializeExt(dmExtension::AppParams* params)
{
    return dmExtension::RESULT_OK;
}

static dmExtension::Result FinalizeExt(dmExtension::Params* params)
{
#if defined(DM_PLATFORM_HTML5)
    if (g_PageVisibilityUserCallback)
    {
        dmScript::DestroyCallback(g_PageVisibilityUserCallback);
        g_PageVisibilityUserCallback = 0;
    }
#endif
    return dmExtension::RESULT_OK;
}

static dmExtension::Result AppFinalizeExt(dmExtension::AppParams* params)
{
    return dmExtension::RESULT_OK;
}

DM_DECLARE_EXTENSION(page_visibility, "page_visibility", AppInitializeExt, AppFinalizeExt, InitializeExt, 0, 0, FinalizeExt)

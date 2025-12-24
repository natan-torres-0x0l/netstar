#include <console/pty/pty.h>

#include <lua5.4/lauxlib.h>
#include <lua5.4/lualib.h>

#include <lua5.4/lua.h>


static char **
console_pty_table_in_commands(lua_State *L, int arg, size_t *commands_count) {
  char **commands = NULL;
  size_t index = 0, count = 0;

  if (!lua_istable(L, 1) || !(*commands_count = lua_rawlen(L, 1)))
    luaL_typeerror(L, arg, "table");

  if (!(commands = (char **)calloc((*commands_count)+1, sizeof(char *))))
    luaL_error(L, "");

  for (index = 0; index < *commands_count; index++) {
    lua_rawgeti(L, 1, index+1);
    if (lua_isstring(L, -1)) {
      const char *string = lua_tostring(L, -1);
      commands[count++] = strdup(string);
    }

    lua_pop(L, 1);
  }

  *commands_count = count;

  return commands;
}

static int
console_pty_method_spawn(lua_State *L) {
  size_t command_count = 0;
  char **command = console_pty_table_in_commands(L, -1, &command_count);

  console_pty_t *pty = NULL;

  if (console_pty_spawn(pty, command) == -1)
    return luaL_error(L, "");

  pty = (console_pty_t *)lua_newuserdata(L, sizeof(console_pty_t));;

  luaL_getmetatable(L, "pty");
  lua_setmetatable(L, -2);

  return 1;
}

static int
console_pty_method_write(lua_State *L) {
  console_pty_t *pty = (console_pty_t *)luaL_checkudata(L, 1, "pty");
  size_t buffer_length = 0;
  const char *buffer = luaL_checklstring(L, 2, &buffer_length);
  ssize_t bytes = -1;

  byte = console_pty_write(pty, buffer, buffer_length);

  lua_pushnumber(L, bytes);
  return 1;
}

static int
console_pty_method_read(lua_State *L) {
  console_pty_t *pty = (console_pty_t *)luaL_checkudata(L, 1, "pty");
  char buffer[1024] = {0};
  ssize_t bytes = -1;

  if ((bytes = console_pty_read(pty, buffer, sizeof(buffer)))) {
    lua_pushnil(L);
    return 1;
  }

  lua_pushlstring(L, buffer, bytes);
  return 1;
}

static int
console_pty_method_close(lua_State *L) {
  console_pty_t *pty = (console_pty_t *)luaL_checkudata(L, 1, "pty");

  console_pty_close(pty);

  lua_pushnumber(L, 1);
  return 1;
}

static inline int
console_pty_login_io_function(lua_State *L, int index) {
  luaL_checktype(L, index, LUA_TFUNCTION);
  lua_pushvalue(L, index);
  return luaL_ref(L, LUA_REGISTRYINDEX);
}

int
console_pty_method_login(lua_State *L) {
  console_pty_t *pty = (console_pty_t *)luaL_checkudata(L, 1, "pty");
  int pty_output = console_pty_login_io_function(L, 2);
  int pty_input = console_pty_login_io_function(L, 3);

  for (;;) {
    switch (console_pty_pipeline(pty, NULL)) {
      case CONSOLE_PTY_MASTER: {
        char buffer[1024] = {0};
        ssize_t bytes = -1;

        if ((bytes = console_pty_read(pty, buffer, sizeof(buffer))) <= 0)
          goto _return;

        lua_rawgeti(L, LUA_REGISTRYINDEX, pty_output);
        lua_pushlstring(L, buffer, bytes);
        if (lua_pcall(L, 1, 0, 0) != LUA_OK) {
          lua_pop(L, 1);
          continue;
        }

        break;
      }

      case CONSOLE_PTY_STDIN: {
        const char *buffer = NULL;
        size_t length = 0;

      	lua_rawgeti(L, LUA_REGISTRYINDEX, pty_input);
        if (lua_pcall(L, 0, 1, 0) != LUA_OK) {
          lua_pop(L, 1);
          continue;
        }

        if (!(buffer = lua_tolstring(L, -1, &length)))
          continue;
        if (console_pty_write(pty, buffer, length) <= 0)
          goto _return;

        break;
      }

      default:
        lua_pushboolean(L, false);
        return 1;
    }
  }

_return:
  lua_pushboolean(L, true);
  return 1;
}

int
luaopen_libconsole_pty(lua_State *L) {
  if (luaL_newmetatable(L, "pty")) {
    lua_newtable(L);

    lua_pushcfunction(L, console_pty_method_login);
    lua_setfield(L, -2, "login");

    lua_pushcfunction(L, console_pty_method_write);
    lua_setfield(L, -2, "write");

    lua_pushcfunction(L, console_pty_method_read);
    lua_setfield(L, -2, "read");

    lua_pushcfunction(L, console_pty_method_close);
    lua_setfield(L, -2, "close");

    lua_setfield(L, -2, "__index");
  }
  lua_pop(L, 1);

  lua_newtable(L);
  lua_pushcfunction(L, console_pty_method_spawn);
  lua_setfield(L, -2, "spawn");

  return 1;
}

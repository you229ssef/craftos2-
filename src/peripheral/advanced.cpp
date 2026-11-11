/*
 * peripheral/advanced.cpp
 * CraftOS-PC 2
 *
 * Virtual compatibility peripherals for Advanced Peripherals.
 */

#include "advanced.hpp"
#include "../util.hpp"
#include <algorithm>
#include <chrono>
#include <cstring>

namespace {
luaL_Reg chat_box_methods[] = {
    {"sendMessage", nullptr}, {"sendFormattedMessage", nullptr}, {"getPlayers", nullptr}, {nullptr, nullptr}
};
luaL_Reg player_detector_methods[] = {
    {"getPlayers", nullptr}, {"getOnlinePlayers", nullptr}, {"getPlayer", nullptr},
    {"getPlayerPos", nullptr}, {"isPlayerInArea", nullptr}, {nullptr, nullptr}
};
luaL_Reg environment_detector_methods[] = {
    {"getDimension", nullptr}, {"getBiome", nullptr}, {"getTime", nullptr},
    {"getMoonPhase", nullptr}, {"isRaining", nullptr}, {"isThundering", nullptr}, {nullptr, nullptr}
};
luaL_Reg geo_scanner_methods[] = {
    {"scan", nullptr}, {"locate", nullptr}, {"isBlock", nullptr}, {nullptr, nullptr}
};
luaL_Reg block_reader_methods[] = {
    {"getBlockName", nullptr}, {"getBlockData", nullptr}, {"getBlockTags", nullptr}, {nullptr, nullptr}
};
luaL_Reg inventory_manager_methods[] = {
    {"getInventory", nullptr}, {"getItem", nullptr}, {"pushItems", nullptr}, {"pullItems", nullptr}, {nullptr, nullptr}
};
luaL_Reg redstone_integrator_methods[] = {
    {"getInput", nullptr}, {"getOutput", nullptr}, {"setOutput", nullptr},
    {"getAnalogInput", nullptr}, {"getAnalogOutput", nullptr}, {"setAnalogOutput", nullptr}, {nullptr, nullptr}
};
luaL_Reg bridge_methods[] = {
    {"listAllItems", nullptr}, {"getItem", nullptr}, {"getEnergyStorage", nullptr},
    {"getMaxEnergyStorage", nullptr}, {"getCraftingCPUs", nullptr}, {nullptr, nullptr}
};
}

advanced_peripheral::advanced_peripheral(lua_State *L, const char *side, const char *type):
    type(type), side(side), comp(get_comp(L)) {}

peripheral *advanced_peripheral::init(lua_State *L, const char *side, const char *type) {
    return new advanced_peripheral(L, side, type);
}

int advanced_peripheral::sideIndex(const char *value) {
    static const char *sides[] = {"bottom", "top", "back", "front", "right", "left"};
    for (int i = 0; i < 6; ++i) if (strcmp(value, sides[i]) == 0) return i;
    return -1;
}

void advanced_peripheral::emptyTable(lua_State *L) {
    lua_newtable(L);
}

void advanced_peripheral::emptyArray(lua_State *L) {
    lua_createtable(L, 0, 0);
}

int advanced_peripheral::chatBox(lua_State *L, const std::string& method) {
    if (method == "sendMessage" || method == "sendFormattedMessage") {
        luaL_checkany(L, 1);
        lua_pushboolean(L, true);
        return 1;
    }
    emptyArray(L);
    return 1;
}

int advanced_peripheral::playerDetector(lua_State *L, const std::string& method) {
    if (method == "getPlayer") {
        lua_pushnil(L);
        return 1;
    }
    if (method == "isPlayerInArea") {
        lua_pushboolean(L, false);
        return 1;
    }
    if (method == "getPlayerPos") {
        lua_pushnil(L);
        return 1;
    }
    emptyArray(L);
    return 1;
}

int advanced_peripheral::environmentDetector(lua_State *L, const std::string& method) {
    if (method == "getDimension") {
        lua_pushliteral(L, "minecraft:overworld");
        return 1;
    }
    if (method == "getBiome") {
        lua_pushliteral(L, "minecraft:plains");
        return 1;
    }
    if (method == "getTime") {
        const auto now = std::chrono::system_clock::now().time_since_epoch();
        const auto ticks = std::chrono::duration_cast<std::chrono::milliseconds>(now).count() / 50;
        lua_pushinteger(L, ticks % 24000);
        return 1;
    }
    if (method == "getMoonPhase") {
        lua_pushinteger(L, 0);
        return 1;
    }
    lua_pushboolean(L, false);
    return 1;
}

int advanced_peripheral::geoScanner(lua_State *L, const std::string& method) {
    if (method == "isBlock") {
        lua_pushboolean(L, false);
        return 1;
    }
    if (method == "locate") {
        lua_pushnil(L);
        return 1;
    }
    emptyArray(L);
    return 1;
}

int advanced_peripheral::blockReader(lua_State *L, const std::string& method) {
    if (method == "getBlockName") {
        lua_pushliteral(L, "minecraft:air");
        return 1;
    }
    if (method == "getBlockData") {
        emptyTable(L);
        return 1;
    }
    emptyArray(L);
    return 1;
}

int advanced_peripheral::inventoryManager(lua_State *L, const std::string& method) {
    if (method == "getItem") {
        lua_pushnil(L);
        return 1;
    }
    if (method == "pushItems" || method == "pullItems") {
        lua_pushinteger(L, 0);
        return 1;
    }
    emptyArray(L);
    return 1;
}

int advanced_peripheral::redstoneIntegrator(lua_State *L, const std::string& method) {
    if (method == "setOutput" || method == "setAnalogOutput") {
        luaL_checkstring(L, 1);
        luaL_checkinteger(L, 2);
        return 0;
    }
    if (method == "getInput" || method == "getOutput") {
        const int index = sideIndex(luaL_checkstring(L, 1));
        lua_pushboolean(L, index >= 0 && (comp->redstoneInputs[index] || comp->redstoneOutputs[index]));
        return 1;
    }
    if (method == "getAnalogInput" || method == "getAnalogOutput") {
        const int index = sideIndex(luaL_checkstring(L, 1));
        lua_pushinteger(L, index >= 0 ? (method == "getAnalogInput" ? comp->redstoneInputs[index] : comp->redstoneOutputs[index]) : 0);
        return 1;
    }
    return luaL_error(L, "No such method");
}

int advanced_peripheral::bridge(lua_State *L, const std::string& method) {
    if (method == "getEnergyStorage" || method == "getMaxEnergyStorage") {
        lua_pushinteger(L, 0);
        return 1;
    }
    if (method == "getItem") {
        lua_pushnil(L);
        return 1;
    }
    emptyArray(L);
    return 1;
}

library_t advanced_peripheral::getMethods() const {
    if (type == "chat_box") return {"chat_box", chat_box_methods, nullptr, nullptr};
    if (type == "player_detector") return {"player_detector", player_detector_methods, nullptr, nullptr};
    if (type == "environment_detector") return {"environment_detector", environment_detector_methods, nullptr, nullptr};
    if (type == "geo_scanner") return {"geo_scanner", geo_scanner_methods, nullptr, nullptr};
    if (type == "block_reader") return {"block_reader", block_reader_methods, nullptr, nullptr};
    if (type == "inventory_manager") return {"inventory_manager", inventory_manager_methods, nullptr, nullptr};
    if (type == "redstone_integrator") return {"redstone_integrator", redstone_integrator_methods, nullptr, nullptr};
    return {type.c_str(), bridge_methods, nullptr, nullptr};
}

int advanced_peripheral::call(lua_State *L, const char *method) {
    lastCFunction = __func__;
    const std::string name(method);
    if (type == "chat_box") return chatBox(L, name);
    if (type == "player_detector") return playerDetector(L, name);
    if (type == "environment_detector") return environmentDetector(L, name);
    if (type == "geo_scanner") return geoScanner(L, name);
    if (type == "block_reader") return blockReader(L, name);
    if (type == "inventory_manager") return inventoryManager(L, name);
    if (type == "redstone_integrator") return redstoneIntegrator(L, name);
    if (type == "rs_bridge" || type == "me_bridge") return bridge(L, name);
    return luaL_error(L, "No such method");
}

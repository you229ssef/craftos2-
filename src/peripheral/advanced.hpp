/*
 * peripheral/advanced.hpp
 * CraftOS-PC 2
 *
 * Virtual compatibility peripherals for Advanced Peripherals.
 */

#ifndef PERIPHERAL_ADVANCED_HPP
#define PERIPHERAL_ADVANCED_HPP

#include <peripheral.hpp>
#include <string>

struct Computer;

class advanced_peripheral: public peripheral {
    std::string type;
    std::string side;
    Computer *comp;

    int chatBox(lua_State *L, const std::string& method);
    int playerDetector(lua_State *L, const std::string& method);
    int environmentDetector(lua_State *L, const std::string& method);
    int geoScanner(lua_State *L, const std::string& method);
    int blockReader(lua_State *L, const std::string& method);
    int inventoryManager(lua_State *L, const std::string& method);
    int redstoneIntegrator(lua_State *L, const std::string& method);
    int bridge(lua_State *L, const std::string& method);

    static int sideIndex(const char *side);
    static void emptyTable(lua_State *L);
    static void emptyArray(lua_State *L);

public:
    advanced_peripheral(lua_State *L, const char *side, const char *type);
    ~advanced_peripheral() override = default;

    static peripheral *init(lua_State *L, const char *side, const char *type);
    static void deinit(peripheral *p) { delete (advanced_peripheral *)p; }

    destructor getDestructor() const override { return deinit; }
    library_t getMethods() const override;
    int call(lua_State *L, const char *method) override;
};

#endif

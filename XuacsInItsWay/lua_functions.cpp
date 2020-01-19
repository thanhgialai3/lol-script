#include "lua_functions.h"

#include "Orbwalker.h"



static class Engine {

};


void DefineClasses(lua_State* L)
{
	using namespace std;
	//&std::vector< CObject >::at


	// CObject
	luabridge::getGlobalNamespace(L)
		.beginClass <CObject>("CObject")
		.addConstructor <void(*) (void)>()
		.addProperty<float>("health", &CObject::health, true)
		.addProperty<float>("max_health", &CObject::max_health, true)
		.addProperty<D3DXVECTOR3>("position", &CObject::position, true)
		.addProperty<D3DXVECTOR3>("start_pos", &CObject::skill_start_pos, true)
		.addProperty<D3DXVECTOR3>("end_pos", &CObject::skill_end_pos, true)
		.addProperty<D3DXVECTOR3>("bounding_box_first_point", &CObject::bounding_box_first_point, true)
		.addProperty<D3DXVECTOR3>("bounding_box_second_point", &CObject::bounding_box_second_point, true)
		.addProperty<float>("missileLineWidth", &CObject::missileLineWidth, true)
		//.addProperty<float>("y", &CObject::positiony, true)
		//.addProperty<float>("z", &CObject::positionz, true)
		.addProperty<int>("network_id", &CObject::network_id, true)
		.addProperty<int>("team", &CObject::team, true)
		.addProperty<__int16>("source_index", &CObject::source_index, true)
		.addProperty<__int16>("target_id", &CObject::target_id, true)
		.addProperty<__int32>("visible", &CObject::visible, true)
		.addProperty<float>("mana", &CObject::mana, true)
		.addProperty<float>("max_mana", &CObject::max_mana, true)
		.addProperty<float>("bonus_attack", &CObject::bonus_attack, true)
		.addProperty<float>("bonus_ability_attack", &CObject::bonus_ability_attack, true)
		.addProperty<float>("bonus_magic_resist", &CObject::bonus_magic_resist, true)
		.addProperty<float>("attack_speed_mod", &CObject::attack_speed_mod, true)
		.addProperty<float>("attack_range", &CObject::attack_range, true)
		.addProperty<float>("base_attack", &CObject::base_attack, true)
		.addProperty<float>("crit_mod", &CObject::crit_mod, true)
		.addProperty<float>("armor", &CObject::armor, true)
		.addProperty<float>("magic_resist", &CObject::magic_resist, true)
		.addProperty<float>("move_speed", &CObject::move_speed, true)
		.addProperty<int>("base_address", &CObject::basePosition, true)
		.addProperty<float>("predicted_health", &CObject::predicted_health, true)
		.addProperty<int>("predicted_after_time", &CObject::predicted_after_time, true)
		.addProperty<int>("focus_count", &CObject::focus_count, true)
		//		.addProperty<__int32>("level", &CObject::level, true)
		.addFunction("IsHero", &CObject::isHero)
		.addFunction("IsMinion", &CObject::isMinion)
		.addFunction("IsTurret", &CObject::isTurret)
		.addFunction("IsMissile", &CObject::isMissile)
		.addFunction("GetName", &CObject::GetName)
		.addFunction("GetHeroName", &CObject::GetHeroName)
		.addFunction("IsTargetable", &CObject::isTargetable)
		.addFunction("IsDead", &CObject::isDead)
		.addFunction("IsValid", &CObject::isValid)
		.addFunction("GetAttackDelay", &CObject::GetAttackDelay)
		.addFunction("GetAttackCastDelay", &CObject::GetAttackCastDelay)
		.addFunction("IsAttacking", &CObject::IsAttacking)
		.addFunction("IsVisible", &CObject::isVisible)
		.addFunction("IsColliding", &CObject::IsColliding)
		.endClass()
		.beginClass< std::vector<CObject> >("CObjectList")
		.addConstructor<void(*) (void)>()
		.addFunction("push", static_cast<void (std::vector<CObject>::*) (const CObject&)>(&std::vector<CObject>::push_back))
		.addFunction("at", static_cast<vector< CObject >::reference(vector< CObject >::*)(vector< CObject >::size_type) >(&std::vector< CObject >::at))
		.addFunction("size", &std::vector< CObject >::size)
		.addFunction("clear", &std::vector< CObject >::clear)
		.endClass();



	////

	luabridge::getGlobalNamespace(L)
		.beginClass <CActiveSkillClass>("CActiveSkill")
		.addConstructor <void(*) (void)>()
		.addProperty<D3DXVECTOR3>("start_pos", &CActiveSkillClass::start_pos, true)
		.addProperty<D3DXVECTOR3>("end_pos", &CActiveSkillClass::end_pos, true)
		.addProperty<float>("missileLineWidth", &CActiveSkillClass::line_width, true)
		.addProperty<CObject>("caster", &CActiveSkillClass::caster, true)
		.addProperty<int>("base_address", &CActiveSkillClass::base_address, true)
		.addProperty<int>("spell_index", &CActiveSkillClass::spell_index, true)
		.addProperty<float>("time_casted", &CActiveSkillClass::time_casted, true)
		.addFunction("GetSpellCastRange", &CActiveSkillClass::GetSpellCastRange)
		.addFunction("GetSpellName", &CActiveSkillClass::GetSpellName)
		.addFunction("GatherInfo", &CActiveSkillClass::GatherInfo)
		.endClass()
		.beginClass<vector< CActiveSkillClass >::const_iterator>("const_iterator")
		.addConstructor<void(*) (void)>()
		.endClass()
		.beginClass<vector< CActiveSkillClass >::iterator>("iterator")
		.addConstructor<void(*) (void)>()
		.endClass()
		.beginClass< std::vector<CActiveSkillClass> >("CActiveSkillList")
		.addConstructor<void(*) (void)>()
		.addFunction("push", static_cast<void (std::vector<CActiveSkillClass>::*) (const CActiveSkillClass&)>(&std::vector<CActiveSkillClass>::push_back))
		.addFunction("at", static_cast<vector< CActiveSkillClass >::reference(vector< CActiveSkillClass >::*)(vector< CActiveSkillClass >::size_type) >(&std::vector< CActiveSkillClass >::at))
		.addFunction("size", &std::vector< CActiveSkillClass >::size)
		.addFunction("clear", &std::vector< CActiveSkillClass >::clear)
		.addFunction("beginv", static_cast<vector< CActiveSkillClass >::const_iterator(vector< CActiveSkillClass >::*)() const >(&std::vector< CActiveSkillClass >::begin))
		.addFunction("endv", static_cast<vector< CActiveSkillClass >::const_iterator(vector< CActiveSkillClass >::*)() const >(&std::vector< CActiveSkillClass >::end))
		.addFunction("erasev", static_cast<vector< CActiveSkillClass >::iterator(vector< CActiveSkillClass >::*)(vector< CActiveSkillClass >::const_iterator) >(&std::vector< CActiveSkillClass >::erase))
		.endClass();

	////


	//typedef pair<const int, int> imtesting_type;

	//imtesting_type type;

	using _Pairib = pair<std::map<int, int>::iterator, bool>;

	//Management::Priority_List


	luabridge::getGlobalNamespace(L)
		.beginClass <std::map<int, int>::iterator>("mapiterator")
		.addConstructor<void(*) (void)>()
		.endClass();

	luabridge::getGlobalNamespace(L)
		.beginClass <_Pairib>("mappair")
		.addConstructor<void(*) (void)>()
		.endClass();

	luabridge::getGlobalNamespace(L)
		.beginClass <std::map<int,int>>("mapintint")
		.addConstructor<std::map<int,int>::reference(*) (void)>()
		.addFunction("at", static_cast<map< int, int >::mapped_type&(map< int,int >::*)(const map< int,int >::key_type&) >(&std::map< int,int >::at))
		//.addFunction("emplace", static_cast<_Pairib(map< int,int >::*)( std::map<int,int>::value_type&& val) >(&std::map< int,int >::emplace))
		.addFunction("endv", static_cast<std::map<int,int>::iterator(map< int,int >::*)() >(&std::map< int,int >::end))
		.addFunction("find", static_cast<std::map<int,int>::iterator(map< int,int >::*)(const std::map<int,int>::key_type& k) >(&std::map< int,int >::find))
		.endClass();

	luabridge::getGlobalNamespace(L)
		.beginClass <D3DXVECTOR3>("Vec3")
		.addConstructor <void(*) (float, float, float)>()
		.addProperty<float>("x", &D3DXVECTOR3::x)
		.addProperty<float>("y", &D3DXVECTOR3::y)
		.addProperty<float>("z", &D3DXVECTOR3::z)
		.addFunction("normalize", &D3DXVECTOR3::Normalized)
		.addFunction("perpendicularTo", &D3DXVECTOR3::perpendicularTo)
		.endClass();

	luabridge::getGlobalNamespace(L)
		.beginClass <D3DXVECTOR2>("Vec2")
		.addConstructor <void(*) (float, float)>()
		.addProperty<float>("x", &D3DXVECTOR2::x)
		.addProperty<float>("y", &D3DXVECTOR2::y)
		.endClass();


	luabridge::getGlobalNamespace(L)
		.addFunction("W2S", WorldToScreen)
		.addFunction("DrawLine", DrawLine)
		.addFunction("DrawWhiteLine", DrawWhiteLine)
		.addFunction("DrawRedLine", DrawRedLine)
		.addFunction("DrawCircle", DrawCircle)
		.addFunction("ReadFloat", ReadFloat)
		.addFunction("ReadInt", ReadInt)
		.addFunction("ReadShort", ReadShort)
		.addFunction("ReadString", ReadString)
		.addFunction("GetObjectByIndex", GetObjectByIndex)
		.addFunction("GetPing", GetPing);

	luabridge::getGlobalNamespace(L)
		.addFunction("ScriptClick", Mouse::ScriptClick)
		.addFunction("RightClick", Mouse::RightClick)
		.addFunction("LeftClick", Mouse::LeftClick)
		.addFunction("KeyboardClick", Mouse::KeyboardClick)
		.addFunction("Move", Mouse::Move)
		.addFunction("MoveTo", Mouse::MoveTo)
		.addFunction("IsKeyPressed", Mouse::IsKeyPressed);


	




	//luabridge::getGlobalNamespace(L)
	//	.beginClass <Overlay>("Overlay")
	//	.addStaticFunction("DrawLine", &Overlay::DrawLine)
	//	.addStaticFunction("DrawCircle", &Overlay::DrawCircle)
	//	.addStaticFunction("DrawString", &Overlay::DrawString)
	//	.endClass();

	// CObject
}



void DefineFunctions(lua_State* L)
{
	lua_register(L, "log", log_print);
	lua_register(L, "sleep", sleep_func);
	lua_register(L, "read_entities", read_entities_func);
	lua_register(L, "read_missiles", read_missiles_func);
	lua_register(L, "read_heroes", read_heroes_func);
	//lua_register(L, "script_click", script_click);
	//lua_register(L, "right_click", right_click);
	//lua_register(L, "left_click", left_click);
	//lua_register(L, "keyboard_click", keyboard_click);
	//lua_register(L, "move", move_click);
	lua_register(L, "now", time_now);
	//lua_register(L, "IsKeyPressed", is_key_pressed);
	lua_register(L, "read_my_hero", read_local_player);
	//lua_register(L, "W2S", WorldToScreenLUA);
}

void DefineGlobals(lua_State* L)
{
	Management::Priority_List[0] = 999;

	std::map<int, int> temp_map = Management::Priority_List;
	for (int i = 0; i < LuaKeeper::lua_list.size(); i++)
	{
		luabridge::push(LuaKeeper::lua_list[i], temp_map);
		lua_setglobal(LuaKeeper::lua_list[i], "priority_list");
	}

	luabridge::push(L, Orbwalker::adjust_for_attack_cast_delay);
	lua_setglobal(L, "attack_cast_delay_multiplier");

	luabridge::push(L, Orbwalker::adjust_for_attack_delay);
	lua_setglobal(L, "attack_delay_multiplier");

	luabridge::push(L, Management::percent_health);
	lua_setglobal(L, "attack_under_hp_percent");


	CRenderer renderer = Management::Manager.Read<CRenderer>(Management::Manager.Read<DWORD>(Management::LolClient.dwBase + offsets::m_RendererClass));

	luabridge::push(L, renderer.ScreenResolutionX);
	lua_setglobal(L, "screen_width");

	luabridge::push(L, renderer.ScreenResolutionY);
	lua_setglobal(L, "screen_height");
}


D3DXVECTOR3 * checkVec3(lua_State * L, int n)
{
	// This checks that the argument is a userdata 
	// with the metatable "luaL_Foo"
	return *reinterpret_cast<D3DXVECTOR3**>(luaL_checkudata(L, n, "Vec3"));
}

D3DXVECTOR2* checkVec2(lua_State * L, int n)
{
	return *reinterpret_cast<D3DXVECTOR2**>(luaL_checkudata(L, n, "Vec2"));
}
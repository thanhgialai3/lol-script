
#ifndef GLOBALS_H
#define GLOBALS_H

#include "MMemory.h"
#include <d3dx9.h>
#include <vector>
#include <map>
#include <mutex>

#include <iostream>

extern "C" {
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
}

#include <LuaBridge/LuaBridge.h>

//////


//////


static class LuaKeeper {
public:
	static std::vector<lua_State*> lua_list;
	static std::mutex mtx;           // mutex for critical section
};





#define XOR_KEY 0x342626A
//(unsigned int)(Management::LolClient.dwBase + 0x327626A)



class CObject;

static class Settings {
public:
	static bool drawings_enabled;
	static bool hero_line_draw;

};


static class Management {
public:
	static Memory Manager;
	static Module LolClient;
	static std::vector<CObject> Player_List;
	static std::map<int, int> Priority_List;
	static float percent_health;
	static CObject current_target;
	static CObject local_player;
	static CObject under_mouse_object;
	static bool wait_for_usage;
};



 template<typename T, typename U> constexpr size_t offsetOf(U T::*member)
{
	return (char*)&((T*)nullptr->*member) - (char*)nullptr;
}

static class offsets {
public:
	 static DWORD m_LocalPlayer; // 3A1AFA4 0x2F709C0 A1 ? ? ? ? 85 C0 0F 84 ? ? ? ? 56 8B B0

	 static DWORD m_UnderMouseObj; // C7 05 ? ? ? ? ? ? ? ? E8 ? ? ? ? 83 C4 04 FF B4

	 static DWORD m_ObjectList; // B9 ? ? ? ? E8 ? ? ? ? 8B C8 8B 10 8B 82

	 static DWORD m_RendererClass; // 8B 15 ? ? ? ? 83 EC 08 F3

	 static DWORD m_PingContainer; // 8B 0D ? ? ? ? 85 C9 74 06 5E || A1 ? ? ? ? 57 8B B0 

	 static DWORD m_AttackDelayOff; // B9 ? ? ? ? E8 ? ? ? ? B9 ? ? ? ? F3
};










#define MAKE_PAD(size) char pad##size##[size];


enum DmgTypes {
	Physical = 0,
	Magical,
	True
};

enum VisibleEnum
{
	Visible = 257,
	nonVisible = 256
};


enum EntityTypes {
	Hero = 5121,
	Mobs = 3073,
	Structure = 131077
};


enum Team {
	FirstTeam = 100,
	SeconTeam = 200,
	Jungle = 300
};

enum LifeState
{
	Living = 642,
	Dead = 658
};

template <typename obfuscation>
__forceinline obfuscation deobfuscate(int obj, uintptr_t offset)
{
	uintptr_t tmp1 = obj + offset;
	BYTE temp[100];
	ReadProcessMemory(Management::Manager._process, (LPCVOID)(obj + offset), temp, 100, 0);

	uint8_t tmp2 = temp[0x1];
	uintptr_t tmp3 = temp[0x10];

	uintptr_t tmp_xor = *(uintptr_t*)&temp[0x4 * tmp2 + 0x4];

	tmp_xor ^= ~(tmp3 ^ XOR_KEY);

	return *(obfuscation*)(&tmp_xor);
}

enum SpellSlots {
	Spell_Q = 0,
	Spell_W,
	Spell_E,
	Spell_R,
	Spell_D,
	Spell_F,
	Spell_1,
	Spell_2,
	Spell_3,
	Spell_4,
	Spell_5,
	Spell_6,
	Spell_7
};


class CRenderer
{
public:
	MAKE_PAD(0x18);
	int ScreenResolutionX; //0x18
	int ScreenResolutionY; //0x1C
	MAKE_PAD(0x48);
	D3DXMATRIX ViewMatrix; //0x8C
	D3DXMATRIX ProjectionMatrix; //0xCC
};


class CSpellInfo {
public:
	MAKE_PAD(0x1C);
	int level; // 0x1C
	MAKE_PAD(0x4);
	float time; // 0x24
	MAKE_PAD(0x58);
	float vayneQDamage;
};

class Smth1 {
public:
	MAKE_PAD(0x1EC);
	float unknownFloat;
};

class Smth2 {
public:
	MAKE_PAD(0x1C);
	Smth1 unknownSmth1;
};

bool isHeroSupporter(int hero, int a2);


//bool isHeroSupporter(int hero, int a2)
//{
//	unsigned __int8 _this[0x100];
//	ReadProcessMemory(Manager._process, (LPBYTE)hero + 72, _this, 0x100, NULL);
//	int v2; // eax
//	unsigned int v4; // ecx
//	unsigned int v5; // edi
//	int *v6; // edx
//	int v7; // eax
//	BYTE v8; // al
//	unsigned int i; // eax
//	int v11; // [esp+8h] [ebp-4h]
//
//	v2 = _this[1];
//	v4 = 0;
//	v5 = _this[20];
//
//	std::cout << "v5: " << v5 << std::endl;
//
//	std::cout << "v11 equaling" << std::endl;
//	v11 = *(int*)&_this[4 * v2 + 4];
//	if (v5)
//	{
//		v6 = (int *)(_this + 16);
//		do
//		{
//			std::cout << "v4: " <<v4 << std::endl;
//
//
//			try {
//				v7 = *v6;
//				++v6;
//				*(&v11 + v4) ^= ~(v7 ^ XOR_KEY);
//				++v4;
//			}
//			catch (...) {
//				break;
//			}
//		} while (v4 < v5);
//	}
//	v8 = _this[21];
//	if (v8)
//	{
//		for (i = 4 - v8; i < 4; ++i)
//		{
//			std::cout << "i: " << i << std::endl;
//
//			*((BYTE *)&v11 + i) ^= *(BYTE *)(_this + i + 16) ^ 0xA5;
//		}
//	}
//
//
//	return (v11 & a2) != 0;
//
//}

float AttackCastDelay(int a1, int a2); // sub_11140C0
float AttackDelay(DWORD a1, float a2); // sub_11141C0




inline int GetPing()
{
	return Management::Manager.Read<int>(Management::Manager.Read<int>(Management::Manager.Read<int>(Management::LolClient.dwBase + offsets::m_PingContainer) + 0x68) + 0x18);
}

enum class BuffType {
	Internal = 0,
	Aura = 1,
	CombatEnchancer = 2,
	CombatDehancer = 3,
	SpellShield = 4,
	Stun = 5,
	Invisibility = 6,
	Silence = 7,
	Taunt = 8,
	Polymorph = 9,
	Slow = 10,
	Snare = 11,
	Damage = 12,
	Heal = 13,
	Haste = 14,
	SpellImmunity = 15,
	PhysicalImmunity = 16,
	Invulnerability = 17,
	Sleep = 18,
	NearSight = 19,
	Frenzy = 20,
	Fear = 21,
	Charm = 22,
	Poison = 23,
	Suppression = 24,
	Blind = 25,
	Counter = 26,
	Shred = 27,
	Flee = 28,
	Knockup = 29,
	Knockback = 30,
	Disarm = 31
};


typedef struct Node{
public:
	int abc;
};

void teasdasd()
{
	Node test;

}

class CObject {
public:
	int vtable;
	char pad_0x0000[0x40]; //0x0000
	__int32 team; //0x0044 
	char pad_0x0048[0x18]; //0x0048
	char name[0x78]; //0x60
	D3DXVECTOR3 bounding_box_first_point; //0x60
	D3DXVECTOR3 bounding_box_second_point; //0x60
	char pad_f0[0x20]; //0xF0
	int network_id; //0x0110 
	char pad_0x0114[0x88]; //0x0114
	D3DXVECTOR3 position; // 0x019C

	//float positionx; //0x019C 
	//float positiony; //0x019C 
	//float positionz; //0x019C 
	char pad_0x01A8[0xA4]; //0x01A8
	__int16 source_index; //0x024C 
	char pad_0x0250[0x16]; //0x0250
	D3DXVECTOR3 skill_start_pos;
	D3DXVECTOR3 skill_end_pos;
	char pad_0x027C[0x1C]; //0x027C
	__int16 target_id; //0x0298 
	char pad_0x029C[0x136]; //0x029C
	__int32 visible;//0x3D0
	char pad_0x03D4[0x24]; //0x03D4
	float mana; //0x03F8 
	char pad_0x03FC[0xC]; //0x03FC
	float max_mana; //0x0408 
	char pad_0x040C[0x68]; //0x040C
	char pad_sd[0x2A4]; // 0x474
	float missileLineWidth; // 0x718
	char pad_0x071C[0x6A8]; //pad_0x071C
	float health; //0xDC4 
	char pad_0x0DEC[0xC]; //0x0DEC
	float max_health; //0x0DF8 
	char pad_0x0DFC[0x3E8]; //0x0DFC
	float bonus_attack; //0x11E0 
	char pad_0x11E4[0xC]; //0x11E4
	float bonus_ability_attack; //0x11F0 
	char pad_0x11F4[0x68]; //0x11F4
	float attack_speed_mod; // 0x125C
	float base_attack; //0x1260 
	char pad_0x1264[0x20]; //0x1264
	float crit_mod; //0x1284 
	float armor; //0x1288 
	char pad_0x128C[0x4]; //0x128C
	float magic_resist; //0x1290 
	float bonus_magic_resist; //0x1294 
	char pad_0x1298[0x8]; //0x1298
	float move_speed; //0x12A0 
	char pad_0x12A4[0x4]; //0x12A4
	float attack_range; //0x12A8 
	char pad_0x12AC[0xE44]; //0x12AC
	DWORD buff_manager; //0x20F0 
	char pad_0x20F4[0x7B4]; //0x20F4
	DWORD spell_book; //0x28A8 
	char pad_0x28AC[0x3C]; //0x28AC
	int current_attack; // 0x28C8
	char pad_0x28CC[0x88C]; //0x28CC
	char hero_name[60]; // 0x3158
	//__int32 level; //0x4964 
	// 0x28C8 is attacking
	int basePosition;
	float predicted_health;
	int predicted_after_time;
	int focus_count;


	


	bool IsColliding(D3DXVECTOR3, D3DXVECTOR3, float);

	bool isVisible()
	{
		return true;
	}

	bool isValid()
	{
		return isTargetable() && !isDead();
	}

	bool isTargetable()
	{
		bool is_targetable = Management::Manager.Read<bool>(basePosition + 0xD3C);

		return is_targetable;
	}

	bool isDead()
	{
		bool is_dead = deobfuscate<bool>(basePosition, 0x3E0);

		return is_dead;
	}

	float GetAttackDelay()
	{
		float val = AttackDelay(basePosition, attack_speed_mod);

		return val;
	}

	float GetAttackCastDelay()
	{
		float val = AttackCastDelay(basePosition, 64);

		return val;
	}

	bool IsAttacking()
	{
		return Management::Manager.Read<int>( basePosition + offsetOf(&CObject::current_attack) ) != 0;
	}

	std::string GetName()
	{
		return std::string(name);
	}
	std::string GetHeroName()
	{
		return std::string(hero_name);
	}

	bool isHero()
	{
		return basePosition /*&& Management::Manager.Read<int>(basePosition) == Management::local_player.vtable */&& (deobfuscate<int>(basePosition, 0x48) & 4096) != 0;
	}

	bool isMissile()
	{
		return basePosition && (deobfuscate<int>(basePosition, 0x48) & 32768) != 0;
	}

	bool isMinion()
	{
		return basePosition && (deobfuscate<int>(basePosition, 0x48) & 2048) != 0;
	}

	bool  isTurret()
	{
		return basePosition && (deobfuscate<int>(basePosition, 0x48) & 8192) != 0;
	}

	CSpellInfo GetSpellInfo(int spellSlot)
	{
		return Management::Manager.Read<CSpellInfo>(spell_book + (spellSlot * 4));
	}

	static CObject GetLocalPlayer()
	{
		static int address = Management::Manager.Read<DWORD>(Management::LolClient.dwBase + offsets::m_LocalPlayer);
		CObject local_player = Management::Manager.Read<CObject>(address);
		local_player.basePosition = address;
		local_player.predicted_health = local_player.health;
		local_player.predicted_after_time = 0;
		local_player.focus_count = 0;


		return local_player;
	}

	static CObject GetUnderMouseObject()
	{
		int address = Management::Manager.Read<DWORD>(Management::LolClient.dwBase + offsets::m_UnderMouseObj);
		CObject under_mouse_object;
		under_mouse_object.network_id = 0;

		if (address > 0)
			under_mouse_object = Management::Manager.Read<CObject>(address);

		under_mouse_object.basePosition = address;
		under_mouse_object.predicted_health = under_mouse_object.health;
		under_mouse_object.predicted_after_time = 0;
		under_mouse_object.focus_count = 0;

		return under_mouse_object;
	}


	operator DWORD() { return DWORD(); }

	bool operator ==(CObject other)
	{
		return network_id == other.network_id;
	}



};

class CActiveSkillClass {
public:
	int vtable;
	char pad_0x4[0x8];
	int spell_index;
	float time_casted;
	char pad_0x14[0x68];
	D3DXVECTOR3 start_pos;
	D3DXVECTOR3 end_pos;
	char pad_0x94[0x44C];
	float line_width;

	CObject caster;
	int base_address;


	float GetSpellCastRange()
	{
		int skill_info = Management::Manager.Read<int>(caster.basePosition + 0x2DB0 + spell_index * 4);

		float cast_range = Management::Manager.Read<float>(Management::Manager.Read<int>(Management::Manager.Read<int>(skill_info + 0x134) + 0x38) + 0x3D0);

		return cast_range;
	}

	std::string GetSpellName()
	{
		int skill_info = Management::Manager.Read<int>(caster.basePosition + 0x2DB0 + spell_index * 4);

		char* name = (char*)Management::Manager.ReadBytes(Management::Manager.Read<int>(Management::Manager.Read<int>(Management::Manager.Read<int>(skill_info + 0x134) + 0x38) + 0x58), 32);

		return name;
	}


	void GatherInfo()
	{
		*this = Management::Manager.Read<CActiveSkillClass>(base_address);
	}

};

inline CObject GetObjectByIndex(int index)
{
	CObject obj = Management::Manager.Read<CObject>(Management::Manager.Read<int>(Management::Manager.Read<int>(Management::LolClient.dwBase + offsets::m_ObjectList) + index * 4));

	if (obj.source_index > 10000 || obj.source_index < 0)
	{
		CObject obj2;
		obj2.network_id = 0;

		return obj2;
	}

	return obj;
}


class ObjectManager

{
public:
	int base;
	DWORD ObjectArray;
	int MaxObjects;
	int ObjectCount;
	int indexLastObject;

	CObject GetEntityFromIndex(int index)
	{
		if (index >= MaxObjects)
			throw -1;

		CObject entity;
		void* temp = (void*)-1;
		DWORD obj = Management::Manager.Read<DWORD>(ObjectArray + (0x4 * index));
		if (obj > 0)
			entity = Management::Manager.Read<CObject>(obj);
		else
			temp = &entity;

		if (temp != (void*)-1) throw -1;

		return entity;
	}

	CObject GetEntityFromnetwork_id(DWORD index)
	{
		CObject null; null.network_id = 0;
		DWORD entityList[10000];
		ReadProcessMemory(Management::Manager._process, (LPVOID)(ObjectArray + 0x0), &entityList, sizeof(DWORD) * 10000, 0);

		for (int i = 0; i < 10000; i++)
		{
			if (entityList[i] <= 0) continue;
			CObject entity = Management::Manager.Read<CObject>(entityList[i]);
			if (entity.network_id == index)
			{
				null = entity;
				break;
			}
		}
		
		return null;
	}
};




inline D3DXVECTOR2 WorldToScreen(D3DXVECTOR3 pos)
{

	CRenderer renderer = Management::Manager.Read<CRenderer>(Management::Manager.Read<DWORD>(Management::LolClient.dwBase + offsets::m_RendererClass));

	D3DXMATRIX viewMatrix = renderer.ViewMatrix;
	D3DXMATRIX projMatrix = renderer.ProjectionMatrix;

	D3DXVECTOR2 returnVec = D3DXVECTOR2(-1,-1);
	D3DXVECTOR2 screen = D3DXVECTOR2(renderer.ScreenResolutionX, renderer.ScreenResolutionY);

	D3DXMATRIX* matrix = new D3DXMATRIX();
	D3DXMatrixMultiply(matrix, &viewMatrix, &projMatrix);

	D3DXVECTOR4 clipCoords = D3DXVECTOR4();

	clipCoords.x = pos.x * matrix->_11 + pos.y * matrix->_21 + pos.z * matrix->_31 + matrix->_41;
	clipCoords.y = pos.x * matrix->_12 + pos.y * matrix->_22 + pos.z * matrix->_32 + matrix->_42;
	clipCoords.z = pos.x * matrix->_13 + pos.y * matrix->_23 + pos.z * matrix->_33 + matrix->_43;
	clipCoords.w = pos.x * matrix->_14 + pos.y * matrix->_24 + pos.z * matrix->_34 + matrix->_44;


	if (clipCoords[3] <= 1.0f)
		return returnVec;

	D3DXVECTOR3 M = D3DXVECTOR3();
	M.x = clipCoords.x / clipCoords.w;
	M.y = clipCoords.y / clipCoords.w;
	M.z = clipCoords.z / clipCoords.w;

	returnVec.x = (screen.x / 2 * M.x) + (M.x + screen.x / 2);
	returnVec.y = -(screen.y / 2 * M.y) + (M.y + screen.y / 2);

	//if (returnVec.x > renderer.ScreenResolutionX || returnVec.x < 0)
	//	return D3DXVECTOR2(0, 0);

	//if (returnVec.y > renderer.ScreenResolutionY || returnVec.y < 0)
	//	return D3DXVECTOR2(0, 0);

	return returnVec;
}






inline bool isHero(int a1)
{
	return a1 && Management::Manager.Read<int>(a1) == Management::local_player.vtable && isHeroSupporter(a1, 4096);
}

inline bool isMissile(int a1)
{
	return a1 && isHeroSupporter(a1, 32768);
}

inline bool isMinion(int a1)
{
	return a1 && isHeroSupporter(a1, 2048);
}

inline bool  isTurret(int a1)
{
	return a1 && isHeroSupporter(a1, 8192);
}

static class SObject {
public:
	static std::vector<CObject> allChampions;

	static std::vector<CObject> allMobs;

	static std::vector<CObject> allTurrets;

	static std::vector<CActiveSkillClass> allMissiles;
};



inline void ReadEntities(bool read_list = false)
{
	static int index_list = 1;

	//CanMakeProggress = false;
	SObject::allChampions.clear();
	SObject::allMobs.clear();
	SObject::allMissiles.clear();
	//allTurrets.clear();


	int size = 10000;

	DWORD objectList2 = Management::Manager.Read<DWORD>(Management::LolClient.dwBase + offsets::m_ObjectList);
	size = Management::Manager.Read<int>(Management::LolClient.dwBase + offsets::m_ObjectList + 0xC) + 1500;

	if (size > 10000)
		size = 10000;

	DWORD* entityList = new DWORD[size];// = new DWORD[size];
	ReadProcessMemory(Management::Manager._process, (LPVOID)(objectList2 + 0x0), entityList, sizeof(DWORD) * size, 0);


	CObject local_player = Management::local_player;


	for (int i = 0; i < size; i++)
	{

		if (entityList[i] <= 0) continue;

		if (isHero(entityList[i]))
		{
			CObject entity = Management::Manager.Read<CObject>(entityList[i]);

			entity.basePosition = entityList[i];
			entity.predicted_health = entity.health;
			entity.predicted_after_time = 0;
			entity.focus_count = 0;

			//if (entity.GetHeroName().find("Ezreal") != std::string::npos)
			//	std::cout << "i: " << i << std::endl;

			SObject::allChampions.push_back(entity);

			if (std::find(Management::Player_List.begin(), Management::Player_List.end(), entity) == Management::Player_List.end())
			{
				Management::Player_List.push_back(entity);
				if (entity.team != local_player.team)
				{
					Management::Priority_List[entity.network_id] = index_list;
					index_list++;
				}
			}
		}
		else if (isMinion(entityList[i]))
		{
			CObject entity = Management::Manager.Read<CObject>(entityList[i]);

			entity.basePosition = entityList[i];
			entity.predicted_health = entity.health;
			entity.predicted_after_time = 0;
			entity.focus_count = 0;

			SObject::allMobs.push_back(entity);
		}
		else if (isMissile(entityList[i]))
		{
			CObject entity = Management::Manager.Read<CObject>(entityList[i]);
			
			CActiveSkillClass toAddObj;

			toAddObj.start_pos = entity.skill_start_pos;
			toAddObj.end_pos = entity.skill_end_pos;
			toAddObj.line_width = entity.missileLineWidth;
			toAddObj.caster = GetObjectByIndex(entity.source_index);
			toAddObj.base_address = entityList[i];

			SObject::allMissiles.push_back(toAddObj);
		}



		//else if (entity.type == Structure)
		//{
		//	allTurrets.insert(allTurrets.begin(),entity);
		//}

	}




	if (read_list)
	{
		std::map<int, int> temp_map = Management::Priority_List;
		for (int i = 0; i < LuaKeeper::lua_list.size(); i++)
		{
			luabridge::push(LuaKeeper::lua_list[i], temp_map);
			lua_setglobal(LuaKeeper::lua_list[i], "priority_list");

			luabridge::push(LuaKeeper::lua_list[i], Management::Player_List);
			lua_setglobal(LuaKeeper::lua_list[i], "all_champions");

			luabridge::push(LuaKeeper::lua_list[i], SObject::allMobs);
			lua_setglobal(LuaKeeper::lua_list[i], "all_mobs");

			luabridge::push(LuaKeeper::lua_list[i], SObject::allMissiles);
			lua_setglobal(LuaKeeper::lua_list[i], "all_missiles");
		}



	}



}



inline void ReadMissiles(bool read_list = false)
{
	static int index_list = 1;

	//CanMakeProggress = false;
	SObject::allMissiles.clear();
	//allTurrets.clear();


	//int size = 10000;

	//DWORD objectList2 = Management::Manager.Read<DWORD>(Management::LolClient.dwBase + offsets::m_ObjectList);
	//size = Management::Manager.Read<int>(Management::LolClient.dwBase + offsets::m_ObjectList + 0xC) + 1500;

	//if (size > 10000)
	//	size = 10000;

	//DWORD* entityList = new DWORD[size];// = new DWORD[size];
	//ReadProcessMemory(Management::Manager._process, (LPVOID)(objectList2 + 0x0), entityList, sizeof(DWORD) * size, 0);


	CObject local_player = Management::local_player;


	for (int i = 0; i < Management::Player_List.size(); i++)
	{
		int addr = Management::Manager.Read<int>(Management::Player_List[i].basePosition + 0x28C8);
		if ( addr > 0 )
		{
			CActiveSkillClass entity = Management::Manager.Read<CActiveSkillClass>(addr);

			entity.caster = Management::Player_List[i];
			entity.base_address = addr;

			SObject::allMissiles.push_back(entity);

		}




		if (read_list)
		{
			for (int i = 0; i < LuaKeeper::lua_list.size(); i++)
			{
				luabridge::push(LuaKeeper::lua_list[i], SObject::allMissiles);
				lua_setglobal(LuaKeeper::lua_list[i], "all_missiles");
			}

		}



	}
}



inline void ReadHeroes(bool read_list = false)
{
	SObject::allChampions.clear();

	if (Management::Player_List.size() <= 0)
		ReadEntities(true);


	CObject local_player = Management::local_player;


	for (int i = 0; i < Management::Player_List.size(); i++)
	{
		int temp_addr = Management::Player_List[i].basePosition;
		Management::Player_List[i] = Management::Manager.Read<CObject>(temp_addr);
		Management::Player_List[i].basePosition = temp_addr;
		Management::Player_List[i].predicted_health = Management::Player_List[i].health;
		Management::Player_List[i].predicted_after_time = 0;
		Management::Player_List[i].focus_count = 0;



		SObject::allChampions.push_back(Management::Player_List[i]);
	}

	for (int i = 0; i < LuaKeeper::lua_list.size(); i++)
	{
		luabridge::push(LuaKeeper::lua_list[i], Management::Player_List);
		lua_setglobal(LuaKeeper::lua_list[i], "all_champions");
	}
}





inline std::vector<CObject> GetEntitiesInField(float distance,CObject local_player, int team = -1, int* count_of_ally_minions = 0)
{

	std::vector<CObject> entities;


	if (local_player.health <= 0.01f) return entities;

	//std::vector<DWORD> network_idS;

	//network_idS.push_back(local_player.network_id);

	for (int i = 0; i < SObject::allChampions.size(); i++)
	{
		//if (std::find(network_idS.begin(), network_idS.end(), allChampions[i].network_id) != network_idS.end()) continue;

		if (SObject::allChampions[i].team == team) continue;

		if (local_player.network_id == SObject::allChampions[i].network_id) continue;

		float diff = (SObject::allChampions[i].position - local_player.position).sqrMagnitude();
		if (diff < distance)
		{
			entities.push_back(SObject::allChampions[i]);
			//network_idS.push_back(allChampions[i].network_id);
		}

	}

	for (int i = 0; i <  SObject::allMobs.size(); i++)
	{
		//if (std::find(network_idS.begin(), network_idS.end(), allMobs[i].network_id) != network_idS.end()) continue;

		float diff = (SObject::allMobs[i].position - local_player.position).sqrMagnitude();
		if (diff < distance)
		{
			if (SObject::allMobs[i].team != team) {
				entities.push_back(SObject::allMobs[i]);

			}

			if (count_of_ally_minions != 0 && SObject::allMobs[i].team == team)
				*count_of_ally_minions += 1;
			//network_idS.push_back(allMobs[i].network_id);
		}

	}

	return entities;
}


inline std::vector<CObject> GetHeroesInField(float distance, CObject local_player,int team = -1)
{

	std::vector<CObject> entities;

	std::vector<DWORD> network_idS;

	//network_idS.push_back(local_player.network_id);

	for (int i = 0; i <  SObject::allChampions.size(); i++)
	{
		//if (std::find(network_idS.begin(), network_idS.end(), allChampions[i].network_id) != network_idS.end()) continue;

		if (SObject::allChampions[i].team == team) continue;

		if (local_player.network_id == SObject::allChampions[i].network_id) continue;

		float diff = (SObject::allChampions[i].position - local_player.position).sqrMagnitude();



		if (diff < distance)
		{
			entities.push_back(SObject::allChampions[i]);
			//network_idS.push_back(allChampions[i].network_id);
		}

	}

	return entities;
}


inline std::vector<CObject> GetMobsInfield(float distance, CObject local_player,int team = -1, int* count_of_ally_minions = 0)
{
	std::vector<CObject> entities;


	//std::vector<DWORD> network_idS;


	for (int i = 0; i <  SObject::allMobs.size(); i++)
	{
		//if (std::find(network_idS.begin(), network_idS.end(), allMobs[i].network_id) != network_idS.end()) continue;

		if (SObject::allMobs[i].team == team) continue;

		float diff = (SObject::allMobs[i].position - local_player.position).sqrMagnitude();
		if (diff < distance)
		{
			entities.push_back(SObject::allMobs[i]);
			//network_idS.push_back(allMobs[i].network_id);

			if (count_of_ally_minions != 0 && SObject::allMobs[i].team == team)
				*count_of_ally_minions += 1;
		}

	}


	return entities;
}



inline float CalculateDmgToObj(CObject obj, int dmgType, float Damage) {
	CObject myObj = Management::local_player;
	float EffectiveHealth = obj.health;
	float resists = 0.0f;

	float ourDamage = Damage;

	if (dmgType == Physical) {
		resists = obj.armor;
		//resists = resists * myObj.armor;
		//resists = resists - myObj.;
	}
	else if (dmgType == Magical)
	{
		resists = obj.magic_resist;
		//resists = resists * myObj.magicPenMultiplier;
		//resists = resists - myObj.mag;

		//ourDamage -= myObj.magicShield;
	}

	EffectiveHealth *= 1.0f + (resists / 100.0f);

	//EffectiveHealth += myObj.;

	return EffectiveHealth - ourDamage;
}


inline CObject GetClosestOneInArray(std::vector<CObject> objArray, CObject local_player)
{
	float mMax = 999999.f;

	CObject entity; entity.network_id = 0;
	

	for (int i = 0; i < objArray.size(); i++)
	{
		float diff = (objArray[i].position - local_player.position).sqrMagnitude();
		if (diff < mMax)
		{
			mMax = diff;
			entity = objArray[i];
		}

	}


	return entity;
}


inline std::vector<CObject> GetDueToHealthPercent(std::vector<CObject> objArray, CObject local_player)
{
	std::vector<CObject> player_list;

	for (int i = 0; i < objArray.size(); i++)
	{
		float percent = (objArray[i].health / objArray[i].max_health) * 100.0f;

		if (percent <= Management::percent_health)
		{
			player_list.push_back(objArray[i]);
		}

	}


	return player_list;
}


inline CObject GetTargetDueToPriority(std::vector<CObject> objArray, CObject local_player)
{
	int min = 999;
	int index = -1;

	CObject entity; entity.network_id = 0;


	for (int i = 0; i < objArray.size(); i++)
	{
		if (Management::Priority_List[objArray[i].network_id] == 0)
			continue;

		if (Management::Priority_List[objArray[i].network_id] < min)
		{
			min = Management::Priority_List[objArray[i].network_id];
			index = i;
		}
	}

	if (index != -1)
		return objArray[index];

	return entity;
}



inline CObject GetEntityFromNetworkIdInArray(DWORD network_id)
{
	CObject entity; entity.network_id = 0;


	for (int i = 0; i <  SObject::allChampions.size(); i++)
	{
		if (SObject::allChampions[i].network_id == network_id)
			return  SObject::allChampions[i];
	}

	for (int i = 0; i <  SObject::allMobs.size(); i++)
	{
		if (SObject::allMobs[i].network_id == network_id)
			return  SObject::allMobs[i];
	}


	return entity;
}


//DWORD GetEntityFromnetwork_idDWORD(DWORD index)
//{
//	DWORD null = 0;
//	DWORD entityList[10000];
//	ReadProcessMemory(Manager._process, (LPVOID)(ObjectArray + 0x0), &entityList, sizeof(DWORD) * 10000, 0);
//
//	for (int i = 0; i < 10000; i++)
//	{
//		if (entityList[i] <= 0) continue;
//
//		DWORD entitynetwork_id = Manager.Read<DWORD>(entityList[i] + offsetOf(&CObject::network_id));
//		if (entitynetwork_id == index)
//		{
//			null = entityList[i];
//			break;
//		}
//	}
//
//	return null;
//}
inline CObject GetHighestMaxHealthInArray(std::vector<CObject> objArray)
{
	float mMax = -99999.f;

	CObject entity; entity.network_id = 0;

	for (int i = 0; i < objArray.size(); i++)
	{
		if (objArray[i].max_health > mMax)
		{
			mMax = objArray[i].max_health;
			entity = objArray[i];
		}

	}

	return entity;
}

inline CObject GetKillableInArray(std::vector<CObject> objArray, float damage, int dmgType, CObject* close_to_die = 0)
{
	CObject entity; entity.network_id = 0;


	std::vector<CObject> killable_array;

	for (int i = 0; i < objArray.size(); i++)
	{
		float lastHealth = CalculateDmgToObj(objArray[i], dmgType, damage);
		if (lastHealth <= 0.0f)
		{

			killable_array.push_back(objArray[i]);
		}

		if (close_to_die != 0)
		{
			if (lastHealth <= damage)
			{
				*close_to_die = objArray[i];
			}
		}
	}

	


	return GetHighestMaxHealthInArray(killable_array);
}


inline CObject GetHeroInArray(std::vector<CObject> objArray)
{
	CObject entity; entity.network_id = 0;

	for (int i = 0; i < objArray.size(); i++)
	{
		if (isHero(objArray[i].basePosition))
			return objArray[i];
	}


	return entity;
}



inline CObject GetHighestHealthInArray(std::vector<CObject> objArray)
{
	float mMax = -99999.f;

	CObject entity; entity.network_id = 0;

	for (int i = 0; i < objArray.size(); i++)
	{
		if (objArray[i].health > mMax)
		{
			mMax = objArray[i].health;
			entity = objArray[i];
		}

	}

	return entity;
}


inline CObject GetLowestHealthInArray(std::vector<CObject> objArray)
{
	float mMax = 999999.f;

	CObject entity; entity.network_id = 0;

	for (int i = 0; i < objArray.size(); i++)
	{
		if (objArray[i].health < mMax)
		{
			mMax = objArray[i].health;
			entity = objArray[i];
		}
			
	}

	return entity;
}

inline bool  sub_D2E410(int object)
{
	int v1; // eax
	unsigned int v3; // ecx
	unsigned int v4; // edi
	char v5; // dl
	int *v6; // edx
	int v7; // eax
	unsigned __int8 v8; // al
	unsigned int v9; // eax
	char v11; // [esp+Bh] [ebp-1h]

	BYTE v2[100];
	ReadProcessMemory(Management::Manager._process, (LPCVOID)(object + 468), v2, 100, 0);

	v1 = Management::Manager.Read<BYTE>(object + 469);
	v3 = 0;
	v4 = v2[6];
	v5 = v2[v1 + 2];
	v11 = v2[v1 + 2];
	if (v4)
	{
		v6 = (int *)(v2 + 5);
		do
		{
			v7 = *v6;
			++v6;
			*((DWORD *)&v11) ^= ~(v7 ^ XOR_KEY);
			v3++;
		} while (v3 < v4);
		v5 = v11;
	}
	v8 = v2[7];
	if (!v8)
		return v5;
	v9 = 1 - v8;
	if (v9 >= 1)
		return v5;
	do
	{
		*(&v11) ^= v2[v9 + 5] ^ 0xA5;
		++v9;
	} while (v9 < 1);
	return v11;
}

inline bool sub_D2E3F0(int object)
{
	return sub_D2E410(object - 504);
}




inline bool isAlive(DWORD object)
{
	return sub_D2E3F0(object + 0x7E);
}

inline bool isTargetable(DWORD object)
{
	//unsigned __int32 _this[10];
	//ReadProcessMemory(Management::Manager._process, (LPBYTE)object, _this, 10*4, NULL);

	return isAlive(object);
}

// ((*(double *)(dword_3A9B038 + 0x18)+*((double *)sub_EA65B0((int *)this + 21,xmmword_3A9D080[22 * this[563]],DWORD1(xmmword_3A9D080[22 * this[563]]))+ 22))* 1000.0)

inline std::vector<CObject> GetOnlyAttackable(std::vector<CObject> objArray)
{

	std::vector<CObject> entities;

	for (int i = 0; i < objArray.size(); i++)
	{
		//bool is_targetable = isTargetable(objArray[i].basePosition);

		//std::cout << "address: " << std::hex << objArray[i].basePosition << std::dec << std::endl;

		bool is_dead = deobfuscate<bool>(objArray[i].basePosition, 0x3E0);
		bool is_targetable = Management::Manager.Read<bool>(objArray[i].basePosition + 0xD60);
		//int type = deobfuscate<int>(objArray[i].basePosition, 0x48);

		//std::cout << "is_targetable: " << is_targetable << std::endl;

		if (!is_targetable) continue;

		if (objArray[i].basePosition == Management::local_player.basePosition) continue;
		if (objArray[i].team == Management::local_player.team) continue;

		if (/*objArray[i].invulnerable ||  || !isAlive(objArray[i].basePosition) ||*/ !objArray[i].visible == nonVisible || objArray[i].health <= 0.001f || objArray[i].max_health <= 5.0f || objArray[i].health == 1 || objArray[i].basePosition == Management::local_player.basePosition) continue;
		
		if (is_dead != 0) continue;

		//if (objArray[i].base_attack == 10 && objArray[i].health == 1000 && objArray[i].max_health == 1000) continue;
		//if (objArray[i].base_attack == 21 && objArray[i].health == 100 && objArray[i].max_health == 100) continue;

		if (objArray[i].team != 100 && objArray[i].team != 200) continue;
		entities.push_back(objArray[i]);
	}
	return entities;
}


inline int sub_E853F0(int a1)
{
	int v1; // ecx

	v1 = 0;
	if (a1 - 65 <= 17)
		v1 = a1 - 65;
	return v1;
}


inline bool sub_E85C70(DWORD a1)
{
	return Management::Manager.Read<BYTE>(Management::Manager.Read<DWORD>(Management::LolClient.dwBase + 0x168C184) + 20) && Management::Manager.Read<float>(a1 + 104 * 4) > 0.0;
}

inline double sub_1209160(DWORD a1, float a2, float a3)
{
	DWORD v3; // eax
	float v4; // xmm2_4
	float v5; // xmm1_4
	DWORD v6; // eax
	DWORD v7; // esi
	float v8; // xmm3_4
	float v9; // xmm0_4
	float v10; // xmm4_4
	float v11; // xmm1_4
	double result; // st7
	float v13; // [esp+8h] [ebp-Ch]
	float v14; // [esp+Ch] [ebp-8h]
	float v15; // [esp+10h] [ebp-4h]

	v14 = Management::Manager.Read<float>(Management::LolClient.dwBase + 0x2F275F8 + 0x18 + 0x8);
	v13 = Management::Manager.Read<float>(Management::LolClient.dwBase + 0x2F275F8 + 0x18 + 0xC);
	v3 = Management::LolClient.dwBase + 0x2F275F8 + 0x18;
	v4 = Management::Manager.Read<float>(a1 + 0x2640);
	v15 = Management::Manager.Read<float>(v3);
	if (v4 > 0.0000099999997)
		v14 = 1.0 / v4;
	v5 = Management::Manager.Read<float>(a1 + 0x2644);
	if (v5 > 0.0000099999997)
		v13 = 1.0 / v5;

	DWORD temp1 = Management::Manager.Read<DWORD>(a1 + 0x2538);
	v6 = Management::Manager.Read<DWORD>(temp1 + 0x1C);

	v9 = (float)((float)(a3 + 1.0) * v15) / a2;

	if (v9 <= v13)
		result = fmaxf(v9, v14);
	else
		result = v13;

	return result;
}

inline int sub_DF07B0(int main)
{
	return (int)(main + 24);
}

inline int sub_E13870(int a1)
{
	int v1; // ecx

	v1 = 0;
	if ((unsigned int)(a1 - 64) <= 0x11)
		v1 = a1 - 64;
	return v1;
}

inline unsigned int func_from_ida(int player)
{
	int result = 0;


	int thing = Management::Manager.Read<int>(player + 0x30DC);


	if (thing)
		result = Management::Manager.Read<unsigned int>(thing + 0x1C);
	else
		result = 0;



	return result;
}




inline bool sub_E14960(unsigned int player)
{
	return Management::Manager.Read<BYTE>(Management::Manager.Read<DWORD>(Management::LolClient.dwBase + 0x16B864C) + 18) && Management::Manager.Read<float>(player + 0x1A4) > 0.0;

	// Manager.Read<BYTE>(Manager.Read<DWORD>(LolClient.dwBase + 0x16C5C54) + 0x1F) && Manager.Read<float>(a1 + 0x190) > 0.0;
}

//LolClient.dwBase+0x2F275F8
//double  sub_11141C0(float a1, float a2)
//{
//	try
//	{
//		char v2; // eax
//		float v3; // esi
//		unsigned int v4; // ecx
//		float v5; // xmm0_4
//		int v6; // eax
//		unsigned int v7; // edi
//		float v8; // xmm0_4
//		int v9; // edx
//		int v10; // eax
//		unsigned __int8 v11; // al
//		unsigned int v12; // edx
//		unsigned int v13; // ecx
//		int v14; // eax
//		unsigned int v15; // ecx
//		unsigned int v16; // ebx
//		float v17; // xmm0_4
//		int v18; // edi
//		int v19; // eax
//		unsigned __int8 v20; // al
//		int v21; // eax
//		int v22; // edi
//		float v23; // xmm0_4
//		float v24; // xmm3_4
//		float v25; // xmm4_4
//		float v26; // xmm1_4
//		double result; // st7
//		float v28; // [esp+8h] [ebp-8h]
//		float v29; // [esp+Ch] [ebp-4h]
//		int val = sub_DF07B0(LolClient.dwBase + 0x2F275F8);
//		v29 = Manager.Read<float>(val + 8);
//		v2 = val;
//		v3 = a1;
//		v4 = 0;
//		v5 = Manager.Read<float>(v2 + 12);
//		v6 = Manager.Read<unsigned __int8>((a1)+13165);
//		v7 = Manager.Read<unsigned __int8>((a1)+13184);
//		v28 = v5;
//		v8 = Manager.Read<float>((a1)+4 * v6 + 13168);
//
//		float* temp = new float[100];
//
//		*temp = Manager.Read<float>((a1)+4 * v6 + 13168);
//		if (v7)
//		{
//			int index_one = (v3)+13180;
//			do
//			{
//				v10 = Manager.Read<int>(index_one);
//				index_one += 4;
//				*((DWORD *)temp + v4++) ^= ~(v10 ^ XOR_KEY);
//			} while (v4 < v7);
//			v8 = *temp;
//		}
//		v11 = Manager.Read<BYTE>((v3)+13185);
//
//		v12 = 4;
//		if (v11)
//		{
//			if (v11 > 4)
//				v11 = 4;
//			v13 = 4 - v11;
//			if (v13 < 4)
//			{
//				do
//				{
//					*((BYTE *)temp + v13) ^= Manager.Read<BYTE>(*(DWORD*)&v3 + v13 + 13180) ^ 0xA5;
//					++v13;
//				} while (v13 < 4);
//				v8 = *temp;
//			}
//		}
//		if (v8 > 0.0000099999997)
//			v29 = 1.0 / v8;
//
//		v14 = Manager.Read<unsigned __int8>((v3)+13189);
//		v15 = 0;
//		v16 = Manager.Read<unsigned __int8>((v3)+13208);
//		v17 = Manager.Read<float>((v3)+4 * v14 + 13192);
//		*temp = Manager.Read<float>((v3)+4 * v14 + 13192);
//		if (v16)
//		{
//			int index_two = (v3)+13204;
//			do
//			{
//				v19 = Manager.Read<int>(index_two);
//				index_two += 4;
//				*((DWORD *)temp + v15++) ^= ~(v19 ^ XOR_KEY);
//			} while (v15 < v16);
//			v17 = *temp;
//		}
//		v20 = Manager.Read<BYTE>((v3)+13209);
//		if (v20)
//		{
//			v12 = 4 - v20;
//			if (v12 < 4)
//			{
//				do
//				{
//					*((BYTE *)temp + v12) ^= Manager.Read<BYTE>((v3)+v12 + 13204) ^ 0xA5;
//					++v12;
//				} while (v12 < 4);
//				v17 = *temp;
//			}
//		}
//		if (v17 > 0.0000099999997)
//			v28 = 1.0 / v17;
//		v21 = func_from_ida(*reinterpret_cast<int*>(&v3));
//		v22 = v21;
//		if (v21)
//		{
//			v23 = 0.0;
//			if (sub_E14960(v21))
//			{
//				v24 = Manager.Read<float>((v3)+4672);
//				v25 = v24 == 0.0 ? 1.0 : v24;
//				v26 = (float)((float)((float)((float)(a2 / v25) - 1.0) * Manager.Read<float>(v22 + 105 * 4)) + Manager.Read<float>(v22 + 104 * 4)) * v24;
//				if (v26 != 0.0)
//					v23 = 1.0 / v26;
//			}
//
//		}
//		else
//		{
//			v23 = 0.0;
//		}
//		if (v23 <= v28)
//		{
//			*temp = fmaxf(v23, v29);
//			result = *temp;
//		}
//		else
//		{
//			*temp = v28;
//			result = v28;
//		}
//
//		free(temp);
//
//		return result;
//	}
//	catch (...)
//	{
//		return 0;
//	}
//}


//double sub_1209160(DWORD a1, float a2, float a3)
//{
//	DWORD v3; // eax
//	float v4; // xmm2_4
//	float v5; // xmm1_4
//	DWORD v6; // eax
//	DWORD v7; // esi
//	float v8; // xmm3_4
//	float v9; // xmm0_4
//	float v10; // xmm4_4
//	float v11; // xmm1_4
//	double result; // st7
//	float v13; // [esp+8h] [ebp-Ch]
//	float v14; // [esp+Ch] [ebp-8h]
//	float v15; // [esp+10h] [ebp-4h]
//
//	v14 = Manager.Read<float>(LolClient.dwBase + 0x2F275F8 + 0x18 + 0x8);
//	v13 = Manager.Read<float>(LolClient.dwBase + 0x2F275F8 + 0x18 + 0xC);
//	v3 = LolClient.dwBase + 0x2F275F8 + 0x18;
//	v4 = Manager.Read<float>(a1 + 0x2640);
//	v15 = Manager.Read<float>(v3);
//	if (v4 > 0.0000099999997)
//		v14 = 1.0 / v4;
//	v5 = Manager.Read<float>(a1 + 0x2644);
//	if (v5 > 0.0000099999997)
//		v13 = 1.0 / v5;
//
//	DWORD temp1 = Manager.Read<DWORD>(a1 + 0x2538);
//	v6 = Manager.Read<DWORD>(temp1 + 0x1C);
//
//	v9 = (float)((float)(a3 + 1.0) * v15) / a2;
//
//	if (v9 <= v13)
//		result = fmaxf(v9, v14);
//	else
//		result = v13;
//
//	return result;
//}


//LolClient.dwBase+0x2F275F8




inline double GetAttackCastDelay(DWORD player_arg, int a2)
{
	int v3; // ebx
	int v4; // edi
	int v5; // eax
	DWORD player; // esi
	float attackspeedmod; // ST20_4
	float v8; // ST24_4
	float v9; // ST1C_4
	float v10; // ST20_4
	float v12; // [esp+28h] [ebp+4h]
	float v13; // [esp+28h] [ebp+4h]

	//DWORD temp1 = Manager.Read<DWORD>(player_arg + 0x2538);
	//temp2 = Manager.Read<DWORD>(temp1 + 0x1C);

	v3 = sub_E853F0(a2);
	v4 = sub_E853F0(a2);
	v5 = sub_E853F0(a2);
	player = player_arg;
	attackspeedmod = Management::Manager.Read<float>(player_arg + offsetOf(&CObject::attack_speed_mod));
	//v12 = Manager.Read<float>(temp2 + 4 * v5 + 0x234);
	//v8 = Manager.Read<float>(temp2 + 4 * v3 + 0x1A4);
	v9 = fmaxf(Management::Manager.Read<float>(Management::LolClient.dwBase + 0x2F275F8 + 0x1C), 0.0);
	v10 = sub_1209160(player, attackspeedmod, 0);
	v13 = sub_1209160(player, 1.0, 0) * v9;
	return fminf((float)((float)((float)(v10 * v9) - v13) * 1) + v13, v10);
}





#endif // !GLOBALS_H

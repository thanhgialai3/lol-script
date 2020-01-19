#include "Globals.h"

#include "LineCollision.h"


bool CObject::IsColliding(D3DXVECTOR3 start_pos, D3DXVECTOR3 end_pos, float width)
{
	D3DXVECTOR3 pos1 = bounding_box_first_point;
	pos1.y = position.y;
	D3DXVECTOR3 pos2 = bounding_box_second_point;
	pos2.y = position.y;

	Geometry::LineCollision line = Geometry::LineCollision(start_pos, end_pos, width);
	return line.IsCollision(Geometry::Circle3D(position, (pos1 - pos2).sqrMagnitude() / 3.0f));

}


//
bool isHeroSupporter(int hero, int a2)
{
	try {
		BYTE v2; // eax
		int v3; // esi
		unsigned int v4; // ecx
		unsigned __int8 v5; // edi
		int v6; // edx
		int v7; // eax
		unsigned int v8; // al
		unsigned int i; // eax
		int v11; // [esp+8h] [ebp-4h]

		if (hero <= 0)
			return 0;

		BYTE temp[100];
		ReadProcessMemory(Management::Manager._process, LPCVOID(hero + 72), temp, 44, 0);

		v2 = temp[1];
		v3 = hero + 72;
		v4 = 0;
		v5 = temp[20];

		v11 = *(DWORD*)&temp[4 * v2 + 4];

		v7 = *(int*)&temp[16];

		v11 ^= ~(v7 ^ XOR_KEY);

		return (v11 & a2) != 0;
	}
	catch (...)
	{
		return 0;
	}
}


float AttackDelay(DWORD a1, float a2) // sub_11141C0
{

	int v2; // eax
	DWORD v3; // esi
	unsigned int v4; // ecx
	float v5; // xmm0_4
	int v6; // eax
	unsigned int v7; // edi
	float v8; // xmm0_4
	int *v9; // edx
	int v10; // eax
	unsigned __int8 v11; // al
	unsigned int v12; // edx
	unsigned int v13; // ecx
	int v14; // eax
	unsigned int v15; // ecx
	unsigned int v16; // ebx
	float v17; // xmm0_4
	int v18; // edi
	int v19; // eax
	unsigned __int8 v20; // al
	unsigned int v21; // eax
	unsigned int v22; // edi
	float v23; // xmm0_4
	float v24; // xmm3_4
	float v25; // xmm4_4
	float v26; // xmm1_4
	double result; // st7
	float v28; // [esp+8h] [ebp-8h]
	float v29; // [esp+Ch] [ebp-4h]

	v29 = Management::Manager.Read<float>(sub_DF07B0(Management::LolClient.dwBase + offsets::m_AttackDelayOff) + 8);
	v2 = sub_DF07B0(Management::LolClient.dwBase + offsets::m_AttackDelayOff);
	v3 = a1;

	v4 = 0;
	v5 = Management::Manager.Read<float>(v2 + 0xC); // above v15
	BYTE temp[100];
	ReadProcessMemory(Management::Manager._process, (LPCVOID)(a1 + 0x336D), temp, 100, 0);
	v6 = temp[0];
	v7 = temp[0x13];
	v28 = v5;
	v8 = *(float*)&temp[3 + 4 * v6]; // above v4
	a1 = v8;

	if (v7)
	{
		int to_read = 0xF;
		do
		{
			v10 = *(int*)&temp[to_read];
			to_read += 4;
			*((DWORD *)&a1 + v4++) ^= ~(v10 ^ XOR_KEY);
		} while (v4 < v7);
		v8 = a1;
	}
	//v11 = temp[0x14];
	//v12 = 4;
	//if (v11)
	//{
	//	v13 = 4 - v11;
	//	if (v13 < 4)
	//	{
	//		do
	//		{
	//			*((BYTE *)&a1 + v13) ^= temp[0xF + v13] ^ 0xA5;
	//			++v13;
	//		} while (v13 < 4);
	//		v8 = a1;
	//	}
	//}


	if (v8 > 0.0000099999997)
		v29 = 1.0 / v8;



	v14 = temp[0x18];
	v15 = 0;
	v16 = temp[0x2B];
	v17 = *(float*)&temp[4 * v14 + 0x1B];
	a1 = v17;

	if (v16)
	{
		v18 = 0x27;
		do
		{
			v19 = *(int*)&temp[v18];
			v18 += 4;
			*((DWORD *)&a1 + v15++) ^= ~(v19 ^ XOR_KEY);
		} while (v15 < v16);
		v17 = a1;
	}


	v20 = temp[0x2C];
	if (v20)
	{
		v12 = 4 - v20;
		if (v12 < 4)
		{
			do
			{
				*((BYTE *)&a1 + v12) ^= temp[0x27 + v12] ^ 0xA5;
				++v12;
			} while (v12 < 4);
			v17 = a1;
		}
	}


	if (v17 > 0.0000099999997)
		v28 = 1.0 / v17;

	v21 = func_from_ida(v3);
	v22 = v21;
	if (v21)
	{
		v23 = 0.0;
		if (sub_E14960(v21))
		{
			v24 = Management::Manager.Read<float>((v3)+4640);
			v25 = v24 == 0.0 ? 1.0 : v24;
			v26 = (float)((float)((float)((float)(a2 / v25) - 1.0) * Management::Manager.Read<float>((int)v22 + 424)) + Management::Manager.Read<float>((int)v22 + 420)) * v24;
			if (v26 != 0.0)
				v23 = 1.0 / v26;
		}
	}
	else
	{
		v23 = 0.0;
	}
	if (v23 <= v28)
	{
		a1 = fmaxf(v23, v29);
		result = a1;
	}
	else
	{
		a1 = v28;
		result = v28;
	}

	//std::cout << "v23: " << v23 << std::endl;
	//std::cout << "v28: " << v28 << std::endl;
	//std::cout << "v29: " << v29 << std::endl;
	//std::cout << "result: " << result << std::endl;

	result = v23;

	return result;
}

float AttackCastDelay(int a1, int a2) // sub_11140C0
{
	int v2; // edi
	int v3; // esi
	int v4; // eax
	float v5; // ST10_4
	float v6; // ST18_4
	float v7; // ST10_4
	float v8; // ST14_4
	float v10; // [esp+1Ch] [ebp+4h]

	v2 = a1;
	v3 = func_from_ida(a1);
	v4 = sub_E13870(a2);
	v5 = Management::Manager.Read<float>(a1 + 1167 * 4);
	v6 = Management::Manager.Read<float>(v3 + 4 * v4 + 440);
	v10 = fmaxf(Management::Manager.Read<float>(sub_DF07B0((Management::LolClient.dwBase + offsets::m_AttackDelayOff)) + 4) + Management::Manager.Read<float>(v3 + 4 * v4 + 512), 0.0);
	v7 = AttackDelay(v2, v5);
	v8 = AttackDelay(v2, 1.0) * v10;

	//std::cout << "v5: " << v5 << std::endl;
	//std::cout << "v6: " << v6 << std::endl;
	//std::cout << "v7: " << v7 << std::endl;
	//std::cout << "v8: " << v8 << std::endl;
	//std::cout << "v10: " << v10 << std::endl;

	float val = (float)((float)((float)(v7 * v10) - v8) * v6) + v8;
	if (val == 0)
		return v7;

	return fminf(val, v7);
}

//
//bool isHeroSupporter(int hero, int a2)
//{
//	try {
//		BYTE v2; // eax
//		int v3; // esi
//		unsigned int v4; // ecx
//		unsigned __int8 v5; // edi
//		int v6; // edx
//		int v7; // eax
//		unsigned int v8; // al
//		unsigned int i; // eax
//		int v11; // [esp+8h] [ebp-4h]
//
//		if (hero <= 0)
//			return 0;
//
//		BYTE temp[100];
//		ReadProcessMemory(Management::Manager._process, LPCVOID(hero + 72), temp, 100, 0);
//
//
//		v2 = temp[1];
//		//std::cout << "v2: " << v2 << std::endl;
//		v3 = hero + 72;
//		v4 = 0;
//		v5 = temp[20];
//		//std::cout << "v5: " << v5 << std::endl;
//
//		v11 = temp[4 * v2 + 4];
//
//
//		if (v5 > 0 && v4 < 5)
//		{
//			int index = 16;
//			do
//			{
//				v7 = temp[index];
//				index += 4;
//				*(&v11 + v4) ^= ~(v7 ^ XOR_KEY);
//				++v4;
//			} while (v4 < v5);
//		}
//
//
//		v8 = temp[21];
//		//std::cout << "v8: " << v8 << std::endl;
//
//		if (v8 && v8 <= 4)
//		{
//			for (i = 4 - v8; i < 4; ++i)
//				*(&v11 + i) ^= Management::Manager.Read<BYTE>(v3 + i + 16) ^ 0xA5;
//		}
//
//
//		return (v11 & a2) != 0;
//	}
//	catch (...)
//	{
//		return 0;
//	}
//}


//
DWORD offsets::m_LocalPlayer = 0x2F57094; // 3A1AFA4 0x2F709C0 A1 ? ? ? ? 85 C0 0F 84 ? ? ? ? 56 8B B0

DWORD offsets::m_UnderMouseObj = 0x2308e74; // C7 05 ? ? ? ? ? ? ? ? E8 ? ? ? ? 83 C4 04 FF B4

DWORD offsets::m_ObjectList = 0x167856C; // B9 ? ? ? ? E8 ? ? ? ? 8B C8 8B 10 8B 82

DWORD offsets::m_RendererClass = 0x2F76560; // 8B 15 ? ? ? ? 83 EC 08 F3

DWORD offsets::m_PingContainer = 0x2305130; // 8B 0D ? ? ? ? 85 C9 74 06 5E

DWORD offsets::m_AttackDelayOff = 0x2F53B18; // B9 ? ? ? ? E8 ? ? ? ? B9 ? ? ? ? F3


Memory Management::Manager;
Module Management::LolClient;
std::vector<CObject> Management::Player_List;
std::map<int,int> Management::Priority_List;
float Management::percent_health = 25;
CObject Management::current_target;
CObject Management::local_player;
CObject Management::under_mouse_object;
bool Management::wait_for_usage = false;



std::vector<CObject> SObject::allChampions;

std::vector<CObject> SObject::allMobs;

std::vector<CObject> SObject::allTurrets;

std::vector<CActiveSkillClass> SObject::allMissiles;


bool Settings::drawings_enabled = true;
bool Settings::hero_line_draw = true;


std::vector<lua_State*> LuaKeeper::lua_list;
std::mutex LuaKeeper::mtx;           // mutex for critical section



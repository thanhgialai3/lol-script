#include "Orbwalker.h"
#include "Globals.h"

#include "Mouse.h"

#include <iostream>


bool find_it_in_list(std::vector<CObject> list, int addr)
{
	for (int i = 0; i < list.size(); i++)
	{
		if (list[i].basePosition == addr)
			return true;
	}

	return false;

}

//
//int PredictHealth(int object)
//{
//	ReadEntities(false);
//
//	for (int i = 0; i < SObject::allMissiles.size(); i++)
//	{
//		if (SObject::allMissiles[i].basePosition <= 0) continue;
//
//		std::cout << "missile address: " << std::hex << SObject::allMissiles[i].basePosition << std::dec << std::endl;
//
//		//for (int j = 0; j < 100000; j+=4)
//		//{
//		//	int addr = Management::Manager.Read<int>(Management::Player_List[i].basePosition + j);
//
//		//	if (addr <= 0) continue;
//
//		//	if (find_it_in_list(Management::Player_List, addr))
//		//		std::cout << "offset is: " << std::hex << j << std::dec << ", address is: " << std::hex << addr << std::dec << std::endl;
//
//		//}
//
//	}
//
//	//for(int i = 0; i < 	Management::Player_List.size();i++)
//	//{
//	//	if (Management::Player_List[i].basePosition <= 0) continue;
//
//	//	std::cout << "hero address: " << std::hex << Management::Player_List[i].basePosition << std::dec << std::endl;
//	//	std::cout << "hero source_index: " << std::hex << Management::Player_List[i].source_index << std::dec << std::endl;
//	//	//std::cout << "local_player network id: " << std::hex << Management::local_player.network_id << std::dec << std::endl;
//
//	//	int active_spell = Management::Manager.Read<int>(Management::Player_List[i].spell_book + 0x20);
//
//	//	if (active_spell <= 0) continue;
//
//	//	std::cout << "active_spell_address: "<<std::hex << active_spell <<std::dec<< std::endl;
//	//	std::cout << "local_player_source_index: "<<std::hex << Management::local_player.source_index <<std::dec<< std::endl;
//
//
//	//	//for (int j = 0; j < 100000; j+=4)
//	//	//{
//	//	//	int addr = Management::Manager.Read<int>(Management::Player_List[i].basePosition + j);
//
//	//	//	if (addr <= 0) continue;
//
//	//	//	if (find_it_in_list(Management::Player_List, addr))
//	//	//		std::cout << "offset is: " << std::hex << j << std::dec << ", address is: " << std::hex << addr << std::dec << std::endl;
//
//	//	//}
//
//	//}
//
//
//	return 0;
//}




int waitBeforeAttack = 0, waitBeforeWalk = 0;

DWORD cacheTarget = 0;
bool one_time = true;

 void Orbwalker::LaneClear()
{
	 int cur_time = GetTickCount();

	if (cur_time > waitBeforeWalk)
	{
		CObject local_player = Management::local_player;

		if (Management::under_mouse_object.network_id != 0)
		{
			if (Management::under_mouse_object.team == local_player.team)
				Mouse::RightClick();
		}
		else
			Mouse::RightClick();
		
	
		if (cur_time > waitBeforeAttack)
		{

			int count_ally_minion = 0;
			std::vector<CObject> entitiesx = GetMobsInfield(local_player.attack_range + 100, local_player, local_player.team, &count_ally_minion);

			if (entitiesx.size() > 0)
			{
				std::vector<CObject> entities = GetOnlyAttackable(entitiesx);

				CObject target, target_close_to_die;
				target_close_to_die.network_id = 0;
				target.network_id = 0;

				if (target.network_id != local_player.network_id)
				{
					if (target.network_id == 0)
					{
						target = GetKillableInArray(entities, local_player.base_attack + local_player.bonus_attack, Physical, &target_close_to_die);

						if (target.network_id == 0)
						{
							if (target_close_to_die.network_id != 0 && count_ally_minion > 1)
							{
								//target = target_close_to_die;
								return;
							}
							else {
								target = GetHighestHealthInArray(entities);
							}

						}
					}


					if (target.network_id == 0)
						return;


					D3DXVECTOR2 screenPos = WorldToScreen(target.position);



					if (screenPos.x != 0 || screenPos.y != 0)
					{
						cacheTarget = target.network_id;

						//std::cout << "health: " << target.health << std::endl;
						//std::cout << "max_health: " << target.max_health << std::endl;
						//std::cout << "base_attack: " << target.base_attack << std::endl;

						Mouse::ScriptClick(screenPos.x, screenPos.y);
						/*						DWORD temp1 = Manager.Read<DWORD>(local_player.basePosition + 0x2538);
						int temp2 = Manager.Read<DWORD>(temp1 + 0x1C);

						int v12 = Manager.Read<float>(temp2 + 0x234);*/

						double attackCastDelay = AttackDelay(local_player.basePosition, local_player.attack_speed_mod);
						//std::cout << "attack cast delay: " << attackCastDelay << std::endl;

						double attackCastDelayWalk = AttackCastDelay(local_player.basePosition, 64);
						//std::cout << "attackCastDelayWalk: " << attackCastDelayWalk << std::endl;

						int ping = Management::Manager.Read<int>(Management::Manager.Read<int>(Management::Manager.Read<int>(Management::LolClient.dwBase + 0x22D8D0C) + 0x68) + 0x18);

						std::cout << "ping: " << ping << std::endl;

						waitBeforeWalk = cur_time + (int)(attackCastDelayWalk * 10.0f * Orbwalker::adjust_for_attack_cast_delay) + ping / 2; // wait for attack anim
						waitBeforeAttack = cur_time + (int)(attackCastDelay * 10.0f * Orbwalker::adjust_for_attack_delay) + ping / 2; // wait for attack time


						bool thing = false;
						std::cout << "time: " << waitBeforeWalk - cur_time << std::endl;

						for (int try_chance = waitBeforeWalk - cur_time; try_chance > 0; try_chance--) {

							if (Management::Manager.Read<int>(Management::local_player.basePosition + 0x28C8) != 0)
							{
								thing = true;
								break;
							}

							Sleep(1);
						}


						if (!thing)
						{
							waitBeforeWalk = cur_time + 50;
							waitBeforeAttack = 0;
						}
					}
				}
			}
		}

	}
	//else
	//{
	//	int diff = waitBeforeWalk - GetTickCount();
	//	if (diff >= 0)
	//		Sleep(diff);

	//	if (Management::Manager.Read<int>(Management::local_player.basePosition + 0x28C8) == 0 && one_time)
	//	{


	//		waitBeforeWalk = 0;
	//		waitBeforeAttack = 0;

	//		one_time = false;
	//	}
	//}
}


void Orbwalker::Combo()
{
	int cur_time = GetTickCount();

	if (cur_time >= waitBeforeWalk)
	{
		one_time = true;
		CObject local_player = Management::local_player;

		if (Management::under_mouse_object.network_id != 0)
		{
			if (Management::under_mouse_object.team == local_player.team)
				Mouse::RightClick();
		}
		else
			Mouse::RightClick();
			

		if (cur_time >= waitBeforeAttack)
		{
			std::vector<CObject> entitiesx = GetHeroesInField(local_player.attack_range + 100, local_player, local_player.team);

			if (entitiesx.size() > 0)
			{
				std::vector<CObject> entities = GetOnlyAttackable(entitiesx);


				CObject target;

				std::vector<CObject> percent_entities = GetDueToHealthPercent(entities, local_player);

				if(percent_entities.size() > 0)
					target = GetTargetDueToPriority(percent_entities, local_player);
				else
					target = GetTargetDueToPriority(entities, local_player);
				

				if (target.network_id == 0)
					target = GetClosestOneInArray(entities, local_player);

				if (Management::current_target.network_id != 0)
				{
					float diff = (Management::current_target.position - local_player.position).sqrMagnitude();

					if (diff <= local_player.attack_range + 100)
					{
						target = Management::current_target;
					}
				}


				if (target.network_id != local_player.network_id && target.network_id != 0)
				{

					D3DXVECTOR2 screenPos = WorldToScreen(target.position);

					if (screenPos.x != 0 || screenPos.y != 0)
					{
						cacheTarget = target.network_id;

						//std::cout << "health: " << target.health << std::endl;
						//std::cout << "max_health: " << target.max_health << std::endl;
						//std::cout << "base_attack: " << target.base_attack << std::endl;
						//std::cout << "visible: " << target.visible << std::endl;

						Mouse::ScriptClick(screenPos.x, screenPos.y);
						double attackCastDelay = AttackDelay(local_player.basePosition, local_player.attack_speed_mod);

						double attackCastDelayWalk = AttackCastDelay(local_player.basePosition, 64);
						//std::cout << "attackCastDelayWalk: " << attackCastDelayWalk*1000 << std::endl;
						//std::cout << "attack cast delay: " << attackCastDelay*1000 << std::endl;

						int ping = Management::Manager.Read<int>(Management::Manager.Read<int>(Management::Manager.Read<int>(Management::LolClient.dwBase + 0x22D8D0C) + 0x68) + 0x18);

						waitBeforeWalk = cur_time + (int)(attackCastDelayWalk * 10.0f * Orbwalker::adjust_for_attack_cast_delay) + ping / 2; // wait for attack anim
						waitBeforeAttack = cur_time + (int)(attackCastDelay * 10.0f * Orbwalker::adjust_for_attack_delay) + ping / 2; // wait for attack time

						bool thing = false;

						std::cout << "time: " << waitBeforeWalk - cur_time << std::endl;

						for (int try_chance = waitBeforeWalk - cur_time; try_chance > 0; try_chance--) {

							if (Management::Manager.Read<int>(Management::local_player.basePosition + 0x28C8) != 0)
							{
								thing = true;
								break;
							}

							Sleep(1);
						}

						std::cout << "thing: " << thing << std::endl;


						if (!thing)
						{
							waitBeforeWalk = cur_time + 50;
							waitBeforeAttack = 0;
						}

						//Sleep(waitBeforeWalk - cur_time + 50);

						//PredictHealth(target.basePosition);

					}
				}
			}
		}

	}
	//else
	//{
	//	int diff = waitBeforeWalk - GetTickCount();
	//	if (diff >= 0)
	//		Sleep(diff);

	//	if (Management::Manager.Read<int>(Management::local_player.basePosition + 0x28C8) == 0 && one_time)
	//	{


	//		waitBeforeWalk = 0;
	//		waitBeforeAttack = 0;

	//		one_time = false;
	//	}
	//}
}

void Orbwalker::LastHit()
{
	int cur_time = GetTickCount();
	if (cur_time >= waitBeforeWalk)
	{
		CObject local_player = Management::local_player;

		if (Management::under_mouse_object.network_id != 0)
		{
			if (Management::under_mouse_object.team == local_player.team)
				Mouse::RightClick();
		}
		else
			Mouse::RightClick();


		if (cur_time >= waitBeforeAttack)
		{

			std::vector<CObject> entitiesx = GetMobsInfield(local_player.attack_range + 100, local_player, local_player.team);

			if (entitiesx.size() > 0)
			{
				std::vector<CObject> entities = GetOnlyAttackable(entitiesx);

				float damage = local_player.base_attack + local_player.bonus_attack;


				CObject target = GetKillableInArray(entities, damage, Physical);

				if (target.network_id != local_player.network_id && target.network_id != 0)
				{
					D3DXVECTOR2 screenPos = WorldToScreen(target.position);

					if (screenPos.x != 0 || screenPos.y != 0)
					{
						cacheTarget = target.network_id;


						Mouse::ScriptClick(screenPos.x, screenPos.y);
						double attackCastDelay = AttackDelay(local_player.basePosition, local_player.attack_speed_mod);
						//std::cout << "attack cast delay: " << attackCastDelay << std::endl;

						double attackCastDelayWalk = AttackCastDelay(local_player.basePosition, 64);
						//std::cout << "attackCastDelayWalk: " << attackCastDelayWalk << std::endl;

						int ping = Management::Manager.Read<int>(Management::Manager.Read<int>(Management::Manager.Read<int>(Management::LolClient.dwBase + 0x22D8D0C) + 0x68) + 0x18);

						waitBeforeWalk = cur_time + (int)(attackCastDelayWalk * 10.0f * Orbwalker::adjust_for_attack_cast_delay) + ping / 2; // wait for attack anim
						waitBeforeAttack = cur_time + (int)(attackCastDelay * 10.0f * Orbwalker::adjust_for_attack_delay) + ping / 2; // wait for attack time


						bool thing = false;

						for (int try_chance = waitBeforeWalk - cur_time; try_chance > 0; try_chance--) {

							if (Management::Manager.Read<int>(Management::local_player.basePosition + 0x28C8) != 0)
							{
								thing = true;
								break;
							}

							Sleep(1);
						}

						if (!thing)
						{
							waitBeforeWalk = cur_time + 50;
							waitBeforeAttack = 0;
						}
					}
				}
			}
		}
	}
	//else
	//{
	//	int diff = waitBeforeWalk - GetTickCount();
	//	if (diff >= 0)
	//		Sleep(diff);

	//	if (Management::Manager.Read<int>(Management::local_player.basePosition + 0x28C8) == 0 && one_time)
	//	{


	//		waitBeforeWalk = 0;
	//		waitBeforeAttack = 0;

	//		one_time = false;
	//	}
	//}
}


void Orbwalker::Update()
{
	if (GetAsyncKeyState(4) & 1)
	{
		CObject under_mouse_object = CObject::GetUnderMouseObject();
		if (isHero(under_mouse_object.basePosition))
		{
			if (under_mouse_object.team != CObject::GetLocalPlayer().team)
			{
				Management::current_target = under_mouse_object;
			}
			else
			{
				Management::current_target.network_id = 0;
				Management::current_target.basePosition = 0;
			}

		}
		else
		{
			Management::current_target.network_id = 0;
			Management::current_target.basePosition = 0;
		}

	}


	if (GetAsyncKeyState(harras_key) || GetAsyncKeyState(combo_key) || GetAsyncKeyState(last_hit_key))
	{

		Management::local_player = CObject::GetLocalPlayer();
		Management::under_mouse_object = CObject::GetUnderMouseObject();


		if (GetAsyncKeyState(combo_key))
		{
			ReadHeroes();

			Combo();
		}
		else if (GetAsyncKeyState(last_hit_key))
		{
			ReadEntities(false);

			LastHit();
		}
		else if (GetAsyncKeyState(harras_key))
		{
			ReadEntities(false);

			LaneClear();
		}

	}


	Management::wait_for_usage = false;
}


int Orbwalker::harras_key = 'V';
int Orbwalker::combo_key = ' ';
int Orbwalker::last_hit_key = 'X';
float Orbwalker::adjust_for_attack_cast_delay = 100;
float Orbwalker::adjust_for_attack_delay = 100;
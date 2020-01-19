
close_to_die = CObject()

close_to_die.network_id = 0

function Loop()

  if IsKeyPressed(string.byte(" ")) then
  
    read_my_hero()
    read_heroes()
    
    Combo()
  
  elseif IsKeyPressed(string.byte("V")) then
  
    read_my_hero()
    read_entities()
    
    LaneClear()
  
  elseif IsKeyPressed(string.byte("X")) then
  
    read_my_hero()
    read_entities()
    
    LastHit()
  
  end
end






attack_cast_delay = 0
attack_delay = 0

cur_time = 0

function Combo()
  
  cur_time = now()
  
  if cur_time >= attack_cast_delay then
  
    if underMouseObject.network_id ~= 0 then
      
      if underMouseObject.team == myHero.team or underMouseObject:IsTargetable() == false then
        
        RightClick()
        
      end
      
    else
      
      RightClick()
      
    end
  
    if cur_time >= attack_delay then
    
      local target = CObject()
      target.network_id = 0

      local infield_entities = GetEntitiesInField(all_champions, myHero.position, myHero.attack_range + 100)
    
      local attackable_entities = GetOnlyAttackable(infield_entities)
    
      local few_hp_entities = GetDueToHealthPercent(attackable_entities)
      
      if(few_hp_entities:size() > 0) then
        target = GetTargetDueToPriority(few_hp_entities)
      else
        target = GetTargetDueToPriority(attackable_entities);
      end
            
      if(target.network_id == 0) then
        
        target = GetClosestOneInArray(attackable_entities)
        
      end
    
      if target.network_id ~= 0 then
            
        local screen_pos = W2S(target.position)
      
        ScriptClick(screen_pos.x, screen_pos.y)
      
        attack_delay = cur_time + myHero:GetAttackDelay() * attack_delay_multiplier * 10 + GetPing() / 2
        attack_cast_delay = cur_time + myHero:GetAttackCastDelay() * attack_cast_delay_multiplier * 10 + GetPing() / 2
      
      
        local thing = false
        for i = attack_cast_delay - cur_time, 0, -1 do
          
          if myHero:IsAttacking() then
            
            thing = true
            break
            
          end
          
          sleep(1)
        end
        
        if thing == false then
          
          attack_cast_delay = cur_time + 25
          attack_delay = 0
          
        end
      
      end
    
    end
  end
end


function LaneClear()
  
  
  
  cur_time = now()
  
  if cur_time >= attack_cast_delay then
  
    if underMouseObject.network_id ~= 0 then
      
      if underMouseObject.team == myHero.team or underMouseObject:IsTargetable() == false then
        
        RightClick()
        
      end
      
    else
      
      RightClick()
      
    end
    
    
  
    if cur_time >= attack_delay then
    
      local target = CObject()
      target.network_id = 0

      local infield_entities = GetEntitiesInField(all_mobs, myHero.position, myHero.attack_range + 100)
    
      local attackable_entities = GetOnlyAttackable(infield_entities)


      target = GetKillableInArray(attackable_entities, myHero.base_attack + myHero.bonus_attack)
      
      if target.network_id == 0 and close_to_die.network_id ~= 0 then
        
      else

      
        

      
      if(target.network_id == 0) then
        
        target = GetHighestHealthInArray(attackable_entities);
        
      end
            
      if(target.network_id == 0) then
        
        target = GetClosestOneInArray(attackable_entities)
        
      end
    
      if target.network_id ~= 0 then
            
        local screen_pos = W2S(target.position)
      
        ScriptClick(screen_pos.x, screen_pos.y)
      
        
      
        attack_delay = cur_time + myHero:GetAttackDelay() * attack_delay_multiplier * 10 + GetPing() / 2
        attack_cast_delay = cur_time + myHero:GetAttackCastDelay() * attack_cast_delay_multiplier * 10 + GetPing() / 2
      
      
        local thing = false
        for i = (attack_cast_delay - cur_time), 0, -1 do
          
          if myHero:IsAttacking() then
            
            thing = true
            break
            
          end
          sleep(1)
        end
        
        if thing == false then
          
          attack_cast_delay = cur_time + 25
          attack_delay = 0
          
        end
      
      end
    end
    end
  end
end


function LastHit()
  
  cur_time = now()
  
  if cur_time >= attack_cast_delay then
  
    if underMouseObject.network_id ~= 0 then
      
      if underMouseObject.team == myHero.team or underMouseObject:IsTargetable() == false then
        
        RightClick()
        
      end
      
    else
      
      RightClick()
      
    end
    
    
  
    if cur_time >= attack_delay then
    
      local target = CObject()
      target.network_id = 0

      local infield_entities = GetEntitiesInField(all_mobs, myHero.position, myHero.attack_range + 100)
    
      local attackable_entities = GetOnlyAttackable(infield_entities)
    
      target = GetKillableInArray(attackable_entities, myHero.base_attack + myHero.bonus_attack)

      if target.network_id ~= 0 then
            
        local screen_pos = W2S(target.position)
      
        ScriptClick(screen_pos.x, screen_pos.y)
      
        attack_delay = cur_time + myHero:GetAttackDelay() * attack_delay_multiplier * 10 + GetPing() / 2
        attack_cast_delay = cur_time + myHero:GetAttackCastDelay() * attack_cast_delay_multiplier * 10 + GetPing() / 2
      
      
        local thing = false
        for i = attack_cast_delay - cur_time, 0, -1 do
          
          if myHero:IsAttacking() then
            
            thing = true
            break
            
          end
          
          sleep(1)
        end
        
        if thing == false then
          
          attack_cast_delay = cur_time + 50
          attack_delay = 0
          
        end
      
      end
    
    end
  end
end

function CalculateDmgToObj(obj, Damage) 
	local myObj = myHero

	local EffectiveHealth = obj.predicted_health
  
  if obj.predicted_after_time > now() then
    EffectiveHealth = obj.health
  end
  
	local resists = 0

	local ourDamage = Damage

	resists = obj.armor

	EffectiveHealth = EffectiveHealth * (1.0 + (resists / 100.0))

	return EffectiveHealth - ourDamage
end

function GetHighestHealthInArray(list)

	local mMax = -99999
  
  local entity = CObject()
	entity.network_id = 0

	for i = 0, list:size()-1, 1 do
	
		if (list:at(i).health > mMax) then
			mMax = list:at(i).health
			entity = list:at(i)
		end
  end
	

	return entity
end

function GetHighestMaxHealthInArray(list_cur)

	local mMax = -99999

	local entitya = CObject()
  entitya.network_id = 0

	for i = 0, list_cur:size()-1, 1 do
	
		if list_cur:at(i).max_health > mMax then
			mMax = list_cur:at(i).max_health
			entitya = list_cur:at(i)
		end
    
  end
	

	return entitya
end

function GetKillableInArray(list, damage)
  
  local len = list:size() - 1

  local killable_list_loc = CObjectList()
  killable_list_loc:clear()
  
  log("test-1")

  PredictHealth(list)
    log("test-2")
close_to_die = CObject()
  close_to_die.network_id = 0
    log("test-3")
    
  for i = 0, len, 1 do
    
    local entityloc = list:at(i)
    log("test-4")
    local last_health = CalculateDmgToObj(entityloc, damage)
    log("test-5")
    if( last_health <= 0 ) then
      killable_list_loc:push(entityloc)
    end
    log("test-6")
    if(last_health > 0 and last_health <= damage * 3) then
      close_to_die = entityloc
    end
    log("test-7")
    if close_to_die.network_id == 0 then
      
      if(last_health > 0 and last_health <= damage * 4 and entityloc.health ~= entityloc.predicted_health) then
        close_to_die = entityloc
      end
      
    end
    log("test-8")
  end
  log("test-9")
  for i = 0, killable_list_loc:size()-1 do
    
    if killable_list_loc:at(i).health ~= killable_list_loc:at(i).predicted_health then
      return killable_list_loc:at(i)
    end
    
  end
  log("test-10")
  return GetHighestMaxHealthInArray(killable_list_loc)

end



function GetEntitiesInField(list, position, radius)
  
  local new_list = CObjectList()
  
  local len = list:size() - 1

  for i = 0, len, 1 do
    
    local entity = list:at(i)
 
    local dist = DistanceBetween(position, entity.position)
  
    if dist <= radius and entity:IsValid() then
            
      new_list:push(entity)     
      
    end
    
  end
  
  
  return new_list

end


function GetOnlyAttackable(list)
  
  local new_list = CObjectList()
  
  local len = list:size() - 1

  for i = 0, len, 1 do
    
    local entity = list:at(i)
    
    if entity.team ~= myHero.team and entity.health > 0 and entity.max_health > 5 and entity:IsVisible() and entity:IsValid() then
            
      new_list:push(entity)     
      
    end
    
  end
  
  
  return new_list

end





function GetDueToHealthPercent(list)
  
  local new_list = CObjectList()
  
  local len = list:size() - 1

  for i = 0, len, 1 do
    
    local entity = list:at(i)
    
    local percent = (entity.health / entity.max_health) * 100;
    
    if percent <= attack_under_hp_percent then
            
      new_list:push(entity)     
      
    end

  end
  
  
  return new_list

end


function GetTargetDueToPriority(list)
  
  local min = 999
  local index = -1

  local len = list:size() - 1

  
  
  for i = 0, len, 1 do
    
    local entity = list:at(i)
    
    if priority_list:find( entity.network_id ) ~= priority_list:endv() then
      
      if priority_list:at( entity.network_id ) < min then
        
        min = priority_list:at( entity.network_id )
        index = i
        
      end
      
    end
    
  end
  
  if index ~= -1 then
    return list:at(index)
  end
  
  entity = CObject()
  entity.network_id = 0
  
  return entity

end



function GetClosestOneInArray(list)
  
  local glob_entity = CObject()
  glob_entity.network_id = 0
  local max = 99999999

  local len = list:size() - 1

  for i = 0, len, 1 do
    
    local entity = list:at(i)
    
    local dist = DistanceBetween(myHero.position, entity.position)
    
    if dist < max then
      
      max = dist
      glob_entity = entity
      
    end 
    
  end
  

  
  return glob_entity

end


function DistanceBetween(first, second)
  
  local diff = Vec3 ( ( second.x - first.x ), ( second.y - first.y ), ( second.z - first.z ) )
  
  local dist = diff.x * diff.x + diff.z * diff.z
  
  return math.sqrt(dist)
end


function find_in_vector(list, to_find)
  
  local len = list:size() - 1
  
  for i = 0, len do
    
    if list:at(i).source_index == to_find.source_index then
      return i
    end
    
  end
  
  
  return -1
  
end


function PredictHealth(to_edit_list)
  
  local list = all_missiles
  
    for i = 0, list:size() - 1 do
    
      local missile = list:at(i)
      
      caster = GetObjectByIndex(missile.source_index)
      target = GetObjectByIndex(missile.target_id)
      
      if caster.source_index >= 0 and caster.source_index <= 10000 and target.source_index >= 0 and target.source_index <= 10000 then
      
      index = find_in_vector(to_edit_list, target)
      
      if index ~= -1 then

        local damage = caster.base_attack + caster.bonus_attack

        to_edit_list:at(index).predicted_health = to_edit_list:at(index).health - damage
        to_edit_list:at(index).predicted_after_time = now() + DistanceBetween(missile.position, missile.end_pos) * 0.5
      end
      
      end
      
    end

    

end


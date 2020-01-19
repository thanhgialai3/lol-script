
-- Example that draws last-hittable minion and attack range for your hero



function Loop()
  
  read_my_hero()
  read_entities()
  --read_heroes()

end


function OnDraw()
  
  
  local list = all_mobs
  
  if list == nil then return end
  
  local length = list:size()
  
  for i = 0, length - 1, 1
  do
     local obj = list:at(i)
    
                      
     if obj.team ~= myHero.team and obj:IsValid() and obj.health <= myHero.base_attack then
     
        DrawCircle(obj.position, 6, obj.attack_range + 100)
     
     end
     
  end

  DrawCircle(myHero.position, 24, myHero.attack_range+100)
  
end




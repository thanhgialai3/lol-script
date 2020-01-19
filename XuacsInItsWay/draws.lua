
function Loop()
  
  read_my_hero()

  
end

function OnDraw()
  
  read_my_hero()
  
  DrawCircle( myHero.position, 24, myHero.attack_range + 100 )
end

include "test/scripting/check_game_end.lua"

local winner = 1

run(function()
  sleep(8000)

  -- conquer the artifacts
   local n_artifacts = 0
   local map = wl.Game().map
   for x=0, map.width-1 do
      for y=0, map.height-1 do
         local field = map:get_field(x,y)
         if field.immovable and field.immovable:has_attribute("artifact") and
               field.immovable.fields[1] == field then
            n_artifacts = n_artifacts + 1
            sleep(2000)
            print(string.bformat("Conquering artifact at %d,%d for the winner.", field.x, field.y))
            game.players[winner]:conquer(field)
         end
      end
   end

   if n_artifacts == 0 then
      sleep(2000)
      assert(false, "No artifacts on the map. The win condition artifacts will not end.")
   else
      print("All artifacts are conquered for the winner, win condition should be triggered soon.")
   end
end)

check_win_condition(winner)

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
            local for_player = game.players[winner]
            if n_artifacts > 2 then
               for_player = game.players[n_artifacts - 2]
            end
            print(string.bformat("Conquering artifact at %d,%d for %s.", field.x, field.y, tostring(for_player)))
            for_player:conquer(field)
         end
      end
   end

   if n_artifacts == 0 then
      sleep(2000)
      assert(false, "No artifacts on the map. The win condition artifacts will not end.")
   else
      print(string.format(
         "All %d artifacts are conquered, win condition should be triggered soon.", n_artifacts))
   end
end)

check_win_condition(winner)

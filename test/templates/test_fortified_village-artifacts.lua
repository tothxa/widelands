include "test/scripting/check_game_end.lua"

local winner = 1
local artifacts = {
  { 68, 29 },
  { 85, 41 },
  { 40, 72 },
  { 36, 82 },
}

run(function()
  sleep(8000)

  -- conquer the artifacts
  for i,coords in pairs(artifacts) do
    sleep(2000)
    print(string.bformat("Conquering artifact at %d,%d for the winner.", coords[1], coords[2]))
    game.players[winner]:conquer(game.map:get_field(coords[1],coords[2]), 3)
  end

  print("All artifacts are conquered for the winner, win condition should be triggered soon.")
end)

check_win_condition(winner)

local tc = lunit.TestCase("Fortiefied village")
function tc.test_no_of_buildings()
   check_min_no_of_buildings(6)
end

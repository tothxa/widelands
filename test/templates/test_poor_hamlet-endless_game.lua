include "scripting/coroutine.lua"
include "test/scripting/check_game_end.lua" -- for general checks and check_min_no_of_buildings()

local tc = lunit.TestCase("Poor hamlet")

function tc.test_random_player()
   local game = wl.Game()
   local r_player = game.players[#game.players]
   assert_not_nil(r_player)
   check_min_no_of_buildings(1)
   local no_same_tribe = 0
   for _i, plr in pairs(game.players) do
      if plr ~= r_player and plr.tribe_name == r_player.tribe_name then
         no_same_tribe = no_same_tribe + 1
         -- check that it is the same start condition (10s are to short to consume wares)
         for _j, descr in pairs(plr.tribe.wares) do
            assert_equal(plr:get_wares(descr.name), r_player:get_wares(descr.name),
                         "ware "..descr.name)
         end
         for _j, descr in pairs(plr.tribe.wares) do
            assert_equal(plr:get_workers(descr.name), r_player:get_workers(descr.name),
                         "worker "..descr.name)
         end
         local p_im_d = game.map.player_slots[plr.number].starting_field.immovable.descr
         local r_im_d = game.map.player_slots[r_player.number].starting_field.immovable.descr
         assert_equal(p_im_d.name, r_im_d.name, "start building")
      end
   end
   assert_equal(1, no_same_tribe)
end

run(function()
  sleep(100)
  wl.Game().desired_speed = 5000
  sleep(10000)
  -- no check of winner, is endless
  lunit.run()
  wl.ui.MapView():close()
end)

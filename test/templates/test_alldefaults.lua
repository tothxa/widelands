include "scripting/coroutine.lua"
include "test/scripting/lunit.lua"

local game = wl.Game()

local tc = lunit.TestCase("Some defaults")

function tc.test_game()
   assert_true(game.allow_diplomacy, "allow_diplomacy")
   assert_equal("singleplayer", game.type)
   assert_equal(1, game.interactive_player, "interactive_player")
   -- assert_equal("Endless Game", game.win_condition)
end

function tc.test_players()
   assert_equal(#game.map.player_slots, #game.players, "all players are set")
   for _idx, player in pairs(game.players) do
      -- tribe is taken from the map
      local slot = game.map.player_slots[player.number]
      if slot.tribe_name ~= "random" then
         assert_equal(slot.tribe_name, player.tribe.name, "tribe from map")
      end
      -- starting position is taken from the map
      assert_not_nil(slot.starting_field.immovable, "built at default start position")
      assert_equal("warehouse", slot.starting_field.immovable.descr.type_name)
      assert_equal(player, slot.starting_field.immovable.owner, "the expected player")
      -- some other things
      assert_false(player.see_all, "should not not see all")
   end
end

run(function()
  wl.Game().desired_speed = 5000

  sleep(10000)
  lunit.run()
  wl.ui.MapView():close()
end)

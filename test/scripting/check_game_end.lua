include "scripting/coroutine.lua"
include "test/scripting/lunit.lua"

game = wl.Game() -- global is used by tests
local mapview = wl.ui.MapView()

local function result_table(winners)
  local r = {}

  for i = 1, #game.players do
    r[i] = 0
  end

  if type(winners) == "number" then
    r[winners] = 1
  else
    for i,w in ipairs(winners) do
      r[w] = 1
    end
  end

  return r
end

local time_fmt = "%02d:%06.3f"
local function formatted_time()
  local t = game.time / 1000
  local m = math.floor(t / 60)
  local s = t % 60
  return time_fmt:bformat(m, s)
end

-- 2 minutes grace period
local timeout = (game.win_condition_duration + 2) * 60 * 1000
local check_interval = 2000
local pause_timeout = 2 * 60  -- 2 minutes

local last_gametime = 0
local pause_counter = 0

local expected = nil

function check_game_ended() -- global, to be callable from callback in add_plugin_timer()
  if (game.time > last_gametime) then
    last_gametime = game.time
    pause_counter = 0
  else
    pause_counter = pause_counter + check_interval / 1000
    print(string.bformat("No progress since last check. (%d seconds)", pause_counter))
  end
  local game_ended = mapview.windows.game_summary ~= nil
  print("Checking end of game at " .. formatted_time())
  if game_ended or game.time > timeout or pause_counter >= pause_timeout then
      local test_case = lunit.TestCase("End result")
      -- Check timeout
      function test_case.test_in_time()
         assert_true(game_ended, "Game did not end in time")
      end
      if game_ended then
         test_case.test_result = check_result
      end
      lunit.run()

      mapview:close()
   end
end

function check_result()
-- TODO local, move above

    -- Check that the expected player(s) won
    local failed = {}
    if expected ~= nil then
      for i = 1, #game.players do
        if(expected[i] ~= game.players[i].end_result) then
          table.insert(failed,game.players[i].name)
        end
      end
    end
    assert_true(#failed == 0, "Wrong result for: "..table.concat(failed, ", "))
end

local tc_init = lunit.TestCase("Check init")
function tc_init.test_all_tribes() -- runs at end
   local ALL_TRIBES = 5
   local tribes = {}
   for _i, plr in pairs(game.players) do
      tribes[plr.tribe_name] = 1
   end
   local n_tribes = 0
   for _name in pairs(tribes) do
      n_tribes = n_tribes + 1
   end
   if ALL_TRIBES == n_tribes then
      assert_true(true) -- is checked
   else
      local msg = ""
      for name in pairs(tribes) do
         msg = msg..name.." "
      end
      assert_equal(5, n_tribes, "Tribes in game: "..msg)
   end
end

function check_win_condition(winners) -- global, to use for tests
  run(function()
    sleep(1000)

    -- Set up expected winners table
    if (winners == nil) then
      print("### WARNING: check_win_condition():")
      print("###    For better test coverage, please call with list of expected winners ###")
    else
      expected = result_table(winners)
    end

    -- Schedule check
    mapview:add_plugin_timer("check_game_ended()", check_interval, false)

    game.desired_speed = 100000
  end)
end

function check_min_no_of_buildings(min_no) -- gobal for use by tests
   for _i, plr in pairs(game.players) do
      local count_all = 0
      local houses = plr:get_buildings("all")
      for _bld_name, blds in pairs(houses) do
         count_all = count_all + #blds
      end
      assert_true(count_all >= min_no, count_all.." buildings for "..plr.tribe_name)
   end
end

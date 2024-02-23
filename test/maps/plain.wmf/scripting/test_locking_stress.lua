include "scripting/messages.lua"

game.desired_speed = 1000

prefilled_buildings(
  p1,
   { "barbarians_wood_hardener", 15, 10 },
   { "barbarians_lime_kiln", 20, 10 },
   { "barbarians_micro_brewery", 25, 10 },
   { "barbarians_barrier", 10, 15 },
   { "barbarians_battlearena", 15, 15 },
   { "barbarians_warehouse", 20, 15 },
   { "barbarians_inn", 25, 15 },
   { "barbarians_rangers_hut", 10, 20 },
   { "barbarians_gamekeepers_hut", 15, 20 },
   { "barbarians_reed_yard", 20, 20 },
   { "barbarians_well", 25, 20 },
   { "barbarians_sentry", 10, 25 },
   { "barbarians_hunters_hut", 15, 25 }
)
pl_buildings = {}
for n,b in pairs(p1:get_buildings("all")) do
   table.insert(pl_buildings, b[1])
end

free_field = map:get_field(5, 15)

function replace_building()
   local sel = math.random(#pl_buildings)
   local b = pl_buildings[sel]
   if b.descr.name == "barbarians_headquarters" then
      return
   end

   pl_buildings[sel] =
      prefilled_buildings(p1, { pl_buildings[sel].descr.name, free_field.x, free_field.y })[1]
   free_field = b.fields[1]
   if b.descr.type_name == "militarysite" or b.descr.type_name == "trainingsite" then
      b:set_soldiers({})
   else
      b:set_workers({})
   end
   b:remove()
end

mv = wl.ui.MapView()

open_counter = 1
close_counter = 1
start_stop_counter = 1
set_inputs_counter = 1

function open_window()
   open_counter = open_counter % #pl_buildings + 1
   local b = pl_buildings[open_counter]
   local bf = b.fields[1]
   mv:click(bf)
end

function get_window(i)
   local j = 1
   for n,w in pairs (mv.windows) do
      if (j == i) then
         return w
      end
      j = j + 1
   end
   return nil
end

function close_window()
   local w = get_window(close_counter)
   if w ~= nil then
      w:close()
      close_counter = close_counter + 2
   else
      close_counter = 1
   end
end

function start_stop_building()
   while mv.windows ~= nil do
      local w = get_window(start_stop_counter)
      if w ~= nil then
         if w.buttons.stop ~= nil then
            w.buttons.stop:click()
         else
            if w.buttons.continue ~= nil then
               w.buttons.continue:click()
            end
         end
         start_stop_counter = start_stop_counter + 1
      else
         start_stop_counter = 1
         return
      end
   end
end

function change_inputs()
   local b = nil
   local inputs = nil
   local wares = {}
   repeat
      set_inputs_counter = set_inputs_counter % #pl_buildings + 1
      b = pl_buildings[set_inputs_counter]
      inputs = b.valid_inputs
      wares = {}
      if inputs ~= nil then
         for k,v in pairs(inputs) do
            table.insert(wares, k)
         end
      end
   until #wares > 0

   local ware = wares[math.random(#wares)]

   if b:get_desired_fill(ware) < inputs[ware] then
      b:set_desired_fill(ware, inputs[ware])
   else
      b:set_desired_fill(ware, 0)
   end
end

test_duration = 200000  -- gametime

-- ### End of helper functions, now the fun starts ### --

mv:add_plugin_timer("open_window()", 511)

-- run(function()
--    while true do
--       sleep(math.random(400, 1600))
--       open_window()
--    end
-- end)

mv:add_plugin_timer("close_window()", 1003)

-- run(function()
--    while true do
--       sleep(math.random(600, 1500))
--       close_window()
--    end
-- end)

-- mv:add_plugin_timer("replace_building()", 1319)

run(function()
   while true do
      sleep(math.random(1000, 3000))
      replace_building()
   end
end)

run(function()
   sleep(math.random(500, 1500))
   mv:add_plugin_timer("start_stop_building()", 257)

--    while true do
--       sleep(math.random(200, 1000))
--       start_stop_building()
--    end
end)

run(function()
   sleep(math.random(500, 1500))
--    mv:add_plugin_timer("change_inputs()", 263)

   while true do
      sleep(math.random(200, 1000))
      change_inputs()
   end
end)

run(function()
   sleep(test_duration)
   game.desired_speed = 200
   open_window = function() end
   replace_building = function() end
   close_window = function() end
   start_stop_building = function() end
   change_inputs = function() end
   sleep(2000)
   print("# All Tests passed.")
   mv:close()
end)

game.desired_speed = 20000

push_textdomain("tribes")

local dirname = path.dirname (__file__)

wl.Descriptions():new_productionsite_type {
   name = "amazons_rare_tree_kiln",
   -- TRANSLATORS: This is a building name used in lists of buildings
   descname = pgettext ("amazons_building", "Rare Tree Kiln"),
   icon = dirname .. "menu.png",
   size = "medium",

   animation_directory = dirname,
   animations = {
      unoccupied = {hotspot = {43, 44}},
      idle = {hotspot = {43, 44}},
   },
   spritesheets = {
      working = {
         hotspot = {43, 45},
         fps = 15,
         frames = 30,
         columns = 6,
         rows = 5
      }
   },

   aihints = {
   },

   working_positions = {
      amazons_charcoal_master = 1
   },

   inputs = {
      { name = "ironwood", amount = 3 },
      { name = "balsa", amount = 3 },
      { name = "rubber", amount = 3 },
      { name = "granite", amount = 3 },
   },

   programs = {
      main = {
         -- TRANSLATORS: Completed/Skipped/Did not start producing charcoal because ...
         descname = _("producing charcoal"),
         actions = {
            "return=skipped unless economy needs coal",
            "consume=ironwood balsa rubber granite",
            -- Similar to Frisian charcoal kiln, but a little more preparation time
            "sleep=duration:40s",
            "animate=working duration:30s",
            "produce=coal"
         }
      },
   },
}

pop_textdomain()

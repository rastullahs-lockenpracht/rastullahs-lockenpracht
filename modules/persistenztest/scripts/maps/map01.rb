require 'embed.rb'
require 'MapChange.rb'

base = $AM.createBoxPrimitiveActor("SelectorTestBase", [-2,0.05,-2], [2,0.3,2], "alpha_blue")

base.placeIntoScene([8,0,8])

CreateSceneChangeTrigger("map1", [8,0,8], [0,0,0,1], [2, 2, 2], "scene02")

puts 'Map 1 init script wurde ausgeführt!'

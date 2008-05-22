require 'embed.rb'
require 'MapChange.rb'

base = $AM.createBoxPrimitiveActor("SelectorTestBase", [-5,0.05,-5], [5,0.3,5], "alpha_blue")

base.placeIntoScene([8,0,8])

puts CreateMapChangeTrigger("map1", [8,0,8], [0,0,0,1], [2, 2, 2], ["persistenztest02.rlmap.xml","hero.rlmap.xml"])

#puts 'Map 1 init script wurde ausgeführt!'

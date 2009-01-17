require 'embed.rb'
require 'SceneChange.rb'

$SCRIPT.log("map 1 von 'persistenztest' wird initialisiert....")

base = $AM.createBoxPrimitiveActor("SelectorTestBase", [-2,0.05,-2], [2,0.3,2], "nx.cube")

base.placeIntoScene([8,0,8])

CreateSceneChangeTrigger("map1", [8,0,8], [0,0,0,1], [2, 2, 2], "scene02")

puts 'Map 1 init script wurde ausgeführt!'

$SCRIPT.log("map 1 von 'persistenztest' initialisiert.")

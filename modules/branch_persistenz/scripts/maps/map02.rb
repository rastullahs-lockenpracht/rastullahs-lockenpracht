require 'embed.rb'
require 'SceneChange.rb'

$SCRIPT.log("map 2 von 'persistenztest' wird initialisiert....")

base = $AM.createBoxPrimitiveActor("SelectorTestBase", [-2,0.05,-2], [2,0.3,2], "nx.cube")

base.placeIntoScene([8,0,-8])

CreateSceneChangeTrigger("map2", [8,0,-8], [0,0,0,1], [2, 2, 2], "scene01")

puts 'Map 2 ausgeführt'

$SCRIPT.log("map 2 von 'persistenztest' initialisiert.")
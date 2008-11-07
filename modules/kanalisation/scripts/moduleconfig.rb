include RlScript

class KanalisationModule < ContentModule
    def initialize()
        super("kanalisation", "Kanalisation", false, 200608012)
    end

    def getDependencies()
        return ["common"];
    end

    def getTextureLocations()
	return ["textures"];
    end

    def getModelLocations()
	return ["Model"]
    end

    def getSoundLocations()
	return ["Sound"]
    end

    def start()
		require 'player.rb'
		require 'mckhero.rb'
		require 'torch.rb'
        #$CORE.loadMap("Octree", "kanalisation.scene", "kanalisation");
		#loader = MapLoader.new("kanalisation");
		#loader.loadMap("kana_lights.rlmap.xml");
		#loader.loadMap("kanalisation.rlmap.xml");
		SceneManager.getSingleton().loadScene("kanalisation");
        load "kanalisation.rb"
        $World = $CORE.getWorld();
    end
end

CoreSubsystem.getSingleton().registerModule(KanalisationModule.new());

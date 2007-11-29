include RlScript

class TechdemoModule < ContentModule
    def initialize()
        super("techdemo", "Techdemo", false, 200603030);
    end

    def getDependencies()
        return ["common"];
    end

    def getTextureLocations()
	return ["alpha", "env", "model", "tiled"];
    end

    def getSoundLocations()
	return ["ost"];
    end

    def getModelLocations()
	return []
    end

    def start()
		require 'player.rb'
		require 'mckhero.rb'
		require 'npc.rb'
		require 'torch.rb'
		require "techdemo_umgebung.rb"
		require "techdemo_quests.rb"
		require "techdemo_spinne.rb"
		require "techdemo_hoehle.rb"
		require "truhe.rb"
		require "techdemo_hoehle_eingang.rb"

        p "Loading map with MapLoader"
        
        loader = MapLoader.new("techdemo")
        
        loader.loadMap("techdemo_terrain.rlmap.xml");
        #loader.loadMap("techdemo_all_except_terrain.rlmap.xml");
        p "Done loading map"
        
        load "techdemo.rb"
        $World = $CORE.getWorld();
    end
end

CoreSubsystem.getSingleton().registerModule(TechdemoModule.new())


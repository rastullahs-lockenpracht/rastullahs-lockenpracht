require 'globals.rb'

class Crate < GameObject
  def initialize(name, description)
    super(name, description);

    crateActor = $AM.createMeshActor( "crate", "kiste.mesh",  PhysicsManager::GT_BOX, 220.0 );
    $SCRIPT.log("crate.rb - Aktor erstellt.");
    setActor(crateActor);
    $SCRIPT.log("crate.rb - Aktor gesetzt");
  end
end


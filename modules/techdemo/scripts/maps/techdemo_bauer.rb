load( "embed.rb" );
load( "npc.rb" );

class BauerArnolfGrossschaedel < Person
  def initialize
    super("ArnolfGrossschaedel", "Ein Bauer")
    setActor($AM.createMeshActor("ArnolfGrossschaedel", "men_wirt_posed.mesh", 4, 0.0))
    addAction(TalkAction.new("bauer.xml"))
  end
end
class TalkAction < Action
  def initialize(dialog)
    super("talk", "Reden")
    @mDialog = dialog
  end

  def doAction(object, actor, target)
    $UI.showDialog($DS.loadBot(@mDialog))
  end
end

class BauerArnolfGrossschaedel < Person
  def initialize
    super("ArnolfGrossschaedel", "Ein Bauer")
#TODO: Mesh-Objekte aus Modulen laden, dann
#    setActor($AM.createMeshActor("ArnolfGrossschaedel", "men_wirt_posed.mesh"))
    setActor($AM.createMeshActor("ArnolfGrossschaedel", "held.mesh"))
    
    addAction(TalkAction.new("bauer.xml"))
  end
end
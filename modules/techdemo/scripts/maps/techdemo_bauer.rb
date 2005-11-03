class BauerArnolfGrossschaedel < Person
  def initialize
    super("ArnolfGrossschaedel", "Ein Bauer")
#TODO: Mesh-Objekte aus Modulen laden, dann
#    setActor($AM.createMeshActor("ArnolfGrossschaedel", "men_wirt_posed.mesh"))
    setActor($AM.createMeshActor("ArnolfGrossschaedel", "held.mesh"))
  end
end
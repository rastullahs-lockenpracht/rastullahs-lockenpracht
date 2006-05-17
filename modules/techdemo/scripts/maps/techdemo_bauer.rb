class TalkAction < Action
  def initialize(dialog)
    super("talk", "Reden")
    @mDialog = dialog
  end

  def doAction(object, actor, target)
    bot = $DS.loadBot(@mDialog)
    bot.setDialogCharacter(actor)
    bot.setDialogPartner(object)
    WindowFactory.getSingleton().showDialog(bot)
  end
end

class BauerArnolfGrossschaedel < Person
  def initialize
    super("ArnolfGrossschaedel", "Ein Bauer")
    setActor($AM.createMeshActor("ArnolfGrossschaedel", "men_wirt_posed.mesh", 4, 0.0))
    addAction(TalkAction.new("bauer.xml"))
  end
end
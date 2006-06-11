load( "embed.rb" );

class TalkAction < Action
  def initialize(name, dialog)
    super("talk", "Reden")
    @mDialog = dialog
    @mName = name
  end

  def doAction(object, actor, target)
    bot = $DS.loadBot(@mName, @mDialog)
    bot.setNonPlayerCharacter( object );
    bot.setPlayerCharacter( actor );
    WindowFactory.getSingleton().showDialog(bot)
  end
end

load( "embed.rb" );

class TalkAction < Action
  def initialize()
    super("talk", "Reden")
  end

  def doAction(object, actor, target)
    bot = $DS.getBot(object.getName())
    if (bot.nil?)
      bot = $DS.loadBot(object.getName(), object.getDialogfile())
    end
    if ( not bot.nil? )
      bot.setPlayerCharacter( actor );
      bot.setNonPlayerCharacter( object );
      WindowFactory.getSingleton().showDialog(bot);
    end
  end
end

module TalkTarget
  @mDialogfile;

  def initialize(id)
    super(id)
    addAction(TalkAction.new())
  end

  def setProperty(key, value)
    if (key == "dialogfile")
      @mDialogfile = value;
    else
      super(key, value)
    end
  end

  def getDialogfile()
    return @mDialogfile;
  end
end

class NPC < Person
  include TalkTarget
end
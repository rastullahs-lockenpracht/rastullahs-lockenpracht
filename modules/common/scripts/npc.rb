load( "embed.rb" );

class TalkAction < Action
  def initialize(name, dialog)
    super("talk", "Reden")
    @mDialog = dialog
    @mName = name
  end

  def doAction(object, actor, target)
	if (@mBot == nil)
		@mBot = $DS.getBot(@mName)
		if (@mBot == nil )
			@mBot = $DS.loadBot(@mName, @mDialog)
		end

	end
	if (@mBot != nil)
		@mBot.setPlayerCharacter( actor );
		@mBot.setNonPlayerCharacter( object );
		WindowFactory.getSingleton().showDialog(@mBot);
	end
  end
end

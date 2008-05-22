class MessageBox < MessageWindow
	
	def initialize(title, text, buttons, actions)
		super(title, text, buttons)
		@mActions = actions;
	end
	
	def onButtonClicked(button, text)
		setVisible(false)
		#CoreSubsystem.getSingleton().setPaused(false)
		if @mActions != nil && button < @mActions.length && @mActions[button] != nil
			@mActions[button].call()
		end
		return true
	end
	
	def MessageBox.showModal(text, title = "", buttons = MessageBox::OK, *actions)
	   MessageBox.new(title, text, buttons, actions).setVisible(true)
	   #CoreSubsystem.getSingleton().setPaused(true)
	end	
end
load "embed.rb"

class DefaultWanderBehaviour < SteeringBehaviour
	def initialize()
		super();
		init();
	end
	
	def getType()
		return "wander"
	end
	
	def init()
		print("Initialize WanderBehaviour");
		setActivationLevel(1.0);
	end
	
	def activate()
		$SCRIPT.log("Activate Wandering");
	end
		
	def update(elapsedTime)
		if(getController().needAvoidance(5.0))
			getController().addForce(getController().calcAvoidNeighbors(5.0) * getActivationLevel());
			getController().addForce(getController().calcAvoidObstacles(5.0) * getActivationLevel());
		else
			getController().addForce(getController().calcWander(elapsedTime) * getActivationLevel());
		end	
	end
	
	def deactivate()
	end
	
	def calculateActivation()
		setActivationLevel(1.0);
		if(getController().needAvoidance(5.0))
			setActivationLevel(getActivationLevel()*5.0);
		end
		return getActivationLevel();
	end
	

end

class DefaultIdleBehaviour < SteeringBehaviour
	def initialize()
		super();
		init();
	end
	
	def getType()
		return "idle"
	end
	
	def init()
		print("Initialize IdleBehaviour");
		setActivationLevel(1.0);
	end
	
	def activate()
		$SCRIPT.log("Activate Idle");
	end
		
	def update(elapsedTime)
		#getController().addForce([0.0, 0.0, 0.0]);
		
	end
	
	def deactivate()
	end
	
	def calculateActivation()
		if(getController().isDialogActive())
			setActivationLevel(getActivationLevel()*0.0);
		else
			setActivationLevel(1.0);
		end
		if(getController().needAvoidance(5.0))
			setActivationLevel(getActivationLevel()*0.0);
		end
		return getActivationLevel();
	end
	

end



class DialogBehaviour < SteeringBehaviour
	def initialize(playerAgent)
		super();
		init();
		@mPlayer = playerAgent;
		@mTalk = false;
	end
	
	def getType()
		return "dialog"
	end
	
	def init()
		print("Initialize DialogBehaviour");
		setActivationLevel(1.0);
	end
	
	def activate()
		$SCRIPT.log("Activate DialogSteering");
	end
		
	def update(elapsedTime)
		if (!getController().isAhead(@mPlayer, 0.95)) #getController().calcDistance(@mPlayer.getVehicle().getPosition(), getController().getPosition()) > 2.5)
			getController().addForce(getController().calcPursuit(@mPlayer) * getActivationLevel() * 5.0);
		else
			if (@mTalk == false)
				getController().setAnimation("reden");
				@mTalk = true;
			end
			#getController().addForce([0.0, 0.0, 0.0]);
		end
	end
	
	def deactivate()
		@mTalk = false;
	end
	
	def calculateActivation()
		if(getController().isDialogActive())
			setActivationLevel(1.0);
		else
			setActivationLevel(0.0);
		end
		return getActivationLevel();
	end
end

class AvoidObstaclesBehaviour < SteeringBehaviour
	def initialize()
		super();
		init();
	end
	
	def getType()
		return "avoid"
	end
	
	def init()
		print("Initialize AvoidObstaclesBehaviour");
		setActivationLevel(1.0);
	end
	
	def activate()
		$SCRIPT.log("Activate DialogSteering");
	end
		
	def update(elapsedTime)
		getController().addForce(getController().calcAvoidNeighbors(5.0) * getActivationLevel() * 2.0);
		getController().addForce(getController().calcAvoidObstacles(5.0) * getActivationLevel() * 2.0);
	end
	
	def deactivate()
	end
	
	def calculateActivation()
		setActivationLevel(1.0);
		return getActivationLevel();
	end
end

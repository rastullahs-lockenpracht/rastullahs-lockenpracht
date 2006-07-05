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
		getController().setAnimation("gehen");
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
		if(getController().isDialogActive())
			setActivationLevel(getActivationLevel()*0.0);
		else
			setActivationLevel(1.0);
		end
		if(getController().needAvoidance(5.0))
			setActivationLevel(getActivationLevel()*0.5);
		end
		return getActivationLevel();
	end
	

end

class DialogBehaviour < SteeringBehaviour
	def initialize(playerActor)
		super();
		init();
		@mActor = playerActor;
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
		getController().setAnimation("gehen");
	end
		
	def update(elapsedTime)
		if (getController().calcDistance(@mActor.getPosition(), getController().getPosition()) > 2.5)
			getController().addForce(getController().calcSeek(@mActor.getPosition()) * getActivationLevel() * 5.0);
		else
			if (@mTalk == false)
				getController().setAnimation("reden");
				@mTalk = true;
			end
		end
	end
	
	def deactivate()
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
		getController().setAnimation("gehen");
	end
		
	def update(elapsedTime)
		getController().addForce(getController().calcAvoidNeighbors(5.0) * getActivationLevel() * 2.0);
		getController().addForce(getController().calcAvoidObstacles(5.0) * getActivationLevel() * 2.0);
	end
	
	def deactivate()
	end
	
	def calculateActivation()
		if(getController().isDialogActive())
			setActivationLevel(0.0);
		else
			setActivationLevel(1.0);
		end
		return getActivationLevel();
	end
end

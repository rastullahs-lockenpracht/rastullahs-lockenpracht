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
		#$SCRIPT.log("Update Wandering");
		getController().addForce(getController().calcWander(elapsedTime) * getActivationLevel());
	#	getController().addForce(getController().calcFlee([-18.0, -6.8, 15.2]) * getActivationLevel());
	end
	
	def deactivate()
	end
	
	def calculateActivation()
		return getActivationLevel();
	end
	

end

load "embed.rb"

class ActorUpdateAnimationListener < AnimationListener
	def animationFinished(anEvent)
		$SCRIPT.log("animationFinished.");
		anim = anEvent.getAnimation();
		mo = anim.getMeshObject();
		actor = mo.getActor();
		pt = actor.getPhysicalThing();
		pt.updateCollisionHull();
	end
	
	def animationPaused(anEvent)
		$SCRIPT.log("animationPaused.");
	end
	
	def animationUnpaused(anEvent)
		$SCRIPT.log("animationUnpaused.");
	end
end


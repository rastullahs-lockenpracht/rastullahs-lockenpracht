require 'globals.rb'

class ActorUpdateAnimationListener < AnimationListener
	def animationFinished(anEvent)
		$CORE.log("animationFinished.");
		anim = anEvent.getAnimation();
		mo = anim.getMeshObject();
		actor = mo.getActor();
		pt = actor.getPhysicalThing();
		pt.updateCollisionHull();
	end
	
	def animationPaused(anEvent)
		$CORE.log("animationPaused.");
	end
	
	def animationUnpaused(anEvent)
		$CORE.log("animationUnpaused.");
	end
end


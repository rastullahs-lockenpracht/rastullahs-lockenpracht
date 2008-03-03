require 'globals.rb'

def PlayAnimation(actor, anim, duration=-1.0, replace=true)
    job = PlayAnimationJob.new(actor, anim, duration, replace)
    $JS.addJob(job)
end

def _PlayAnimation(actor, anim, duration=-1.0, replace=true)
    PlayAnimationJob.new(actor, anim, duration, replace)
end

def StartAnimation(actor, anim, doloop=false, replace=true)
    job = StartAnimationJob.new(actor, anim, doloop, replace)
    $JS.addJob(job)
end

def _StartAnimation(actor, anim, doloop=false, replace=true)
    StartAnimationJob.new(actor, anim, doloop, replace)
end


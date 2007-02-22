require 'globals.rb'

def PlayAnimation(actor, anim, doloop=false, replace=true)
    job = PlayAnimationJob.new(actor, anim, doloop, replace)
    $JS.addJob(job)
end


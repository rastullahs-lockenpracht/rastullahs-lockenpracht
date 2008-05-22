require 'globals.rb'

def GoTo(acting_creature, targetpos)
    job = GoToJob.new(acting_creature, targetpos, 1000000, 1000000)
    $JS.addJob(job)
end

def _GoTo(acting_creature, targetpos)
    GoToJob.new(acting_creature, targetpos, 1000000, 1000000)
end

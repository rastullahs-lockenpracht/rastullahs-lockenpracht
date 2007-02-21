require 'globals.rb'

# Play a 2d sound
def PlaySound2d(sound, volume=1.0)
    job = PlaySound2dJob.new(sound, volume)
    $JS.addJob(job)
end

# Play a 3d sound at position pos
def PlaySound3d(sound, pos, volume=1.0)
    job = PlaySound3dJob.new(sound, pos, volume)
    $JS.addJob(job)
end


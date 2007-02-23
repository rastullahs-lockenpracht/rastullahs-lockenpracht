require 'globals.rb'

# Schedule a sequence of jobs.
# jobs is an array of Jobs and Numerics. Numerics are accumulated pauses
# till the next job starts.
def jobSequence (jobs)
    start = 0
    p "start"
    jobs.each { |x|
        if x.is_a? Numeric:
            # This is a time, we add it to the start time, of jobs
            # later in the queue
            start += x
            p "starttime #start"
        else
            $JS.addJob(x, 20, start)
            p "job added"
        end
    }
end


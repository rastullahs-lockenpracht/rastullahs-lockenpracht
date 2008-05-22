require 'globals.rb'

class ClosureJob < Job
  def initialize()
    super(false, true)
  end

  def setCode(&code)
    @code = code
  end
  
  def execute(time)
	begin
		@code.call
	rescue StandardError => se
		$SCRIPT.logError(se.to_s)
	ensure
		return true
	end
  end
end

def _Execute(&code)
    job = ClosureJob.new()
    job.setCode(&code) 
    return job
end

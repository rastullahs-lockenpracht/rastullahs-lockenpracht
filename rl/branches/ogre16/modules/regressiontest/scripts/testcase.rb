class AssertionException < RuntimeError
end

class TestCase
    def initialize(center)
        @center = center
    end
	
    def rel_pos(position)
        vector_add(@center, position)
    end
	
	def assert(condition, text)
	  if not condition then
	    raise AssertionException, text
	  end
	end

	def assertEquals(target, current, text)
	  if target != current then
	    raise AssertionException, "Value should be '" + target.to_s + "' but is '"+current.to_s+"'. "+text
      end
	end
	
	def run()
		execute()
		return 0
	rescue Exception => ex
		p ex
		return 1 
	end
	
	def getCenter()
	  return @center
	end
	
	# Override this method in test cases
	def execute()
	end
end


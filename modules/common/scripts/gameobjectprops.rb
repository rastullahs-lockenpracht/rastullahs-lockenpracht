load "embed.rb"

module GameObjectProperties
  
    def getAllPropertyKeys()
        $SCRIPT.log("Check Ruby GameObject for properties.");
        
        #  Get all properties from the parent C++ class
        #ps = PropertySet.new
        keys = super()
    
        #  Get all instance variables from the ruby class
        aPropArray = instance_variables
    
        # For every variable check if it is a property
        for aPropertyName in aPropArray
            if aPropertyName =~ /^@_prop_/
                keys << "#{$'}"
            end
        end
        return keys
    end
	
	# Generic setProperty method. This will query self for a member variable @_prop_<key>
	# and, if it is available set the property accordingly. If not, setProperty of the super class is
	# called instead.
	def setProperty(key, value)
		if instance_variables.include?("@_prop_"+key)
	        begin
	            instance_variable_set("@_prop_"+key, value)
            rescue
		        super(key, value)
		    end
		else
		    super(key, value);
		end
	end

	# Generic getProperty method. This will query self for a member variable @_prop_<key>
	# If present, its value is returned, else getProperty of the super class is called.
	def getProperty(key)
		if instance_variables.include?("@_prop_"+key)
	        return instance_variable_get("@_prop_"+key)
		else
		    return super(key);
		end
	end
end

load "embed.rb"

module GameObjectProperties
  
    def getAllProperties()
        $SCRIPT.log("Check Ruby GameObject for properties.");
        
        #  Get all properties from the parent C++ class
        #ps = PropertySet.new
        ps = super()
    
        #  Get all instance variables from the ruby class
        aPropArray = instance_variables
    
        # For every variable check if it is a property
        for aPropertyName in aPropArray
            if aPropertyName =~ /^@_prop_/
                # Get the value of the variable
                aPropertyValue = instance_variable_get(aPropertyName)      
                $SCRIPT.log("Property <" + "#{$'}" + "," + 
                    instance_variable_get(aPropertyName).to_s + "> added to PropertySet");
          
                # Add property to the property set
                ps.setProperty("#{$'}", aPropertyValue)
            end
        end
        return ps
    end
	
#	def setProperty(key, value)
#		instance_variable_set("@_prop_"+key, value)
#	end

#	def getProperty(key)
#		return instance_variable_get("@_prop_"+key)
#	end
  
end

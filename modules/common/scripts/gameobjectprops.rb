load "embed.rb"

module GameObjectProperties
  
    def getAllProperties()
        #  Get all properties from the parent C++ class
        ps = PropertySet.new
        ps = super.getAllProperties()
    
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
                #newProp = Property.new(aPropertyValue)
                #ps.setProperty("#{$'}", newProp)
            end
        end
        return ps
    end
  
end

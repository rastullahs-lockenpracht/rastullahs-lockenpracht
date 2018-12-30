TAB = "\t"

class Attribute 
    attr_reader :name, :value
    attr_writer :name, :value

    def initialize( name, value )
        @name = name
        @value = value
    end
end

class Newline
    def to_s
        "__NEWLINE__"
    end
end

class AttributedObject
    attr_reader :attributes

    def initialize
        @attributes = []
    end

    def addAttribute( name, attributes )
        attributes.pop
        @attributes.push( Attribute.new( name, attributes ) )     
    end

    def getAttributes( name )
        rets = []
        @attributes.each { |att| 
            rets.push( att ) unless name != att.name
        }
        return rets
    end

    def to_s
        temparr = []
        @attributes.each { |att| 
            temparr << TAB << att.name << " " << att.value.join( " " ) << "\n"
        }
        temparr.join
    end
end

class Material < AttributedObject
    attr_reader :techniques, :name, :basename, :filename, :firstinfile
    attr_writer :techniques, :name, :basename, :filename

    def initialize( filename, firstinfile )
        super()
        @techniques = []
        @name = nil
        @basename = nil
        @filename = filename
        @firstinfile = firstinfile
    end

    def addTechnique( tech )
        @techniques.push( tech )
    end
    
    def to_s
        temparr = []
        
        temparr << "material" << " " << @name
        temparr << " : " << @basename unless @basename.nil?
        temparr << "\n{\n"
        temparr << super()
        @techniques.each { |su| temparr << su.to_s.gsub(/^/, TAB ) }
        temparr << "}\n"

        temparr.join
    end
end

class Technique < AttributedObject
    attr_reader :passes, :name
    attr_writer :passes, :name

    def initialize( )
        super()
        @passes = []
        @name = nil
    end

    def to_s
        temparr = []
        
        temparr << "technique"
        temparr << " " << @name unless @name.nil?
        temparr << "\n{\n"
        temparr << super()
        @passes.each { |su| temparr << su.to_s.gsub(/^/, TAB ) }
        temparr << "}\n"

        temparr.join
    end
end

class Pass < AttributedObject
    attr_reader :tus, :vprs, :scvprs, :srvprs, :fprs, :name
    attr_writer :tus, :vprs, :scvprs, :srvprs, :fprs, :name

    def initialize( )
        super()
        @tus = []
        @vprs = []
        @scvprs = []
        @srvprs = []
        @fprs = []
        @name = nil
    end

    def to_s
        temparr = []
        
        temparr << "pass"
        temparr << " " << @name unless @name.nil?
        temparr << "\n{\n"
        temparr << super()
        @tus.each { |su| temparr << su.to_s.gsub(/^/, TAB ) }
        @vprs.each { |su| temparr << su.to_s.gsub(/^/, TAB ) }
        @scvprs.each { |su| temparr << su.to_s.gsub(/^/, TAB ) }
        @srvprs.each { |su| temparr << su.to_s.gsub(/^/, TAB ) }
        @fprs.each { |su| temparr << su.to_s.gsub(/^/, TAB ) }
        temparr << "}\n"

        temparr.join
    end
end

class TextureUnit < AttributedObject
    attr_reader :name
    attr_writer :name

    def initialize( )
        super()
        @name = nil
    end

    def to_s
        temparr = []
        
        temparr << "texture_unit"
        temparr << " " << @name unless @name.nil?
        temparr << "\n{\n"
        temparr << super()
        temparr << "}\n"

        temparr.join
    end
end

class VertexProgramRef < AttributedObject
    attr_reader :name
    attr_writer :name

    def initialize( )
        super()
        @name = nil
    end

    def to_s
        temparr = []
        
        temparr << "vertex_program_ref"
        temparr << " " << @name unless @name.nil?
        temparr << "\n{\n"
        temparr << super()
        temparr << "}\n"

        temparr.join
    end
end

class ShadowCasterVertexProgramRef < AttributedObject
    attr_reader :name
    attr_writer :name

    def initialize( )
        super()
        @name = nil
    end

    def to_s
        temparr = []
        
        temparr << "shadow_caster_vertex_program_ref"
        temparr << " " << @name unless @name.nil?
        temparr << "\n{\n"
        temparr << super()
        temparr << "}\n"

        temparr.join
    end
end

class ShadowReceiverVertexProgramRef < AttributedObject
    attr_reader :name
    attr_writer :name

    def initialize( )
        super()
        @name = nil
    end

    def to_s
        temparr = []
        
        temparr << "shadow_receiver_vertex_program_ref"
        temparr << " " << @name unless @name.nil?
        temparr << "\n{\n"
        temparr << super()
        temparr << "}\n"

        temparr.join
    end
end

class FragmentProgramRef < AttributedObject
    attr_reader :name
    attr_writer :name

    def initialize( )
        super()
        @name = nil
    end

    def to_s
        temparr = []
        
        temparr << "fragment_program_ref"
        temparr << " " << @name unless @name.nil?
        temparr << "\n{\n"
        temparr << super()
        temparr << "}\n"

        temparr.join
    end
end

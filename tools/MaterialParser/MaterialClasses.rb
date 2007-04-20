class Attribute 
    attr_reader :name, :value
    attr_writer :name, :value

    def initialize( name, value )
        @name = name
        @value = value
    end
end

class AttributedObject
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
            ret.push( att ) unless name != att.name
        }
        return rets
    end
end

class Material < AttributedObject
    attr_reader :techniques, :name, :basename
    attr_writer :techniques, :name, :basename

    def initialize( )
        super()
        @techniques = []
        @name = nil
        @basename = nil
    end

    def addTechnique( tech )
        @techniques.push( tech )
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
end

class Pass < AttributedObject
    attr_reader :tus, :vprs, :scvprs, :fprs,:name
    attr_writer :tus, :vprs, :scvprs, :fprs,:name

    def initialize( )
        super()
        @tus = []
        @vprs = []
        @scvprs = []
        @fprs = []
        @name = nil
    end
end

class TextureUnit < AttributedObject
    attr_reader :texture, :name
    attr_writer :texture, :name

    def initialize( )
        super()
        @name = nil
    end
end

class VertexProgramRef < AttributedObject
    attr_reader :texture, :name
    attr_writer :texture, :name

    def initialize( )
        super()
        @name = nil
    end
end

class ShadowCasterVertexProgramRef < AttributedObject
    attr_reader :texture, :name
    attr_writer :texture, :name

    def initialize( )
        super()
        @name = nil
    end
end

class FragmentProgramRef < AttributedObject
    attr_reader :texture, :name
    attr_writer :texture, :name

    def initialize( )
        super()
        @name = nil
    end
end

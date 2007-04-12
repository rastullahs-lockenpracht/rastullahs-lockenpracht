require "MaterialClasses.rb"

class MaterialParser
    attr_reader :materials

	def initialize
        @materials = []     
	end

	def parseFile( file )
	end

    def addMaterial( mat )
        @materials.push( mat )
    end

    def parseString( string )
        mp = self;

        mat = Material.new
        tec = Technique.new
        pa = Pass.new
        tu = TextureUnit.new
        vpr = VertexProgramRef.new
        scvpr = ShadowCasterVertexProgramRef.new
        fpr = FragmentProgramRef.new

        parser = RDParser.new do
            # Whitespaces und Kommentare werden ignoriert
            token(/[\s\n]+/)   
            token(/^\s*\/\/*.*$/)
            # Integer
            token(/\b[-+]?\d+\b/) {|m| m.to_i }
            # Float
            token(/\b\d+(\.\d+)?\b/) {|m| m.to_f }            
            # Wortzeichen
            token(/[^\s]+/) {|m| m.to_s }
            # Einzelne Zeichen
            token(/./) {|m| m }

            def save_attr(ao,name,attr) 
                ao.addAttribute( name, attr ); 
            end

            start :skript do
                match('material', :string, '{', :material, '}' ) {|_,n| mat.name = n; mp.addMaterial(mat);mat = Material.new }
                match('material', :string, ':', :string, '{', :material, '}' ) {|_,n,_,b| mat.name = n; mat.basename = b; mp.addMaterial(mat); mat = Material.new }

                match(:skript, :skript ) 
            end

            rule :material do
                match('technique', :string, '{', :technique, '}' ) {|_,n| tec.name = n; mat.techniques.push( tec ); tec = Technique.new; }
                match('technique', '{', :technique, '}' ) { mat.techniques.push( tec ); tec = Technique.new; } 

                match('receive_shadows', :string ) {|n,*a| save_attr(mat,n,a) }
                match('lod_distances', :float ) {|n,*a| save_attr(mat,n,a) }
                match('lighting', :string ) {|n,*a| save_attr(mat,n,a) }
                match('set_texture_alias', :string, :string ) {|n,*a| save_attr(mat,n,a) } 

                match(:material, :material ) 
            end

            rule :technique do
                match('pass', :string, '{', :pass, '}' ) {|_,n| pa.name = n; tec.passes.push( pa ); pa = Pass.new; }
                match('pass', '{', :pass, '}' ) { tec.passes.push( pa ); pa = Pass.new; } 

                match('lod_index', :int ) {|n,*a| save_attr(tec,n,a) }
                match('scheme', :string ) {|n,*a| save_attr(tec,n,a) }

                match(:technique, :technique ) 
            end

            rule :pass do
                match('texture_unit', :string, '{', :texture_unit, '}' ) {|_,n| tu.name = n; pa.tus.push( tu ); tu = TextureUnit.new; }
                match('texture_unit', '{', :texture_unit, '}' ) { pa.tus.push( tu ); tu = TextureUnit.new; } 

                match('vertex_program_ref', :string, '{', :vertex_program_ref, '}' ) {|_,n| tu.name = n; pa.vprs.push( vpr ); vpr = VertexProgramRef.new; }
                match('vertex_program_ref','{', :vertex_program_ref, '}' ) { pa.vprs.push( vpr ); vpr = VertexProgramRef.new; } 

                match('shadow_caster_vertex_program_ref', :string, '{', :shadow_caster_vertex_program_ref, '}' ) { pa.scvprs.push( scvpr ); scvpr = ShadowCasterVertexProgramRef.new; } 
                match('shadow_caster_vertex_program_ref','{', :shadow_caster_vertex_program_ref, '}' ) { pa.scvprs.push( scvpr ); scvpr = ShadowCasterVertexProgramRef.new; } 

                match('fragment_program_ref', :string, '{', :fragment_program_ref, '}' ) { pa.fprs.push( fpr ); fpr = FragmentProgramRef.new; } 
                match('fragment_program_ref','{', :fragment_program_ref, '}' ) { pa.fprs.push( fpr ); fpr = FragmentProgramRef.new; } 

                match('alpha_rejection', :string, :int ) {|n,*a| save_attr(pa,n,a) }
                match('depth_write', :string ) {|n,*a| save_attr(pa,n,a) }
                match('point_sprites', :string ) {|n,*a| save_attr(pa,n,a) }
                match('point_size', :int ) {|n,*a| save_attr(pa,n,a) }
                match('point_size_attenuation', :string ) {|n,*a| save_attr(pa,n,a) }
                match('lighting', :string ) {|n,*a| save_attr(pa,n,a) }
                match('scene_blend', :string ) {|n,*a| save_attr(pa,n,a) }
                match('shading', :string ) {|n,*a| save_attr(pa,n,a) }
                match('ambient', :color ) {|n,*a| save_attr(pa,n,a) }
                match('diffuse', :color ) {|n,*a| save_attr(pa,n,a) }
                match('specular', :color ) {|n,*a| save_attr(pa,n,a) }
                match('cull_hardware', :string ) {|n,*a| save_attr(pa,n,a) }
                match('cull_software', :string ) {|n,*a| save_attr(pa,n,a) }
                match('depth_func', :string ) {|n,*a| save_attr(pa,n,a) }
                match('fog_override', :string, :string ) {|n,*a| save_attr(pa,n,a) }

                match(:pass, :pass ) 
            end

            rule :texture_unit do                 
                match('texture', :string ) {|n,*a| save_attr(tu,n,a) }
                match('texture_alias', :int ) {|n,*a| save_attr(tu,n,a) }
                match('tex_coord_set', :int ) {|n,*a| save_attr(tu,n,a) } 
                match('scale', :float, :float ) {|n,*a| save_attr(tu,n,a) }
                match('scroll_anim', :float, :float ) {|n,*a| save_attr(tu,n,a) }
                match('cubic_texture', :string, :string ) {|n,*a| save_attr(tu,n,a) }
                match('filtering', :string, :string, :string ) {|n,*a| save_attr(tu,n,a) }
                match('filtering', :string ) {|n,*a| save_attr(tu,n,a) }
                match('max_anisotropy', :int ) {|n,*a| save_attr(tu,n,a) }
                match('tex_address_mode', :string ) {|n,*a| save_attr(tu,n,a) }
                match('colour_op_multipass_fallback', :string, :string ) {|n,*a| save_attr(tu,n,a) } 
                match('colour_op_ex', :string, :string, :string, :float, :float, :float  ) {|n,*a| save_attr(tu,n,a) } 
                match('colour_op_ex', :string, :string, :string ) {|n,*a| save_attr(tu,n,a) } 
                match('alpha_op_ex', :string, :string, :string, :float ) {|n,*a| save_attr(tu,n,a) } 

                match(:texture_unit, :texture_unit ) 
            end

            rule :vertex_program_ref do
                match('param_named_auto', :string, :string, :int ) {|n,*a| save_attr(vpr,n,a) }
                match('param_named_auto', :string, :string ) {|n,*a| save_attr(vpr,n,a) }
                
                match(:vertex_program_ref, :vertex_program_ref ) 
            end
             
            rule :shadow_caster_vertex_program_ref do
                match('param_named_auto', :string, :string, :int ) {|n,*a| save_attr(scvpr,n,a) }
                match('param_named_auto', :string, :string ) {|n,*a| save_attr(scvpr,n,a) }
                
                match(:shadow_caster_vertex_program_ref, :shadow_caster_vertex_program_ref ) 
            end

            rule :fragment_program_ref do
                match('param_named', :string, :string, :float ) {|n,*a| save_attr(fpr,n,a) }
                match('param_named', :string, :string, :int ) {|n,*a| save_attr(fpr,n,a) }
                
                match(:fragment_program_ref, :fragment_program_ref ) 
            end

            rule :color do
                match(:float, :float, :float, :float )
                match(:float, :float, :float )
                match('vertexcolour')
            end

            rule :string do
                match(String)
                match(Float) {|f| return String(f) }
                match(Integer) {|i| return String(i) }
            end

            rule :float do
                match(Float)
                match(Integer) {|i| return Float(i) }
            end

            rule :int do
                match(Integer)
            end
        end

        parser.parse( string )
    end
end
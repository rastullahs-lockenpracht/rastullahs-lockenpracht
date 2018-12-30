require "MaterialClasses.rb"

NEWLINE = Newline.new

class MaterialParser
    attr_reader :materials
    attr_reader :loose_strings
    include Enumerable

	def initialize
        @materials = []    
        @loose_strings = Hash.new
	end

	def parseFile( filename )
        parseMe = File.new( filename ).read
        return parseString( parseMe, filename )
	end

    def addMaterial( mat )
        @materials.push( mat )
    end

    def addLooseString( filename, string)
        temp = @loose_strings[filename];
        temp = string if ( temp == nil )
        
        p string;

        @loose_strings[filename] = temp;
    end

    def each
        @materials.each do
            yield
        end
    end

    def parseString( string, filename )
        mp = self;

        mat = Material.new( filename, true )
        tec = Technique.new
        pa = Pass.new
        tu = TextureUnit.new
        vpr = VertexProgramRef.new
        scvpr = ShadowCasterVertexProgramRef.new
        srvpr = ShadowReceiverVertexProgramRef.new
        fpr = FragmentProgramRef.new

        parser = RDParser.new do
            # Whitespaces und Kommentare werden ignoriert
            token(/^\s*\/\/*.*$/)
            token(/\s*\n/) { ::NEWLINE }
            token(/[\s\n]+/)  
            # Float/Integer
            token(/\b[-+]?\d+(\.\d+)?\b/) {|m| m.to_f }   
            # Wortzeichen
            token(/[^\s:]+/) {|m| m.to_s }
            # Einzelne Zeichen
            token(/./) {|m| m }

            def save_attr(ao,name,attr) 
                ao.addAttribute( name, attr ); 
            end

            start :skript do 
                match('material', :string, :newlines, '{', :material, '}' ) {|_,n| mat.name = n; mp.addMaterial(mat); mat = Material.new( filename, false ) }
                match('material', :string, ':', :string, :newlines, '{', :material, '}' ) {|_,n,_,b| mat.name = n; mat.basename = b; mp.addMaterial(mat); mat = Material.new( filename, false ) }
                
                match('vertex_program', :string, :string, :newlines, '{', :shader_def, '}' )
                match('fragment_program', :string, :string, :newlines, '{', :shader_def, '}' )

                match( ::NEWLINE )

                match(:skript, :skript ) 
            end

            rule :material do 
                match('technique', :string, :newlines, '{', :technique, '}' ) {|_,n| tec.name = n; mat.techniques.push( tec ); tec = Technique.new; }
                match('technique', :newlines, '{', :technique, '}' ) { mat.techniques.push( tec ); tec = Technique.new; } 

                match('receive_shadows', :string, ::NEWLINE ) {|n,*a| save_attr(mat,n,a) }
                match('lod_distances', :float, ::NEWLINE ) {|n,*a| save_attr(mat,n,a) }
                match('lighting', :string, ::NEWLINE ) {|n,*a| save_attr(mat,n,a) }
                match('set_texture_alias', :string, :string, ::NEWLINE ) {|n,*a| save_attr(mat,n,a) } 

                match( ::NEWLINE )
                match(:material, :material ) 
            end

            rule :technique do 
                match('pass', :string, :newlines, '{', :pass, '}' ) {|_,n| pa.name = n; tec.passes.push( pa ); pa = Pass.new; }
                match('pass', :newlines, '{', :pass, '}' ) { tec.passes.push( pa ); pa = Pass.new; } 

                match('lod_index', :int, ::NEWLINE ) {|n,*a| save_attr(tec,n,a) }
                match('scheme', :string, ::NEWLINE ) {|n,*a| save_attr(tec,n,a) }

                match( ::NEWLINE )
                match(:technique, :technique ) 
            end

            rule :pass do 
                match('texture_unit', :string, :newlines, '{', :texture_unit, '}' ) {|_,n| tu.name = n; pa.tus.push( tu ); tu = TextureUnit.new; }
                match('texture_unit', :newlines, '{', :texture_unit, '}' ) { pa.tus.push( tu ); tu = TextureUnit.new; } 

                match('vertex_program_ref', :string, :newlines, '{', :vertex_program_ref, '}' ) {|_,n| tu.name = n; pa.vprs.push( vpr ); vpr = VertexProgramRef.new; }
                match('vertex_program_ref', :newlines,'{', :vertex_program_ref, '}' ) { pa.vprs.push( vpr ); vpr = VertexProgramRef.new; } 

                match('shadow_caster_vertex_program_ref', :string, :newlines, '{', :shadow_caster_vertex_program_ref, '}' ) { pa.scvprs.push( scvpr ); scvpr = ShadowCasterVertexProgramRef.new; } 
                match('shadow_caster_vertex_program_ref', :newlines,'{', :shadow_caster_vertex_program_ref, '}' ) { pa.scvprs.push( scvpr ); scvpr = ShadowCasterVertexProgramRef.new; } 

                match('shadow_receiver_vertex_program_ref', :string, :newlines, '{', :shadow_caster_vertex_program_ref, '}' ) { pa.srvprs.push( srvpr ); srvpr = ShadowReceiverVertexProgramRef.new; } 
                match('shadow_receiver_vertex_program_ref', :newlines,'{', :shadow_caster_vertex_program_ref, '}' ) { pa.srvprs.push( srvpr ); srvpr = ShadowReceiverVertexProgramRef.new; } 


                match('fragment_program_ref', :string, :newlines, '{', :fragment_program_ref, '}' ) { pa.fprs.push( fpr ); fpr = FragmentProgramRef.new; } 
                match('fragment_program_ref', :newlines,'{', :fragment_program_ref, '}' ) { pa.fprs.push( fpr ); fpr = FragmentProgramRef.new; } 

                match('alpha_rejection', :string, :int, ::NEWLINE ) {|n,*a| save_attr(pa,n,a) }
                match('receive_shadows', :string, ::NEWLINE ) {|n,*a| save_attr(pa,n,a) }
                match('depth_write', :string, ::NEWLINE ) {|n,*a| save_attr(pa,n,a) }
                match('point_sprites', :string, ::NEWLINE ) {|n,*a| save_attr(pa,n,a) }
                match('point_size', :int, ::NEWLINE ) {|n,*a| save_attr(pa,n,a) }
                match('point_size_attenuation', :string, ::NEWLINE ) {|n,*a| save_attr(pa,n,a) }
                match('lighting', :string, ::NEWLINE ) {|n,*a| save_attr(pa,n,a) }
                match('scene_blend', :string, :string, ::NEWLINE ) {|n,*a| save_attr(pa,n,a) }
                match('scene_blend', :string, ::NEWLINE ) {|n,*a| save_attr(pa,n,a) }
                match('shading', :string, ::NEWLINE ) {|n,*a| save_attr(pa,n,a) }
                match('ambient', :color, ::NEWLINE ) {|n,*a| save_attr(pa,n,a) }
                match('emissive', :color, ::NEWLINE ) {|n,*a| save_attr(pa,n,a) }
                match('diffuse', :color, ::NEWLINE ) {|n,*a| save_attr(pa,n,a) }
                match('specular', :color, ::NEWLINE ) {|n,*a| save_attr(pa,n,a) }
                match('cull_hardware', :string, ::NEWLINE) {|n,*a| save_attr(pa,n,a) }
                match('cull_software', :string, ::NEWLINE ) {|n,*a| save_attr(pa,n,a) }
                match('depth_func', :string, ::NEWLINE ) {|n,*a| save_attr(pa,n,a) }
                match('fog_override', :string, ::NEWLINE ) {|n,*a| save_attr(pa,n,a) }
                match('fog_override', :string, :string, ::NEWLINE ) {|n,*a| save_attr(pa,n,a) }
                match('fog_override', :string, :string, :float, :float, :float, :float, :float, :float, ::NEWLINE ) {|n,*a| save_attr(pa,n,a) }

                match( ::NEWLINE )
                match(:pass, :pass ) 
            end

            rule :texture_unit do    
                match('texture', :string, :string, ::NEWLINE ) {|n,*a| save_attr(tu,n,a) }
                match('texture', :string, ::NEWLINE ) {|n,*a| save_attr(tu,n,a) }
                match('texture_alias', :string, ::NEWLINE ) {|n,*a| save_attr(tu,n,a) }
                match('tex_coord_set', :int, ::NEWLINE ) {|n,*a| save_attr(tu,n,a) } 
                match('scale', :float, :float, ::NEWLINE ) {|n,*a| save_attr(tu,n,a) }
                match('scroll_anim', :float, :float, ::NEWLINE ) {|n,*a| save_attr(tu,n,a) }
                match('cubic_texture', :string, :string, ::NEWLINE ) {|n,*a| save_attr(tu,n,a) }
                match('filtering', :string, :string, :string, ::NEWLINE ) {|n,*a| save_attr(tu,n,a) }
                match('filtering', :string, ::NEWLINE ) {|n,*a| save_attr(tu,n,a) }
                match('max_anisotropy', :int, ::NEWLINE ) {|n,*a| save_attr(tu,n,a) }
                match('tex_address_mode', :string, ::NEWLINE ) {|n,*a| save_attr(tu,n,a) }
                match('colour_op_multipass_fallback', :string, :string, ::NEWLINE ) {|n,*a| save_attr(tu,n,a) } 
                match('colour_op_ex', :string, :string, :string, :float, :float, :float, ::NEWLINE  ) {|n,*a| save_attr(tu,n,a) } 
                match('colour_op_ex', :string, :string, :string, ::NEWLINE ) {|n,*a| save_attr(tu,n,a) } 
                match('alpha_op_ex', :string, :string, :string, :float, ::NEWLINE ) {|n,*a| save_attr(tu,n,a) } 
                match('alpha_op_ex', :string, :string, :string, ::NEWLINE ) {|n,*a| save_attr(tu,n,a) } 

                match( ::NEWLINE )
                match(:texture_unit, :texture_unit ) 
            end

            rule :vertex_program_ref do 
                match('param_named', :string, :params, ::NEWLINE ) {|n,*a| save_attr(vpr,n,a) }
                match('param_named_auto', :string, :params, ::NEWLINE ) {|n,*a| save_attr(vpr,n,a) }

                match( ::NEWLINE )
                match(:vertex_program_ref, :vertex_program_ref ) 
            end
             
            rule :shadow_caster_vertex_program_ref do 
                match('param_named', :string, :params, ::NEWLINE ) {|n,*a| save_attr(scvpr,n,a) }
                match('param_named_auto', :string, :params, ::NEWLINE ) {|n,*a| save_attr(scvpr,n,a) }

                match( ::NEWLINE )
                match(:shadow_caster_vertex_program_ref, :shadow_caster_vertex_program_ref ) 
            end
             
            rule :shadow_receiver_vertex_program_ref do 
                match('param_named', :string, :params, ::NEWLINE ) {|n,*a| save_attr(srvpr,n,a) }
                match('param_named_auto', :string, :params, ::NEWLINE ) {|n,*a| save_attr(srvpr,n,a) }

                match( ::NEWLINE )
                match(:shadow_receiver_vertex_program_ref, :shadow_receiver_vertex_program_ref ) 
            end

            rule :fragment_program_ref do 
                match('param_named', :string, :params, ::NEWLINE ) {|n,*a| save_attr(fpr,n,a) }
                match('param_named_auto', :string, :params, ::NEWLINE  ) {|n,*a| save_attr(fpr,n,a) }

                match( ::NEWLINE )
                match(:fragment_program_ref, :fragment_program_ref ) 
            end

            rule :params do              
                match( 'float4', :float, :float, :float, :float ) {|*a| a.join(" ") }
                match( 'float', :float ) {|*a| a.join(" ") }
                match( :string, :int ) {|*a| a.join(" ") }
                match( :string ) {|s| s }
            end

            rule :color do 
                match(:float, :float, :float, :float, :float ) {|*a| a.join(" ") }
                match(:float, :float, :float, :float ) {|*a| a.join(" ") }
                match(:float, :float, :float ) {|*a| a.join(" ") }
                match('vertexcolour') 
            end

            rule :shader_def do 
                match( "source", :string, ::NEWLINE ) 
	            match( "target", :string, ::NEWLINE ) 
	            match( "entry_point", :string, ::NEWLINE ) 

                match( ::NEWLINE )
                match(:shader_def, :shader_def ) 
            end

            rule :string do                
                match(Float) {|f| String(f) }
                match(Integer) {|i| String(i) }
                match(String)
            end

            rule :float do
                match(Float)
                match(Integer) {|i| Float(i) }
            end

            rule :int do
                match(Integer)
                match(Float) {|f| Integer(f) }
            end

            rule :newlines do 
                match(::NEWLINE)
                match(::NEWLINE,:newlines)
            end
        end

        return parser.parse( string )
    end
end
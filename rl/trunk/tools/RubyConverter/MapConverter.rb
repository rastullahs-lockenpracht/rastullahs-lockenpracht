require "MathHelper.rb"     

module MapConverter
    const_set("MAP_Q3",0)
    const_set("MAP_HL",1)    
    
    class Vertex
        attr_reader :vector, :normal
        attr_accessor :u, :v
    
        def initialize( vector, u, v, normal )
            @vector = vector
            @u = u
            @v = v
            @normal = normal
        end
        
        def x 
            return @vector[0]
        end
        
        def y 
            return @vector[1]
        end
        
        def z 
            return @vector[2]
        end
        
        def nx 
            return @normal[0]
        end
        
        def ny 
            return @normal[1]
        end
        
        def nz 
            return @normal[2]
        end
        
        def ==( otherObj )
            return false unless 
                otherObj.is_a?( self.class ) &&
                self.vector == otherObj.vector &&
                self.normal == otherObj.normal &&
                self.u == otherObj.u &&
                self.v == otherObj.v
        end
    end
    
    class Face
        attr_reader :vertices, :texture , :center
    
        def initialize
            @vertices = Array.new
        end
        
        def addVector( vec )
            raise( TypeError,  vec.class.to_s+" is no MathHelper::Vector" ) unless vec.class == MathHelper::Vector
        
            if ! @vertices.include?( vec )
                @vertices.push( vec )
            end 
        end
        
        def toVertex( vec, uv, normal )
            raise( TypeError,  vec.class.to_s+" is no MathHelper::Vector" ) unless vec.class == MathHelper::Vector
            raise( TypeError,  normal.class.to_s+" is no MathHelper::Vector" ) unless normal.class == MathHelper::Vector
            raise( "Vector is no part of this face" ) unless @vertices.include?( vec )
            
            i = @vertices.index( vec )
            @vertices[i] = Vertex.new( vec, uv[0], uv[1], normal )
        end
        
        def calculateCenter()
            return unless @vertices.size > 0
        
            @center = MathHelper::Vector.new( [0,0,0] )
            @vertices.each{ |v| @center += v }
            @center = center/@vertices.size
        end
        
        def normalizeTexCoords
            normU = true
            normV = true
            
            # Check which Coords can be normalized
            @vertices.each{ |vert| 
                if vert.u < 1 && vert.u > -1
                    normU = false
                end
                if vert.v < 1 && vert.v > -1
                    normV = false
                end
            }
            
            if normU || normV
                nearestU = 0;
                u= @vertices[0].u

                nearestV = 0;
                v = @vertices[0].v
                
                
                if normU
                    if u > 1
                        nearestU = u.floor
                    else
                        nearestU = u.ceil
                    end
                end
                
                if normV
                    if v > 1
                        nearestV = v.floor
                    else
                        nearestV =v.ceil
                    end
                end
                
                
                @vertices.each{ |vert| 
                    
                    if normU
                        u = vert.u
                        
                        if u.abs < nearestU.abs
                            if u > 1
                                nearestU = u.floor
                            else
                                nearestU = u.ceil
                            end
                        end
                    end
                    
                    if normV
                        v = vert.v
                        
                        if v.abs < nearestV.abs
                            if v > 1
                                nearestV = v.floor
                            else
                                nearestV = v.ceil
                            end
                        end
                    end
                    
                    if normV
                    end
                }
                
                @vertices.each{ |vert| 
                    vert.u -= nearestU
                    vert.v -= nearestV
                }
            end
                
        end
        
        def sortVertices()            
            return unless @vertices.size > 0
            
            for i in 0..@vertices.size-3
                smallestAngle = -1.0
                smallestAngleID = -1
            
                ma = MathHelper::Vector.new(  @vertices[i].vector.to_ary ) - @center  
                ma.normalize!
                
                for j in i+1..@vertices.size-1
                    mb = MathHelper::Vector.new( @vertices[j].vector.to_ary ) - @center
                    mb.normalize!
                    angle = ma.dot(mb)
                    
                    if ma.cross(mb).dot(@vertices[j].normal) < 0.0 
                        angle += 2.0
                    end
                                        
                    if angle > smallestAngle
                        smallestAngle = angle
                        smallestAngleID = j
                    end
                end
                
                if smallestAngleID != i+1 && smallestAngleID != -1
                    temp = @vertices[i+1]
                    @vertices[i+1] = @vertices[smallestAngleID]
                    @vertices[smallestAngleID] = temp
                end
            end
            
            @vertices.reverse!
        end
        
        def setTexture( tex )
            @texture = tex
        end
    end
    
    class Plane
        # Regulärer Ausdruck für eine HL-Plane-Zeile
        @@re_hl = /\( (.*?) \) \( (.*?) \) \( (.*?) \) (.*?) \[ (.*?) \] \[ (.*?) \] (.*?) (.*?) (.*?)$/
        @@m = nil
        attr_reader :normal, :distance, :texture
        
        def initialize( )
        end

        def parsePlane( string, map_type )
            if map_type = MAP_HL 
                mD = @@re_hl.match( string )
            else
                raise( "Bisher nur HL unterstützt" );
            end
            
            # Aufspalten und in Integer umwandeln und nen Vector draus, so einfach :)
            @v1 = MathHelper::Vector.new( mD[1].split( " " ).collect { |x| x.to_f } )
            @v2 = MathHelper::Vector.new( mD[2].split( " " ).collect { |x| x.to_f } )
            @v3 = MathHelper::Vector.new( mD[3].split( " " ).collect { |x| x.to_f } )
            # Die Textur
            @texture = mD[4]
            # Die Texturverschiebung
            t1 =  mD[5].split( " " ).collect { |x| x.to_f } 
            @t1 = MathHelper::Vector.new( -t1[0], -t1[1], -t1[2]  )
            @tex_offset_x = -t1[3]
            t2 =  mD[6].split( " " ).collect { |x| x.to_f } 
            @t2 = MathHelper::Vector.new( -t2[0], -t2[1], -t2[2] )
            @tex_offset_y = -t2[3]
            # Rotation und Skalierung
            @rotation = mD[7].to_i
            @scale_x = mD[8].to_f
            @scale_y = mD[9].to_f
            
            if @@m
                @v1.rotate!( @@m )
                @v2.rotate!( @@m )
                @v3.rotate!( @@m )
                @t1.rotate!( @@m )
                @t2.rotate!( @@m )
            end
            
            calculateHessian(  )
        end
        
        def Plane.setOgreAxis
            # Ausgleich der Quake-Achse
            Plane.setRotation( Matrix[[1,0,0],[0,0,1],[0,-1,0]] )
        end
        
        def Plane.setRotation( matrix )            
            @@m = matrix
        end
                
        def getTextureCoordinates( vertex , center )
            texs = TextureManager.getSingleton.loadTexture( @texture )
            width = texs[0]
            height = texs[1]

            u = ( @t1.dot(vertex)/width/@scale_x  ) - ( @tex_offset_x / width )
            v = ( @t2.dot(vertex)/height/@scale_y  ) - ( @tex_offset_y / height )            
            
            return MathHelper::Vector.new( [u,v] )
        end
                
        def calculateHessian(  )
            @normal = ( @v1 - @v2 ).cross(@v3 - @v2)
            @normal.normalize!
            @distance = @normal.dot( @v2 ) #.abs
        end
        
        # Get the intersection point of three planes.
        #                                     -1
        #    |x|     | n1.x n1.y n1.z   |      |d1|
        #    |y|  =  | n2.x n2.y n2.z |  X  |d2|
        #    |z|     | n3.x n3.y n3.z |       |d3|
        def Plane.getIntersection( p1, p2, p3 )
            denom = p1.normal.dot( p2.normal.cross( p3.normal ) )

            # 2 Ebenen sind wohl parallel
            if denom == 0.0
                   return nil
            end
            
            # Muha, nicht zulange anstarren, das beisst
            return ((p2.normal.cross(p3.normal))*-p1.distance - (p3.normal.cross(p1.normal))*p2.distance - ( p1.normal.cross(p2.normal))*p3.distance)/denom            
        end
    end
    
    class Brush        
        attr_reader :faces
    
        def initialize( )
            @planes = Array.new
            @faces = Array.new
        end
        
        def addPlane( plane )
            raise( TypeError, plane.class.to_s+" is no Plane" ) unless plane.class == Plane
        
            @planes.push( plane )
            @faces.push( Face.new )
        end
        
        def addFace( face )
            raise( TypeError, face .class.to_s+" is no Face") unless face .class == Face
        
            @faces.push( face )
        end
        
        def transformToFaces()
        
            # Cycle through all plane combinations.            
            for i in 0..@planes.size-3
                for j in i+1..@planes.size-2
                       for k in j+1..@planes.size-1
                        if i != j && i != k && j != k        
                            inter = Plane.getIntersection( @planes[i], @planes[j], @planes[k] )
                            
                            if inter != nil 
                                bLegal = true
                                
                                @planes.each { |plane|                        
                                    # Make sure the new vector is in the brush.
                                    if ( plane.normal.dot(inter) + plane.distance  ) < -0.0001 
                                        bLegal = false;    
                                        break
                                    end    
                                }
                                
                                if bLegal
                                    @faces[i].addVector( inter )
                                    @faces[j].addVector( inter )
                                    @faces[k].addVector( inter )
                                end
                            end
                        end
                       end
                end
            end
            
            @faces.each_index { |i|         
                @faces[i].setTexture( @planes[i].texture )
                

                
                @faces[i].calculateCenter
                
                @faces[i].vertices.each { |v|
                    @faces[i].toVertex( v, @planes[i].getTextureCoordinates( v, @faces[i].center  ), @planes[i].normal )
                }
                
                @faces[i].normalizeTexCoords
                @faces[i].sortVertices()
                
                #p @faces[i].vertices.size
                #@faces[i].vertices.each{ |v| print v.vector; print "\n" }
                
                #p "->"
                #@faces[i].vertices.each{ |v| print v.vector; print "\n" }
            }    
        end
    end
    
    class Entity
        attr_reader :arguments, :brushes
        @@re = /"(.*)" "(.*)"/
    
        def initialize( )
            @arguments = Hash.new
            @brushes = Array.new
        end
        
        def addArgument( arg, val )
            @arguments[arg] = val
        end
        
        def parseArgument( string )
            md = @@re.match( string )
            
            if md != nil            
                addArgument( md[1], md[2] )
            end
        end
        
        def getArgument( arg )
            @arguments[arg]
        end
        
        def addBrush( brsh )
            raise( TypeError, brsh.class.to_s+" is no Brush" ) unless brsh.class == Brush
                    
            @brushes.push( brsh )
            brsh.transformToFaces()
        end
    end
    
    class Map    
        attr_reader :entities
        
        def initialize(  )
            @entities = Array.new
        end
        
        def addEntity( ent )
            raise( TypeError, ent.class.to_s+" is no Entity") unless ent.class == Entity 
            
            @entities.push( ent )
        end
    end    
    
    class MapReader
        const_set("LINETYPE_EMPTY",0)
        const_set("LINETYPE_OPENBRACE",1)
        const_set("LINETYPE_CLOSEDBRACE",2)
        const_set("LINETYPE_PLANE",3)
        const_set("LINETYPE_ARGVAL",4)
    
        def initialize( )
        end
    
        def readMap( filename, facetype, readGeometry )
            @type = facetype
            @map = Map.new( )
            @readGeometry = readGeometry
            readFile( filename )
            return @map
        end
        
        private
        
        def readFile( filename )
            file = File.new(filename, "r")
            @filecontents = file.read.split( "\n" )            
            @currLine = 0
            
            while @currLine < @filecontents.size
                linetype = processLine( @filecontents[@currLine] )
                line = @filecontents[@currLine]
                @currLine+=1;
                
                case linetype
                    when LINETYPE_EMPTY
                    # Leere Zeile, wir machen nichts
                    when LINETYPE_OPENBRACE
                    # Es beginnt eine Entity
                        ent = readEntity()
                        @map.addEntity( ent )    
                        #p ent.getArgument( "classname" )
                    when LINETYPE_CLOSEDBRACE                
                    # Sowas sollte hier nicht sein
                        raise( "Unexpected Map-Line - ClosedBrace for Base at at line "+@currLine.to_s )
                    when LINETYPE_PLANE
                    # Plane hat hier nichts zu suchen                    
                        raise( "Unexpected Map-Line - Face for Base at at line "+@currLine.to_s )
                    when LINETYPE_ARGVAL                    
                    # Argumente auch nicht... 
                        raise( "Unexpected Map-Line - ArgumentValue for Base at at line "+@currLine.to_s )
                end
            end            
        end
        
        def readEntity( )
            entity = Entity.new
            
            while @currLine < @filecontents.size
                linetype = processLine( @filecontents[@currLine] )
                line = @filecontents[@currLine]
                @currLine+=1;
                
                case linetype
                    when LINETYPE_EMPTY
                    # Leere Zeile, wir machen nichts
                    when LINETYPE_OPENBRACE
                    # Hier beginnt ein Brush
                        brsh = readBrush() 
                        if @readGeometry
                            entity.addBrush( brsh )
                        end
                    when LINETYPE_CLOSEDBRACE                
                    # Es endet die Entity
                        return entity;
                    when LINETYPE_PLANE
                    # Plane hat hier nichts zu suchen
                        raise( "Unexpected Map-Line - Face for Entity at at line "+@currLine.to_s )
                    when LINETYPE_ARGVAL                    
                    # Argumente passen zu Entities
                        entity.parseArgument( line )
                end
            end
        end
        
        def readBrush( )    
            brush = Brush.new

            while @currLine < @filecontents.size
                linetype = processLine( @filecontents[@currLine] )
                line = @filecontents[@currLine]
                @currLine+=1;
                
                case linetype
                    when LINETYPE_EMPTY
                    # Leere Zeile, wir machen nichts
                    when LINETYPE_OPENBRACE
                    # So was wollen wir hier nicht
                        raise( "Unexpected Map-Line - OpenBrace for Brush at at line "+@currLine.to_s    )
                    when LINETYPE_CLOSEDBRACE                
                    # Es endet der Brush
                        return brush;
                    when LINETYPE_PLANE
                    # Plane gehört hier hin
                        if @readGeometry
                            plane = Plane.new
                            plane.parsePlane( line, @facetype )
                            brush.addPlane( plane )
                        end
                    when LINETYPE_ARGVAL                    
                    # Argumente nicht... 
                        raise( "Unexpected Map-Line - ArgVal for Entity at at line "+@currLine.to_s )
                end
            end
        end
        
        def processLine( line )                        
            # Kommentar suchen
            ipos = line.index('//')            
            # Alles nach dem Kommentar abschneiden
            if ipos != nil
                line = line[0,ipos]
            end
            # Leerstellen entfernen
            line.strip!
        
            rval = nil
            
            rval = if line.length  == 0
                LINETYPE_EMPTY
            elsif line[0,1] == "{" then
                LINETYPE_OPENBRACE
            elsif line[0,1] == "}" then
                LINETYPE_CLOSEDBRACE
            elsif line[0,1] == "(" then
                LINETYPE_PLANE
            elsif line[0,1] == "\"" then
                LINETYPE_ARGVAL
            else
                raise( "Unknown Map-Line at line "+@currLine.to_s+" ("+line+") " )
            end
            
            @filecontents[@currLine] = line
        
            return rval
        end
    end
    
    class MeshTriangle
        attr_reader :v1, :v2, :v3, :texture
    
        def initialize( v1, v2, v3, texture )
             @v1 = v1
             @v2 = v2
             @v3 = v3
             @texture = texture
        end        
    end
    
    class FaceConverter
        def FaceConverter.toMeshTriangles( faces )
            triangles = Array.new
            
            # In Dreiecke umwandeln
            faces.each{ |f|
            
                while f.vertices.size >= 3
                    tri = MeshTriangle.new( f.vertices[0], f.vertices[1], f.vertices[2], f.texture )
                    triangles.push( tri )                    
                    
                    f.vertices.delete_at(1)                    
                end
            }
            
            return triangles
        end
    end
    
    class TextureManager
        require 'fox'
        @@manager = nil
        
        def initialize( basedir )
            raise( "Singleton already initialized" ) unless !@@manager 
            @basedir = basedir
            @textures = Hash.new
            @@manager = self
        end
        
        def TextureManager.getSingleton( )
            @@manager = new( "" ) unless @@manager
            return @@manager
        end
        
        def loadTexture( name )

            if ! @textures.has_key? name
                fname = File.join( @basedir , name ) 
                
                img = nil
                ending = ""
                
                if File.exist? fname+".jpg"
                    ending = "jpg"
                    f = Fox::FXFileStream.open( fname+".jpg" , Fox::FXStreamLoad ) 
                    img = Fox.fxloadJPG( f  )           
                elsif File.exist? fname+".gif"
                    ending = "gif"
                    f = Fox::FXFileStream.open( fname+".jpg" , Fox::FXStreamLoad ) 
                    img = Fox.fxloadGIF( f  )     
                elsif File.exist? fname+".png"
                    ending = "png"
                    f = Fox::FXFileStream.open( fname+".png" , Fox::FXStreamLoad ) 
                    img = Fox.fxloadPNG( f  )
                elsif File.exist? fname+".tga"
                    ending = "tga"
                    f = Fox::FXFileStream.open( fname+".png" , Fox::FXStreamLoad ) 
                    img = Fox.fxloadTGA( f  ) 
                end

                if img != nil
                    @textures[ name ] = [img[2],img[3],ending]
                    img = nil
                else
                    print "Konnte Textur #{name} nicht finden\n"
                    @textures[ name ] = [256,256,ending]
                end
                
            end
            
            return @textures[ name ] 
        end
        
        def getTexture( name )
            return @textures[ name ]
        end
    end
    
    class MeshWriter
        def initialize( map )
            raise( TypeError, map.class.to_s+" is no Map" ) unless map.class ==  Map
            
            @map = map
        end
    
        def writeMesh( outdir, basename, ignore_textures = ["SKY","NULL"] )
        
            all_textures = Array.new
            mesh_count = 1
            
            # Jede Entity ein einzelnes Mesh
            @map.entities.each{ |ent| 
                # Alles faces dieser Entity sammeln
                faces = Array.new
            
                ent.brushes.each{ |brush|
                    faces += brush.faces#.slice(0,1)
                }
                
                # Abbruch wenn keine Faces in dieser Entity sind
                break unless faces.size > 0
                
                stream = ""
                
                # Faces in Dreiecke umwandeln
                triangles = FaceConverter.toMeshTriangles( faces )

                # Alle Texturen sammeln
                textures = Array.new
                triangles.each{ |tri| 
                    if ! textures.include?( tri.texture ) && !ignore_textures.include?( tri.texture )
                        textures.push( tri.texture )
                    end
                    
                    if ! all_textures.include?( tri.texture )  && !ignore_textures.include?( tri.texture )
                        all_textures.push( tri.texture )
                    end
                }
                
                break unless textures.size > 0

                stream <<  "<mesh>" << "\n"
                stream <<  "\t<submeshes>" << "\n"
                
                # Jede Textur muss ein einzelnes Submesh sein
                textures.each{ |tex|    
                    # Herausfiltern der Texturen
                    subtris = triangles.dup                    
                    subtris.delete_if{ |tri| 
                        tri.texture != tex 
                    }
                    
                    vertices = Array.new
                    
                    stream <<  "\t\t<submesh material=\"#{tex}\" usesharedvertices=\"false\" use32bitindexes=\"false\">" << "\n"
                    stream <<  "\t\t\t<faces count=\"#{subtris.size}\">" << "\n"
                    # Alle Triangles
                    subtris.each{ |tri|
                        vertices.push tri.v1 unless vertices.include? tri.v1
                        vertices.push tri.v2 unless vertices.include? tri.v2
                        vertices.push tri.v3 unless vertices.include? tri.v3
                        stream <<  "\t\t\t\t<face v1=\"#{vertices.index( tri.v1 )}\" v2=\"#{vertices.index( tri.v2 )}\" v3=\"#{vertices.index( tri.v3 )}\" />" << "\n"  
                    }
                    stream <<  "\t\t\t</faces>" << "\n"
                    stream <<  "\t\t\t<geometry vertexcount=\"#{vertices.size}\">" << "\n"
                    stream <<  "\t\t\t\t<vertexbuffer positions=\"true\" normals=\"true\" colours_diffuse=\"false\" texture_coords=\"1\" texture_coord_dimensions_0=\"2\" >" << "\n"
                    # Alle Vertices der Triangles
                    vertices.each{ |vert| 
                        stream <<  "\t\t\t\t\t<vertex>" << "\n"
                        stream <<  "\t\t\t\t\t\t<position x=\"#{-vert.x}\" y=\"#{-vert.y}\" z=\"#{-vert.z}\" />" << "\n"
                        stream <<  "\t\t\t\t\t\t<normal x=\"#{vert.nx}\" y=\"#{vert.ny}\" z=\"#{vert.nz}\" />" << "\n"
                        stream <<  "\t\t\t\t\t\t<texcoord u=\"#{vert.u}\" v=\"#{vert.v}\" />" << "\n"
                        stream <<  "\t\t\t\t\t</vertex>" << "\n"
                    }
                    stream <<  "\t\t\t\t</vertexbuffer>" << "\n"
                    stream <<  "\t\t\t</geometry>" << "\n"
                    stream <<  "\t\t</submesh>" << "\n" 
                    
                }
                
                stream <<  "\t</submeshes>" << "\n"    
                stream <<  "</mesh>" << "\n"
                
                f = File.new( File.join(outdir, basename ) +"_"+ mesh_count.to_s + ".xml",  "w")
                print "File written for Entity #{mesh_count}: #{File.join(outdir, basename ) +"_"+ mesh_count.to_s + ".xml"} \n"
                f << stream
                
                mesh_count += 1
            }    
            
            stream = ""
            
            all_textures.each { |t|
                stream <<  "material #{t}" << "\n"
                stream <<  "{" << "\n"
                stream <<  "\ttechnique" << "\n"
                stream <<  "\t{" << "\n"
                stream <<  "\t\tpass" << "\n"
                stream <<  "\t\t{" << "\n"
                stream <<  "\t\t\ttexture_unit" << "\n"
                stream <<  "\t\t\t{" << "\n"
                stream <<  "\t\t\t\ttexture textures/#{t}.#{TextureManager.getSingleton.getTexture(t)[2]}" << "\n"
                stream <<  "\t\t\t\tfiltering anisotropic anisotropic anisotropic" << "\n"
                stream <<  "\t\t\t}" << "\n"
                stream <<  "\t\t}" << "\n"
                stream <<  "\t}" << "\n"
                stream <<  "}" << "\n"
            }
            
            if all_textures.size > 0
                f = File.new( File.join(outdir, basename ) +".material",  "w")
                print "File written for #{all_textures.size} Texture(s): #{File.join(outdir, basename ) +".material"} \n"
                f << stream   
            end
        end
    end
    
end


vec = [0, 0, 1]
#angle = 90.0
#p sin = Math.sin( angle * Math::PI / 180.0 )
#p cos = Math.cos( angle * Math::PI / 180.0 )



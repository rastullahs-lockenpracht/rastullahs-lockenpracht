require "MathHelper.rb"     

module MapConverter
	const_set("MAP_Q3",0)
	const_set("MAP_HL",1)	
	
	class Vertex
		attr_reader :vector, :u, :v, :normal
	
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
		attr_reader :vertices, :texture 
	
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
		
		def sortVertices()			
			return unless @vertices.size > 0
		
			center = MathHelper::Vector.new( [0,0,0] )
			@vertices.each{ |v| center += v.vector }
			center = center/@vertices.size
			
			for i in 0..@vertices.size-3
				smallestAngle = -1.0
				smallestAngleID = -1
			
				ma = MathHelper::Vector.new(  @vertices[i].vector.to_ary ) - center  
				ma.normalize!
				
				for j in i+1..@vertices.size-1
					mb = MathHelper::Vector.new( @vertices[j].vector.to_ary ) - center
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
			
			
		end
		
		def setTexture( tex )
			@texture = tex
		end
	end
	
	class Plane
		# Regulärer Ausdruck für eine HL-Plane-Zeile
		@@re_hl = /\( (.*?) \) \( (.*?) \) \( (.*?) \) (.*?) \[ (.*?) \] \[ (.*?) \] (.*?) (.*?) (.*?)$/
		
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
			@t1 = MathHelper::Vector.new( t1[0], t1[1], t1[2]  )
			@tex_offset_x = t1[3]
			t2 = MathHelper::Vector.new( mD[6].split( " " ).collect { |x| x.to_f } )
			@t2 = MathHelper::Vector.new( t2[0], t2[1], t2[2] )
			@tex_offset_y = t2[3]
			# Rotation und Skalierung
			@rotation =  mD[7].to_i
			@scale_x =  mD[8].to_f
			@scale_y =  mD[9].to_f
			
			calculateHessian(  )
		end
				
		def getTextureCoordinates( vertex )
			width = height = 16
			s = ( vertex.dot(@t1)  + @tex_offset_x  ) / width
			t = ( vertex.dot(@t2)  + @tex_offset_y  ) / height
			
			return MathHelper::Vector.new( [s,t] )
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
				@faces[i].vertices.each { |v|
					@faces[i].toVertex( v, @planes[i].getTextureCoordinates( v ), @planes[i].normal )
				}
				
				#p @faces[i].vertices.size
				#@faces[i].vertices.each{ |v| print v.vector; print "\n" }
				@faces[i].sortVertices()
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
	
		def readMap( filename, facetype )
			@type = facetype
			@map = Map.new( )
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
						entity.addBrush( readBrush() )
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
						raise( "Unexpected Map-Line - OpenBrace for Brush at at line "+@currLine.to_s	)
					when LINETYPE_CLOSEDBRACE				
					# Es endet der Brush
						return brush;
					when LINETYPE_PLANE
					# Plane gehört hier hin
						plane = Plane.new
						plane.parsePlane( line, @facetype )
						brush.addPlane( plane )
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
	
	class MeshWriter
		def initialize( map )
			raise( TypeError, map.class.to_s+" is no Map" ) unless map.class ==  Map
			
			@map = map
		end
	
		def writeMesh( stream )
		
			# Jede Entity ein einzelnes Mesh
			@map.entities.each{ |ent| 
				# Alles faces dieser Entity sammeln
				faces = Array.new
			
				ent.brushes.each{ |brush|
					faces += brush.faces#.slice(0,1)
				}
				
				# Abbruch wenn keine Faces in dieser Entity sind
				break unless faces.size > 0
				
				# Faces in Dreiecke umwandeln
				triangles = FaceConverter.toMeshTriangles( faces )

				# Alle Texturen sammeln
				textures = Array.new
				triangles.each{ |tri| 
					if ! textures.include?( tri.texture )
						textures.push( tri.texture )
					end
				}
				
				stream <<  "<mesh>" << "\n"
				stream <<  "\t<submeshes>" << "\n"
				
				# Jede Textur muss ein einzelnes Submesh sein
				textures.each{ |tex|	
					# Herausfiltern der texturen
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
						stream <<  "\t\t\t\t\t\t<position x=\"#{vert.x}\" y=\"#{vert.y}\" z=\"#{vert.z}\" />" << "\n"
						stream <<  "\t\t\t\t\t\t<normal x=\"#{-vert.nx}\" y=\"#{-vert.ny}\" z=\"#{-vert.nz}\" />" << "\n"
						stream <<  "\t\t\t\t\t\t<texcoord u=\"#{0*vert.u}\" v=\"#{0*vert.v}\" />" << "\n"
						stream <<  "\t\t\t\t\t</vertex>" << "\n"
					}
					stream <<  "\t\t\t\t</vertexbuffer>" << "\n"
					stream <<  "\t\t\t</geometry>" << "\n"
					stream <<  "\t\t</submesh>" << "\n" 
					
				}
				
				stream <<  "\t</submeshes>" << "\n"	
				stream <<  "</mesh>" << "\n"
							
			}	
			
		end
	end
end

map = MapConverter::MapReader.new.readMap( "a.map", MapConverter::MAP_HL )

s = ""
MapConverter::MeshWriter.new( map ).writeMesh( s )
if File.exists?( "../MV/test.mesh" ) 
	File.delete("../MV/test.mesh" )
end
f = File.new("../MV/test.xml",  "w")
f << s



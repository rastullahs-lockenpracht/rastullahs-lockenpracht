require "MapConverter.rb"

if ARGV.size < 1 or ARGV.include? "-help" or ARGV.include? "--help"
    print "\nUsage: MapConverter.rb [switches] mapfile", "\n"
    print "  -tD[directory]\twhere the textures can be found, default './textures'", "\n"
    print "  -tI[ignoreList]\tthe Textures which are to be skipped, separated by ';'", "\n"
    print "  -oD[directory]\tthe Directory where the meshes/material will be stored", "\n"
    print "  -oN[name]\t\tthe basename of the outputfiles", "\n"
    print "\nNotes: Textures have to be in TGA, PNG, JPG or GIF Format, named as in the map", "\n"
    
    exit
end

tDir = "../MV/textures" 
oDir = "."
tIgnore =  ["SKY","NULL"]
mapName = ARGV.pop
oName = nil

ARGV.each{ |i|
    first = i.slice(0,3)
    if first == "-tD"
        tDir = i.slice(3,i.size)
    elsif first == "-tI"
        tIgnore = i.slice(3,i.size).split( ";" )
    elsif first == "-oD"
        oDir = i.slice(3,i.size) 
    elsif first == "-oN"
        oName = i.slice(3,i.size) 
    else
        raise( ArgumentError, "Unknown Switch #{i}" )
    end
}

raise( ArgumentError, "Mapfile '#{mapName}' not found " ) unless mapName != nil && File.exist?( mapName )
raise( ArgumentError, "Texture Directory '#{tDir}' not found " ) unless tDir != nil && File.exist?( tDir )
raise( ArgumentError, "Output Directory '#{oDir}' not found " ) unless tDir != nil && File.exist?( oDir )

if !oName 
    oName = File.basename(mapName, ".map")
end

MapConverter::TextureManager.new( tDir )
map = MapConverter::MapReader.new.readMap( mapName, MapConverter::MAP_HL )
MapConverter::MeshWriter.new( map ).writeMesh( oDir, oName, tIgnore )
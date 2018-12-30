require "RdParser.rb"
require "MaterialParser.rb"
require "pp.rb"

# Construct parser
mp = MaterialParser.new( )

# Get modules directory
basePath = "E:/Programmieren/ms_vs/rl/trunk/rl_dist/modules/"

# Change Dir and search for all material files
Dir.chdir( basePath )
materialFiles = Dir['**/*.{material,program}']

print "Found #{materialFiles.size} different material-files.\n"

skipped = 0;

# Load all material files and parse them
materialFiles.each { |filename|
    begin 
        mp.parseFile( filename )
    rescue RuntimeError => error
        print error.to_s
        print "\n"
        print basePath+filename+":0:"+error.to_s
        print "\n"
        skipped += 1;
    end
}

# All parsed materials
materials = mp.materials

# Collect some infos...
numfiles = 0
materials.each {|m| numfiles+=1 unless not m.firstinfile }
print "Parsed #{materials.size} different materials in #{numfiles} files (skipped #{skipped}).\n\n"


# Collect all 'simple' materials containing just 
#  - One Technique, - One Pass, - One Texture-Unit AND - One texture
#  - NO Basematerial
#  - NO additional attributes
working = []
materials.each {|m| 
    if  m.basename.nil? and
        m.techniques.size == 1 and 
        m.techniques.first.attributes.empty? and
        m.techniques.first.passes.size == 1 and 
        m.techniques.first.passes.first.attributes.empty? and
        m.techniques.first.passes.first.tus.size == 1 and
        m.techniques.first.passes.first.vprs.empty? and
        m.techniques.first.passes.first.scvprs.empty? and
        m.techniques.first.passes.first.fprs.empty? and
        m.techniques.first.passes.first.tus.first.getAttributes( "texture" ).size == 1
        working << m
    end
}

print "Filtered " + working.size.to_s + " materials."

#working.each{ |m| print m.to_s }

exit

# Save the materials
numfiles = 0
materials.each {|m| 
    if( m.firstinfile )
        modes = File::CREAT|File::TRUNC|File::WRONLY  
        numfiles = numfiles +1
    else
        modes = File::CREAT|File::APPEND|File::WRONLY
    end
    f = File.open( basePath+"/"+m.filename, modes )
    f << m.to_s
    f.close
}
print "Saved #{materials.size} different materials in #{numfiles} files.\n\n"
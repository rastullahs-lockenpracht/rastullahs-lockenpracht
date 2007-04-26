require "RdParser.rb"
require "MaterialParser.rb"
require "pp.rb"

# Construct parser
mp = MaterialParser.new( )

# Get modules directory
basePath = "E:/Programmieren/ms_vs/rl/trunk/rl_dist/modules/"

# Change Dir and search for all material files
Dir.chdir( basePath )
materialFiles = Dir['**/*.material']

print "Found #{materialFiles.size} different material-files.\n"

# Load all material files and parse them
materialFiles.each { |filename|
    begin 
        mp.parseFile( filename )
    rescue RuntimeError => error
        print error.to_s
        print "\n"
        print basePath+filename+":0:"+error.to_s
        print "\n"
        break
    end
}

# All parsed materials
materials = mp.materials

# Collect some infos...
numfiles = 0
materials.each {|m| numfiles+=1 unless not m.firstinfile }
print "Parsed #{materials.size} different materials in #{numfiles} files.\n\n"

# Save the materials
materials.each {|m| 
    if( m.firstinfile )
        modes = File::CREAT|File::TRUNC|File::WRONLY  
    else
        modes = File::CREAT|File::APPEND|File::WRONLY
    end
    f = File.open( basePath+"/"+m.filename, modes )
    f << m.to_s
    f.close
}

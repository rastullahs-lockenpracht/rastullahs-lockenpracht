require "RdParser.rb"
require "MaterialParser.rb"
require "pp.rb"


basePath = "E:/Programmieren/ms_vs/rl/trunk/rl_dist/modules/"
Dir.chdir( basePath )
allMaterials = Dir['**/*.material']


mp = MaterialParser.new( )

allMaterials.each { |filename|
    begin 
        mp.parseFile( filename )
    rescue RuntimeError => error
        print error.to_s
        print "\n"
        print  basePath+filename+":0:"+error.to_s
        break
    end

    pp mp
    exit
}

#pp mp
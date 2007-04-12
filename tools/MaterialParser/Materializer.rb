require "RdParser.rb"
require "MaterialParser.rb"
require "pp.rb"


basePath = "E:/RL/rl/rl_dist/modules/"
Dir.chdir( basePath )
allMaterials = Dir['**/*.material']


mp = MaterialParser.new( )

allMaterials.each { |filename|
    parseMe = File.new( filename ).read
    begin
        mp.parseString( parseMe )
    rescue RuntimeError => error
        print error
        print "\n"
        print  basePath+filename+":0:"+error
        break
    end
}

#pp mp
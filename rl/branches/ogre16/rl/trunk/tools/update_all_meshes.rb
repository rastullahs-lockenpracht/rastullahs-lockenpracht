# Path to modules directory
modPath = "../rl_dist/modules/"
# Command to start (from modules)
runMe = '../OgreMeshUpgrade.exe'


################################################################################
# Class for correct execution of system commands under windows
class ExecCmd
        attr_reader :output,:cmd,:exec_time
        #When a block is given, the command runs before yielding
        def initialize( cmd )
            @cmd=cmd
            @cmd_run=cmd+" 2>&1" unless cmd=~/2>&1/
            if block_given?
                run
                yield self
            end
        end
        #Runs the command
        def run()
            t1=Time.now
            IO.popen(@cmd_run){|f|
                @output=f.read
                @process=Process.waitpid2(f.pid)[1]
            }
            @exec_time=Time.now-t1
        end
        #Returns false if the command hasn't been executed yet
        def run?()
            return false unless @process
            return true
        end
        #Returns the exit code for the command. Runs the command if it hasn't run yet.
        def exitcode()
            run unless @process
            @process.exitstatus
        end
        #Returns true if the command was succesfull.
        #
        #Will return false if the command hasn't been executed
        def success?()
            return @process.success? if @process
            return false
        end
end 

################################################################################

# Change Dir and search for all mesh files
Dir.chdir( modPath )
basePath = Dir.pwd + "/"
meshes = Dir['**/*.{mesh}']


print "Found #{meshes.size} different mesh-files.\n\n"

skipped = 0;
count = 0;

# Sync print() calls
STDOUT.sync = 1
# Update all meshes
meshes.each { |filename|   
    begin 
        runCommand = basePath + runMe + ' -b "' + basePath + filename + '"'
        cmd = ExecCmd.new( runCommand );
        cmd.run

        if not cmd.success? then
            raise( RuntimeError, "Command not executed\n" + cmd.output )
        end
        count += 1
        print "Updated #{filename} (#{count})                                          \r";
 
    rescue RuntimeError => error
        print basePath+filename+":0:"+error.to_s
        skipped += 1
        exit
    end
}

print "\nUpdated #{meshes.size-skipped} mesh files (skipped #{skipped}).\n\n"
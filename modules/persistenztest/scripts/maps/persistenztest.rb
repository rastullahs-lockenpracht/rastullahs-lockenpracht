load "embed.rb"

require 'player.rb'
require 'hero.rb'
require 'messagebox.rb'
require 'MapChange.rb'

# Physik aktivieren
$PM.setEnabled(true)

$SCRIPT.log("init map 'regressiontest'...")



$SCRIPT.log("begin tests.")


class PersistenzTest
	def initialize
		@mErrors = 0
		@mTests = 0
		@mFailedTests = Array.new()
	end

	def runTest(test, coords)
		p "Start test " + test.to_s()
		@mTests += 1
		testErrors = test.new(coords).run()
		if testErrors != 0
			@mErrors += testErrors
			@mFailedTests << test.to_s()
		end
		p "Finished test " + test.to_s()
	end
	
	def runTests()
		p $MAPLOADER
		
		$PM.setEnabled(false)
		#changeMap($MAPLOADER, ["persistenztest02.scene","hero.scene"])
		$PM.setEnabled(true)
		
		if @mErrors > 0
		  MessageBox.showModal(
			@mErrors.to_s() + " of the " + @mTests.to_s() +  " tests were not successful.\nNot working:\n" + @mFailedTests.join(", "),
			"Regressiontest Results",
			MessageBox::OK)
		end

		$SCRIPT.log("tests initialisiert.")
	end
end

PersistenzTest.new().runTests()
$SCRIPT.log("map 'persistenztest' initialisiert.")

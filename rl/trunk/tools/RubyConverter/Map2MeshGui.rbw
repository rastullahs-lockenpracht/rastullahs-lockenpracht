require 'fox'

include Fox

class Map2MeshGui < FXMainWindow
    def setText
        @text.value = "Hund, du"
    end

    def initialize(app)
        # Initialize base class
        super(app, "Map2MeshGui")
        
        @text = FXDataTarget.new("")
        
        top = FXVerticalFrame.new(self, LAYOUT_FILL_X|LAYOUT_FILL_Y) { |theFrame|
            theFrame.padLeft = 10
            theFrame.padRight = 10
            theFrame.padBottom = 10
            theFrame.padTop = 10
            theFrame.vSpacing = 10
        }
    
        p = proc { setText }
        
        FXLabel.new(top, 'Enter Text:') { |theLabel|
            theLabel.layoutHints = LAYOUT_FILL_X
        }
        
        FXTextField.new(top, 20, @text, FXDataTarget::ID_VALUE) { |theTextField|
            theTextField.layoutHints = LAYOUT_FILL_X
            theTextField.setFocus()
        }
        
        FXButton.new(top, 'Pig It') { |pigButton|
            pigButton.connect(SEL_COMMAND, p)
            pigButton.layoutHints = LAYOUT_CENTER_X
        }
        
        FXButton.new(top, 'Exit') { |exitButton|
            exitButton.connect(SEL_COMMAND) { exit }
            exitButton.layoutHints = LAYOUT_CENTER_X
        }
    end
  
    def create
        super
        show(PLACEMENT_SCREEN)
    end
end

if __FILE__ == $0
    app = FXApp.new("Map2MeshGui", "FXRuby")
    Map2MeshGui.new(app)
    app.create
    app.run
end



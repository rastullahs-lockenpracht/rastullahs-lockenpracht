
require 'rubygems'
require 'wx'
require 'wx_sugar'

module Rl
    
    
    class MainApp < Wx::App
        def on_init
            $xml = Wx::XmlResource.get();
            $xml.init_all_handlers();
            # Load a resource file
            xrc_file = File.join( File.dirname(__FILE__), 'main_frame.xrc' )
            $xml.load(xrc_file)
            
            frame = $xml.load_frame(nil, 'MainFrame')
            init_frame(frame)
            frame.show()
        end
        
        private
        def init_frame(frame)
            frame.maximize(true)
            
        end
    end
    
end

Rl::MainApp.new.main_loop()
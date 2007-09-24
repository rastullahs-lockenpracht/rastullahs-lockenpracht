class DebugKeys
    def initialize()
        @isWPShown = false
    end

    def toggleWP()
      if @isWPShown:
        $DVM.hide(8)
        p "hide"
      else
      	$DVM.show(8)
        p "show"
      end
      @isWPShown = (@isWPShown == false)
    end
end


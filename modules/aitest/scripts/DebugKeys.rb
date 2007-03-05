class DebugKeys
    def initialize()
        @isWPShown = false
    end

    def toggleWP()
      if @isWPShown:
	$DVM.hide(8)
      else
	$DVM.show(8)
      end
      @isWPShown = (@isWPShown == false)
    end
end


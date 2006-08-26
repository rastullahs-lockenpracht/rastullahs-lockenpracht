class TestCase
    def initialize(center)
        @center = center
    end
    def rel_pos(position)
        vector_add(@center, position)
    end
end


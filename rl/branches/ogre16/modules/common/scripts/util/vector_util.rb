#Functions to calculate with vectors represented as arrays
def vector_add(a, b)
    rval = []
    a.each_index { |i| rval[i] = a[i] + b[i] }
    rval
end

require 'matrix'
require 'mathn.rb'

module MathHelper

	### Instance of this class are matrices..
	class Matrix < ::Matrix
	
		### Class constants
		Version = /([\d\.]+)/.match( %q$Revision: 1.1 $ )[1]
		Rcsid = %q$Id: Matrix.rb,v 1.1 2003/02/04 11:28:21 deveiant Exp $
	
	end # class Matrix
    
	class Vector
		include Enumerable
	
		### Class constants
		Version = /([\d\.]+)/.match( %q{$Revision: 1.3 $} )[1]
		Rcsid = %q$Id: Vector.rb,v 1.3 2003/02/18 01:45:34 deveiant Exp $
	
		X = 0; Y = 1; Z = 2
	
	
		### Instantiate and return a new Vector. Defaults to a 3rd-order
		### zero vector if no arguments are given.
		def initialize( *args )
		    args.flatten!
	
		    case args.length
			    when 0
				@elements = [ 0.0, 0.0, 0.0 ]
		
			    when 1
				if args[0].respond_to?( :to_ary )
				    @elements = args[0].to_ary.clone
				elsif !args[0].is_a?( Numeric )
				    raise TypeError, "no implicit conversion from %s to Numeric" %
					[ args[0].class.name ]
				else
				    @elements = [args[0]]
				end
			   else
				args.each{ |a| 
					raise TypeError, "no implicit conversion from %s to Numeric" %[ args[0].class.name ]  unless a.is_a?( Numeric )
				} 
				@elements  = args;
		    end
		end
	
	
	
		######
		public
		######
	
		# Internal array of elements
		attr_accessor :elements
	
	
		### Returns true if the receiver is a zero vector.
		def isZeroVector?
		    return @elements.find_all {|i| i.zero?}.length == 0
		end
		alias_method :zero?, :isZeroVector?
	
	
		### Returns true if the vector has a length close to 1.0, measured in
		### the Euclidean norm.
		def isUnitVector?
		    return (1.0 - self.mag) < 1e-10
		end
	
	
		### Element reference operator -- returns the <tt>i</tt>th element of
		### the vector.
		def [](i)
		    @elements[i]
		end
	
	
		### Element assignment operator -- assigns the value <tt>x</tt> to the
		### <tt>i</tt>th element of the vector.
		def []=(i, x)
		    @elements[i] = Float(x)
		end
	
	
		### Returns the magnitude of the vector, measured in the Euclidean norm.
		def mag		    
		    #sum = 0.0
		    #@elements.each{ |i| sum += i*i }
		    #return Math.rsqrt(  sum.abs()  )
		    return Math.rsqrt(  self.dot( self ) ) 
		end
		alias_method :abs, :mag
		alias_method :length, :mag
	
	
		### Return a distinct copy of the vector (as opposed to #dup, which only
		### returns a shallow copy).
		def copy
		    return Vector.new( self )
		end
	
		### Return a vector collinear to the given vector and having a length of
		### 1.0, measured in the Euclidean norm. This will fail if the receiver
		### vector is the zero vector.
		def normalize
		    self.copy.normalize!
		end
		alias_method :normalized, :normalize
	
	
		### Normalizes the vector in place.
		def normalize!
		    mag = self.mag
		    @elements = @elements.each_index {|i| @elements[i] = @elements[i] / mag }
		    return self
		end
	
	
		### Return the dot-product of the receiving vector and
		### <tt>otherVector</tt>.
		def dot( otherVector )
		    raise ArgumentError,
			"Cannot calculate the dot-product of vectors of different dimensions" unless
			self.size == otherVector.size
			
		    scalar = 0.0
		    @elements.each_index {|i| scalar += @elements[i] * otherVector[i]    }
							  
		    return scalar
		end
	
	
		### Return the geometric product of the receiver and the
		### <tt>otherVector</tt> as an Quaternion.
		def gp( otherVector )
		    raise TypeError,
			"wrong argument type '%s': Expected an Vector" % otherVector.class.name unless
			otherVector.is_a?( Vector )
		    raise ArgumentError,
			"Cannot calculate geometric product of vectors of different dimensions." unless
			self.size == otherVector.size
	
		    Quaternion::sv2q( self.dot(v), self.cross(v) )
		end
	
	
		### Return the cross-product of the receiver and the
		### <tt>otherVector</tt> as a new instance of the receiving class.
		def cross( otherVector )
		    raise TypeError,
			"wrong argument type '%s': Expected an Vector" % otherVector.class.name unless
			otherVector.is_a?( Vector )
		    raise ArgumentError, "Can only cross 3rd-order vectors" unless
			self.size == 3 and otherVector.size == 3
	
		    self.class.new( [@elements[1]*otherVector[2] - @elements[2]*otherVector[1],
				          @elements[2]*otherVector[0] - @elements[0]*otherVector[2],
				          @elements[0]*otherVector[1] - @elements[1]*otherVector[0]])
		end
	
	
		### Call the given block once with each element in the vector.
		def each(&block) 
		    @elements.each(&block)
		end
	
		
	
		### Return a new instance of the receiver by calling the specified block
		### once for each element, passing that element and the equivalent
		### element from <tt>otherVector</tt>. The new object will be created
		### with the Array of all the return values of the block.
		def collect2( otherVector )
		    ary = []
		    @elements.each_with_index {|elem, i| ary << yield(elem, otherVector[i]) }
		    self.class.new( *ary )
		end
		alias :map2 :collect2
	
	
		### Addition operator -- Add the receiving vector to the
		### <tt>otherVector</tt> (which must be of the same #size), and return
		### the result as a new instance of the receiver.
		def +( otherVector )
		    raise ArgumentError, "Cannot add vectors of different dimensions" unless
			self.size == otherVector.size
	
		    rvec = self.copy
		    @elements.each_index {|i| rvec[i] += otherVector[i] }
		    return rvec
		end
	
	
		### Subtraction operator -- Subtract the <tt>otherVector</tt> from the
		### receiving vector (which must be of the same #size), and return the
		### result as a new instance of the receiver.
		def -( otherVector )
		    raise ArgumentError, "Cannot add vectors of different dimensions" unless
			self.size == otherVector.size
	
		    rvec = self.copy
		    @elements.each_index {|i| rvec[i] -= otherVector[i] }
		    return rvec
		end
	
	
		### Multiplication operator -- Multiply the receiving vector with the
		### specified <tt>scalar</tt> and return the result as a new instance of
		### the receiver.
		def *( scalar )
		    scalar = Float(scalar)
		    return self.class.new( self.collect {|elem| elem * scalar} )
		end
	
	
		### Division operator -- Divide the receiving vector by the specified
		### <tt>scalar</tt> and return the result as a new instance of the
		### receiver.
		def /( scalar )
		    scalar = Float(scalar)
		    raise ZeroDivisionError if scalar == 0.0
	
		    return self.class.new( self.collect {|elem| elem / scalar} )
		end
	
	
		### Returns the order of the vector.
		def size
		    @elements.size
		end
	
	
		### Return the receiver as an Array of its elements.
		def to_ary
		    return @elements.to_ary
		end
	
	
		### Return a nicely stringified version of the vector
		def to_s
		    return "|%s|" % @elements.collect {|e| "%0.2f" % e}.join(", ")
		end
	
	
		### Equality operator -- returns true if the receiver and
		### <tt>otherObj</tt> are of the same class, and each element of the
		### receiver is the same as the corresponding element of the
		### <tt>otherObj</tt>.
		def ==( otherObj )
		    return false unless otherObj.is_a?( self.class )
		    return self.to_ary == otherObj.to_ary
		end
	
	
		### Similarity tests -- returns true if the receiver and
		### <tt>otherObj</tt> are of the same class, and each element of the
		### receiver is within 1e-10 of the corresponding element of the
		### <tt>otherObj</tt>.
		def similarTo?( otherObj )
		    return false unless otherObj.is_a?( self.class )
		    otherAry = otherObj.to_ary
		    rval = true
		    self.to_ary.each_with_index {|elem,i|
			if (elem - otherAry[i]).abs > 1e-10
			    rval = false
			    break
			end
		    }
		    return rval
		end
	end # class Vector
end
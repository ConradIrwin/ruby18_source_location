class Example

  def example1

  end

  define_method(:example2) {

  }

  Example3 = Proc.new do

  end

  Example4 = lambda {

  }

  define_method(:example5, Example3)

  define_method(:example6, &Example4)

  define_method(:example7, instance_method(:example1))

  define_method(:example8, &Example.new.method(:example2))

  Example9 = Proc.new &Example3

  Example10 = Proc.new &Example.new.method(:example1)

  eval <<-EVAL
    def example11

    end
  EVAL

  eval <<-EVAL, binding, __FILE__, __LINE__+1
    def example12

    end
  EVAL

  def self.example13

  end

  alias example14 example1

  alias_method :example15, :example2

  attr_reader :example16

  attr_accessor :example17

  def example18(&block)
    block
  end

  Example19 = lambda &method(:puts)
end



# vim: number

require 'test/unit'

puts "Testing with #{RUBY_VERSION}"
if Method.method_defined?(:source_location)
  warn "using BUILTING source_location"
else
  require "#{File.dirname(__FILE__)}/../ext/ruby18_source_location"
end
require "#{File.dirname(__FILE__)}/examples"

class TestRuby18SourceLocation < Test::Unit::TestCase

  def assert_on_line(line, method)
    assert_equal [File.expand_path('./examples.rb'), line], method.source_location
  end

  def assert_same_source(m1, m2)
    assert_equal m1.source_location, m2.source_location
  end

  def test_normal_method
    assert_on_line 3, Example.instance_method(:example1)
  end

  def test_define_method_with_block
    assert_on_line 7, Example.instance_method(:example2)
  end

  def test_proc_new
    assert_on_line 11, Example::Example3
  end

  def test_lambda
    assert_on_line 15, Example::Example4
  end

  def test_define_method_with_proc_arg
    assert_same_source Example::Example3, Example.new.method(:example5)
  end

  def test_define_method_with_amp_proc
    assert_same_source Example::Example4, Example.new.method(:example6)
  end

  def test_define_method_with_method_arg
    assert_same_source Example.new.method(:example1), Example.new.method(:example7)
  end

  def test_define_method_with_amp_method
    assert_same_source Example.new.method(:example2), Example.new.method(:example8)
  end

  def test_proc_with_amp_proc
    assert_same_source Example::Example3, Example::Example9
  end

  def test_proc_with_amp_method
    assert_same_source Example.instance_method(:example1), Example::Example10
  end

  def test_eval
    assert_equal ['(eval)', 1], Example.instance_method(:example11).source_location
  end

  def test_eval_with_overridden_line
    assert_on_line 38, Example.new.method(:example12)
  end

  def test_class_method
    assert_on_line 43, Example.method(:example13)
  end

  def test_alias
    assert_same_source Example.new.method(:example1), Example.new.method(:example14)
  end

  def test_alias_method
    assert_same_source Example.new.method(:example2), Example.new.method(:example15)
  end

  def test_c_method
    assert_equal nil, method(:puts).source_location
  end

  def test_attr_reader
    assert_on_line 51, Example.instance_method(:example16)
  end

  def test_attr_accessor
    assert_on_line 53, Example.instance_method(:example17)
  end

  def test_attr_assign
    assert_on_line 53, Example.instance_method(:example17=)
  end

  def test_blocks
    assert_equal [File.expand_path(__FILE__), __LINE__], Example.new.example18{

    }.source_location
  end

  def test_lambdas_with_c_methods
    assert_equal nil, Example::Example19.source_location
  end
end

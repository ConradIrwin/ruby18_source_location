
Gem::Specification.new do |s|
  s.name = "ruby18_source_location"
  s.version = "0.2"
  s.platform = Gem::Platform::RUBY
  s.author = "Conrad Irwin"
  s.email = "conrad.irwin@gmail.com"
  s.homepage = "http://github.com/ConradIrwin/ruby18_source_location"
  s.summary = "Add .source_location to methods in Ruby 1.8.7"
  s.description = "Allows you to make use of lots of ruby 1.9.2 specific goodness"
  s.files = `git ls-files`.split("\n")
  s.extensions = "ext/extconf.rb"
  s.require_path = "ext"
end

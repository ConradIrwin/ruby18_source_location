
def dir(*args); Dir.chdir(File.join(File.dirname(__FILE__), *args)); end
desc "Compile local copy"
task :compile do
  dir('ext')
  system "ruby extconf.rb && make"
end

task :clean do
  dir('ext')
  system "make clean"
end

desc "Package the gem"
task :gem do
  dir('.')
  system "gem build ruby18_source_location.gemspec"
end

desc "Run the tests"
task :test do
  dir("test")
  system "ruby test_ruby18_source_location.rb"
end

desc "Run an interactive shell"
task :pry do
  dir('.')
  system "pry -f -r ext/ruby18_source_location -r test/examples"
end

task :default => [:clean, :compile, :test]

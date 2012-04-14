
def dir(*args); Dir.chdir(File.join(File.dirname(__FILE__), *args)); end

desc "Compile local copy"
task :compile do
  cd("ext") do
    system "ruby extconf.rb"
    system "make"
  end
end

task :clean do
  cd("ext") do
    system "make clean"
  end
end

desc "Package the gem"
task :gem do
  dir('.')
  system "gem build ruby18_source_location.gemspec"
end

desc "Package the windows gem"
task :win_gem => [:clean, :compile] do
  dir = File.dirname(__FILE__)
  cd(dir) do
    system "cp #{dir}/ext/*.so #{dir}/lib/"
    system "gem build ruby18_source_location_mswin32.gemspec"
    system "gem build ruby18_source_location_mingw32.gemspec"
  end
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

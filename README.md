The `ruby18_source_location` gem backports the `source_location` instance method of `Method`, `UnboundMethod` and
`Proc` from Ruby 1.9.

This allows you to use `Method#source_location` on ruby 1.8.7, and also all the other gems that depend on it.

Usage
-----

    gem install ruby18_source_location

```ruby
    require 'rubygems'
    require 'ruby18_source_location'

    def example
      2
    end

    puts method(:example).source_location.inspect
    # => ["/tmp/example.rb", 4]

    puts Proc.new{}.source_location.inspect
    # => ["/tmp/example.rb", 11]

    puts method(:puts).source_location.inspect
    # => nil
```

Known issues
------------

Under Ruby-1.8.7, it's not currently possible to get the `source_location` of methods defined by `attr_reader`,
`attr_writer` or `attr_accessor`, unless you are running Ruby Enterprise Edition.

Meta-foo
--------

This is made available under the MIT license (see LICENSE.MIT), contributions and bug reports are welcome.

Gem::Specification.new do |spec|
  spec.name              = 'mysql2-keepalive'
  spec.version           = '0.1.0'
  spec.summary           = 'TCP keepalive extension for Mysql2'
  spec.description       = 'TCP keepalive extension for Mysql2. see tcp(7).'
  spec.require_paths     = %w(lib)
  spec.files             = Dir.glob('ext/mysql2/*.{c,h}') + %w(ext/mysql2/extconf.rb README)
  spec.author            = 'winebarrel'
  spec.email             = 'sgwr_dts@yahoo.co.jp'
  spec.homepage          = 'https://bitbucket.org/winebarrel/mysql2-keepalive'
  spec.extensions        = 'ext/mysql2/extconf.rb'
  spec.has_rdoc          = false
  spec.add_dependency('mysql2', '~> 0.3.11').
end

# copy from https://github.com/brianmario/mysql2/blob/master/ext/mysql2/extconf.rb

# encoding: UTF-8
require 'mkmf'

def asplode lib
  abort "-----\n#{lib} is missing.  please check your installation of mysql and try again.\n-----"
end

# borrowed from mysqlplus
# http://github.com/oldmoe/mysqlplus/blob/master/ext/extconf.rb
dirs = ENV['PATH'].split(File::PATH_SEPARATOR) + %w[
  /opt
  /opt/local
  /opt/local/mysql
  /opt/local/lib/mysql5
  /usr
  /usr/mysql
  /usr/local
  /usr/local/mysql
  /usr/local/mysql-*
  /usr/local/lib/mysql5
].map{|dir| "#{dir}/bin" }

GLOB = "{#{dirs.join(',')}}/{mysql_config,mysql_config5}"

if RUBY_PLATFORM =~ /mswin|mingw/
  inc, lib = dir_config('mysql')
  exit 1 unless have_library("libmysql")
elsif mc = (with_config('mysql-config') || Dir[GLOB].first) then
  mc = Dir[GLOB].first if mc == true
  ver = `#{mc} --version`.chomp.to_f
  cflags = `#{mc} --cflags`.chomp
  exit 1 if $? != 0
  libs = `#{mc} --libs_r`.chomp
  # MySQL 5.5 and above already have re-entrant code in libmysqlclient (no _r).
  if ver >= 5.5 || libs.empty?
    libs = `#{mc} --libs`.chomp
  end
  exit 1 if $? != 0
  $CPPFLAGS += ' ' + cflags
  $libs = libs + " " + $libs
else
  inc, lib = dir_config('mysql', '/usr/local')
  libs = ['m', 'z', 'socket', 'nsl', 'mygcc']
  while not find_library('mysqlclient', 'mysql_query', lib, "#{lib}/mysql") do
    exit 1 if libs.empty?
    have_library(libs.shift)
  end
end

if have_header('mysql.h') then
  prefix = nil
elsif have_header('mysql/mysql.h') then
  prefix = 'mysql'
else
  asplode 'mysql.h'
end

# GCC specific flags
if RbConfig::MAKEFILE_CONFIG['CC'] =~ /gcc/
  $CFLAGS << ' -Wall -funroll-loops'

  if hard_mysql_path = $libs[%r{-L(/[^ ]+)}, 1]
    $LDFLAGS << " -Wl,-rpath,#{hard_mysql_path}"
  end
end

create_makefile('mysql2/keepalive')

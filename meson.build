project('my-meson-project', 'cpp',
  version : '0.1',
  meson_version : '>= 0.61.0',
  default_options : [
    'warning_level=3',
    'buildtype=debugoptimized',
    'cpp_std=c++17',
    'b_sanitize=address,undefined' # https://github.com/google/sanitizers
  ])

catch2_dep = dependency('catch2', fallback : ['catch2', 'catch2_with_main_dep'], version: '>=3.7.1')

include_directories = [
  include_directories('.'),
  include_directories('src/utils'),
  include_directories('src/parser'),
  include_directories('src/system'),
  include_directories('src/parser/antlr4'),
  include_directories('src/parser/antlr4/generated'),
]

subdir('src')

if get_option('enable_tests')
  subdir('tests')
endif
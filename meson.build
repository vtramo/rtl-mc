project('rtl-mc', 'cpp',
  version : '0.1',
  meson_version : '>= 0.64.0',
  default_options : [
    'warning_level=3',
    'buildtype=debugoptimized',
    'cpp_std=c++17',
    'b_sanitize=none' # https://github.com/google/sanitizers
  ])

src_dir = 'src'
parser_dir = 'parser'
system_dir = 'system'
utils_dir = 'utils'
include_directories = [
  include_directories('.'),
  include_directories(src_dir / utils_dir),
  include_directories(src_dir / utils_dir / 'ppl'),
  include_directories(src_dir / utils_dir / 'spot'),
  include_directories(src_dir / system_dir),
  include_directories(src_dir / 'discretization'),
  include_directories(src_dir / 'automaton'),
  include_directories(src_dir / parser_dir),
  include_directories(src_dir / parser_dir / 'rtlf'),
  include_directories(src_dir / parser_dir / system_dir),
  include_directories(src_dir / 'core'),
  include_directories(src_dir / 'cli'),
  include_directories(src_dir / 'logging'),
]

subdir(src_dir)

if get_option('enable_tests') == true
  catch2_dep = dependency('catch2', fallback : ['catch2', 'catch2_with_main_dep'], version: '>=3.7.1')
  subdir('tests')
endif
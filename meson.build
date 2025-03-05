project('rtl-mc', 'cpp',
  version : '0.1',
  meson_version : '>= 0.64.0',
  default_options : [
    'warning_level=3',
    'buildtype=debugoptimized',
    'cpp_std=c++17',
    'b_sanitize=none' # https://github.com/google/sanitizers
  ])

if get_option('enable_profiling') == true
  add_global_arguments('-pg', language : 'cpp')
endif

src_dir = 'src'
parser_dir = 'parser'
system_dir = 'system'
automata_dir = 'automata'
utils_dir = 'utils'
mcsolvers_dir = 'mcsolvers'
onthefly_dir = 'onthefly'
abstraction_dir = 'abstractions'
omnidirectional_dir = 'omnidirectional'
include_directories = [
  include_directories('.'),
  include_directories(src_dir / utils_dir),
  include_directories(src_dir / utils_dir / 'ppl'),
  include_directories(src_dir / utils_dir / 'spot'),
  include_directories(src_dir / system_dir),
  include_directories(src_dir / abstraction_dir),
  include_directories(src_dir / abstraction_dir / omnidirectional_dir),
  include_directories(src_dir / abstraction_dir / omnidirectional_dir / 'tilextractor'),
  include_directories(src_dir / abstraction_dir / 'general'),
  include_directories(src_dir / 'discretisation'),
  include_directories(src_dir / 'operators'),
  include_directories(src_dir / 'product'),
  include_directories(src_dir / automata_dir),
  include_directories(src_dir / automata_dir / 'onthefly'),
  include_directories(src_dir / parser_dir),
  include_directories(src_dir / parser_dir / 'rtlf'),
  include_directories(src_dir / parser_dir / system_dir),
  include_directories(src_dir / parser_dir / 'mc'),
  include_directories(src_dir / mcsolvers_dir),
  include_directories(src_dir / mcsolvers_dir / onthefly_dir),
  include_directories(src_dir / mcsolvers_dir / onthefly_dir / 'concurrent'),
  include_directories(src_dir / 'cli'),
  include_directories(src_dir / 'logger'),
  include_directories(src_dir / 'stats'),
]

subdir(src_dir)

if get_option('enable_tests') == true
  catch2_subproject = subproject('catch2', version: '>=3.7.1')
  catch2_dep = catch2_subproject.get_variable('catch2_dep')
  catch2_with_main_dep = catch2_subproject.get_variable('catch2_with_main_dep')
  subdir('tests')
endif
project('chronicle-benchmark', 'cpp',
  default_options: ['cpp_std=c++17'])

incdirs = include_directories(
    '../include',
    '../thirdparty/include'
)

sources = [
    'benchmark.cpp',
    '../thirdparty/source/format.cc',
    '../thirdparty/source/os.cc'
]

executable('chronicle-benchmark',
            sources, include_directories: incdirs)

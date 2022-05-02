project('chronicle', 'cpp',
    license: 'MIT',
    default_options: [
        'cpp_std=c++20',
        'warning_level=3'])

incdirs = include_directories('./include')

date = declare_dependency(
    version: '3.0.1',
    include_directories: incdirs
)


hydra_deps = [dependency('threads')]

system = host_machine.system()
if system == 'windows'
  cpp = meson.get_compiler('cpp')
  synch_api = cpp.find_library('synchronization', required: true)
  hydra_deps += [synch_api]
endif

hydra = declare_dependency(
    include_directories: incdirs,
    dependencies: hydra_deps
)

ufmt = declare_dependency(
    include_directories: incdirs
)

headers = [
    'include/chronicle/data_log.hpp',
    'include/chronicle/message.hpp',
    'include/chronicle/severity.hpp',
    'include/chronicle/sink.hpp',
    'include/chronicle/structured_log.hpp',
    'include/chronicle/text_log.hpp',
    'include/chronicle/traits.hpp',
    'include/chronicle/sinks/conerr.hpp',
    'include/chronicle/sinks/conout.hpp',
    'include/chronicle/sinks/daily_rotated_file.hpp',
    'include/chronicle/sinks/file.hpp',
    'include/chronicle/fields/format.hpp',
    'include/chronicle/fields/severity.hpp',
    'include/chronicle/fields/severity_marker.hpp',
    'include/chronicle/fields/source.hpp',
    'include/chronicle/fields/thread_id.hpp',
    'include/chronicle/fields/utc_time_ms.hpp',
    'include/chronicle/fields/utc_time_only_ms.hpp',
    'include/chronicle/fields/utc_time_only_us.hpp',
    'include/chronicle/fields/utc_time_us.hpp'
]


chronicle = declare_dependency(
    version: meson.project_version(),
    include_directories: incdirs,
    sources: headers,
    dependencies: [date, hydra]
)

subdir('benchmark')
subdir('test')
subdir('stand')

install_headers(headers, subdir: 'chronicle')

pkg = import('pkgconfig')
pkg.generate(
    name: 'chronicle',
    filebase: 'chronicle',
    subdirs: ['chronicle'],
    version: meson.project_version(),
    description: 'C++17 header-only library for logging'
)

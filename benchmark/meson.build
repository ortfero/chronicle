nanolog = declare_dependency(
    include_directories: '../thirdparty/nanolog/include',
    sources: ['../thirdparty/nanolog/src/NanoLog.cpp']
)


reckless_sources = [
        '../thirdparty/reckless/src/basic_log.cpp',
        '../thirdparty/reckless/src/fd_writer.cpp',
        '../thirdparty/reckless/src/file_writer.cpp',
        '../thirdparty/reckless/src/lockless_cv.cpp',
        '../thirdparty/reckless/src/mpsc_ring_buffer.cpp',
        '../thirdparty/reckless/src/ntoa.cpp',
        '../thirdparty/reckless/src/output_buffer.cpp',
        '../thirdparty/reckless/src/platform.cpp',
        '../thirdparty/reckless/src/policy_log.cpp',
        '../thirdparty/reckless/src/template_formatter.cpp',
        '../thirdparty/reckless/src/trace_log.cpp',
        '../thirdparty/reckless/src/unit_test.hpp',
        '../thirdparty/reckless/src/writer.cpp'
]

if host_machine.system() == 'windows'
    reckless_sources += [
        '../thirdparty/reckless/src/crash_handler_win32.cpp',
        '../thirdparty/reckless/src/spsc_event_win32.cpp'
    ]
else
    reckless_sources += [
        '../thirdparty/reckless/src/crash_handler_unix.cpp'
    ]
endif

reckless = declare_dependency(
    include_directories: '../thirdparty/reckless/include',
    sources: reckless_sources
)


spdlog = declare_dependency(
    include_directories: '../thirdparty/spdlog/include'
)


sources = [
    'benchmark.cpp'
]

executable('chronicle-benchmark',
            sources, dependencies: [chronicle, nanolog, reckless, spdlog])

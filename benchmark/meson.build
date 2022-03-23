nanolog = declare_dependency(
    include_directories: '../thirdparty/include',
    sources: ['../thirdparty/source/nanolog/NanoLog.cpp']
)

reckless = declare_dependency(
    include_directories: '../thirdparty/include',
    sources: [
        '../thirdparty/source/reckless/basic_log.cpp',
        '../thirdparty/source/reckless/crash_handler_unix.cpp',
        '../thirdparty/source/reckless/crash_handler_win32.cpp',
        '../thirdparty/source/reckless/fd_writer.cpp',
        '../thirdparty/source/reckless/file_writer.cpp',
        '../thirdparty/source/reckless/lockless_cv.cpp',
        '../thirdparty/source/reckless/mpsc_ring_buffer.cpp',
        '../thirdparty/source/reckless/ntoa.cpp',
        '../thirdparty/source/reckless/output_buffer.cpp',
        '../thirdparty/source/reckless/platform.cpp',
        '../thirdparty/source/reckless/policy_log.cpp',
        '../thirdparty/source/reckless/spsc_event_win32.cpp',
        '../thirdparty/source/reckless/template_formatter.cpp',
        '../thirdparty/source/reckless/trace_log.cpp',
        '../thirdparty/source/reckless/unit_test.hpp',
        '../thirdparty/source/reckless/writer.cpp
    ]
)


sources = [
    'benchmark.cpp'
]

executable('chronicle-benchmark',
            sources, dependencies: [chronicle, nanolog, reckless])

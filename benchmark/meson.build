nanolog = declare_dependency(
    include_directories: '../thirdparty/nanolog/include',
    sources: ['../thirdparty/nanolog/src/NanoLog.cpp']
)


spdlog = declare_dependency(
    include_directories: '../thirdparty/spdlog/include'
)


sources = [
    'benchmark.cpp'
]

executable('chronicle-benchmark',
            sources, dependencies: [chronicle, nanolog, spdlog])

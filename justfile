project := "chronicle"
test-file := project + "-test"
bench-file := project + "-bench"
stand-file := project + "-stand"
flags := "-std=c++20 -Iinclude -Ithirdparty/include"
debug-flags := flags + " -g -O0"
release-flags := flags + " -O3 -DNDEBUG"
nanolog := `echo thirdparty/src/nanolog/*.cpp`
spdlog := `echo thirdparty/src/spdlog/*.cpp`
thirdparty := nanolog + " " + spdlog


alias b := build

default: test

build-test:
    mkdir -p build
    c++ test/test.cpp \
        -o build/{{test-file}} {{debug-flags}}

build-bench:
    mkdir -p build
    c++ -DSPDLOG_COMPILED_LIB benchmark/benchmark.cpp {{thirdparty}} \
        -o build/{{bench-file}} {{release-flags}}

build-stand:
    mkdir -p stand
    c++ stand/stand.cpp \
        -o build/{{stand-file}} {{release-flags}}

build: build-test build-bench build-stand

test: build-test
    build/{{test-file}}
    rm ./data.log ./test-*.log

bench: build-bench
    build/{{bench-file}}
    rm ./spd.log ./test.log ./nanolog.*.txt

stand: build-stand
    build/{{stand-file}}

clean:
    rm -rf build


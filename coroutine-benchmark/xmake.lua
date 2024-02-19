set_default(false)
set_group("coroutine-benchmark")

add_requires("benchmark")

target("coroutine-benchmark")
    set_kind("binary")
    add_files("src/*.cpp")
    add_packages("benchmark")
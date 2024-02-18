add_rules("mode.debug", "mode.release")

set_languages("cxx23")
set_warnings("all", "error")

-- well, clang++ is not clang
add_cxxflags("clang++::-stdlib=libc++")

add_plugindirs(path.join(os.projectdir(), "plugins"))

includes("3rd")
includes("ds")
includes("coroutine-benchmark")
includes("link-test")

add_rules("mode.debug", "mode.release")

set_languages("cxx23")
set_warnings("all", "error")

add_cxxflags("-stdlib=libc++", {tools = "clang"})

includes("3rd")
includes("ds")
includes("coroutine-benchmark")

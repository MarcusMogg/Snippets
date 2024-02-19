add_rules("mode.debug", "mode.release")

set_languages("cxx23")
set_warnings("all", "error")

add_plugindirs(path.join(os.projectdir(), "plugins"))

includes("3rd")
includes("ds")
includes("coroutine-benchmark")
includes("link-test")

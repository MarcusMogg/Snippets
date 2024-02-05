add_rules("mode.debug", "mode.release")

set_languages("cxx23")
set_warnings("all", "error")

includes("3rd")
includes("ds")
includes("coroutine-benchmark")

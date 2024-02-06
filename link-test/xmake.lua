target("hpp-a")
    set_kind("static")
    add_files("hpp/a.cc")

target("hpp-main")
    set_kind("binary")
    add_deps("hpp-a")
    add_files("hpp/main.cc")


set_policy("build.c++.modules", true)

target("module-base")
    set_kind("static")
    add_files("modules/base.ixx")

target("module-a")
    set_kind("static")
    add_deps("module-base")
    add_files("modules/a.cc")

target("module-b")
    set_kind("static")
    add_deps("module-base")
    add_files("modules/b.cc","modules/b.ixx")

target("module-main")
    set_kind("binary")
    add_deps("module-a","module-b","module-base")
    add_files("modules/main.cc")


add_requires("catch2")
-- add_requires("gtest")

set_policy("build.c++.modules", true)

target("ds-normal")
    set_kind("static")
    add_files("normal/*.ixx")
    set_group("ds/normal")

target("ds-test")
    set_kind("binary")
    add_files("test/*.cpp")
    add_deps("ds-normal")
    add_packages("catch2")
    -- add_packages("gtest")
    -- for visual studio
    set_group("ds/test")
    -- for 3rd error
    set_warnings("none")
    -- https://xmake.io/#/zh-cn/manual/project_target?id=targetadd_tests
    set_default(false)
    add_tests("all")



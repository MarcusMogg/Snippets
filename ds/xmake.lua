
add_requires("catch2")
-- add_requires("gtest")

set_policy("build.c++.modules", true)

target("ds-normal")
    set_kind("static")
    add_files("normal/*.ixx")
    -- remove_files("normal/test_*.cc")
    set_group("ds/normal")

target("ds-test")
    set_kind("binary")
    add_deps("ds-normal")
    add_packages("catch2")
    -- add_packages("gtest")
    -- for visual studio
    set_group("ds/test")
    -- for 3rd error
    set_warnings("none")
    -- https://xmake.io/#/zh-cn/manual/project_target?id=targetadd_tests
    add_files("normal/*_test.cc")
    set_default(false)
    add_tests("all")



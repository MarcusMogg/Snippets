set_policy("build.c++.modules", true)

target("third_party_wrapper")
    set_kind("static")
    add_files("*.ixx")
    set_group("third_party")

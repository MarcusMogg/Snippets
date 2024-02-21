task("my-config")
    set_category("plugin")
    on_run(function ()
        import("core.base.task")
        import("core.base.option")

        local mode = option.get("mode");
        if mode == "llvm" then 
            -- xmake f --toolchain=llvm --cxxflags=-stdlib=libc++ --ldflags=-stdlib=libc++ 
            task.run("config", {
                    toolchain = "llvm", 
                    cxxflags="-stdlib=libc++",
                    ldflags="-stdlib=libc++"
                })
        elseif mode == 'win' then
        end
    end)
    set_menu {
        usage = "xmake my-config [options]",
        description = "Quick set env config",
        shortname = 'mf',
        options = {
            {'', "mode", "v", nil, "Set clean mod", " - local", " - global"},
        }
    }
task("clean-all")
    set_category("plugin")
    on_run(function ()
        import("core.base.option")
        -- see XMAKE_PROGRAM_DIR/plugins/show/lists/envs.lua
        -- https://xmake.io/#/zh-cn/guide/configuration?id=xmake_globaldir
        import("core.base.global")

        local mode = option.get("mode");
        if mode == "local" then 
            -- https://xmake.io/#/zh-cn/manual/builtin_variables
            os.rm("$(buildir)")
            os.rm(path.join("$(projectdir)",".xmake"))
        elseif mode == 'global' then
            os.rm("$(buildir)")
            os.rm(path.join("$(projectdir)",".xmake"))
            os.rm(global.directory())
        end
    end)
    -- https://xmake.io/#/zh-cn/manual/plugin_task?id=taskset_menu
    set_menu {
        usage = "xmake clean-all [options]",
        description = "Quick clean xmake cache",
        shortname = 'ca',
        options = {
            {'', "mode", "v", nil, "Set clean mod", " - local", " - global"},
        }
    }
-- 定义一个名叫hello的插件任务
task("build-and-run")
    set_category("plugin")
    on_run(function ()
        import("core.base.task")
        import("core.base.option")

        local target_name = option.get("target");
        task.run("build", {target = target_name, all = false})
        task.run("run", {target = target_name})
    end)
    set_menu {
        usage = "xmake build-and-run [options]",
        description = "Quick build and run target",
        shortname = 'br',
        options = {
            {nil, "target", "v", nil, ""},
        }
    }
-- set xmake version
set_xmakever("2.9.4")

-- include local folders
includes("extern/commonlibobr")

-- set project
set_project("BakaAchievementEnabler-OBR")
set_version("1.0.0")
set_license("GPL-3.0")

-- set defaults
set_arch("x64")
set_languages("c++23")
set_optimize("faster")
set_warnings("allextra", "error")
set_defaultmode("releasedbg")

-- enable lto
set_policy("build.optimization.lto", true)

-- add rules
add_rules("mode.debug", "mode.releasedbg")
add_rules("plugin.vsxmake.autoupdate")

-- setup targets
target("BakaAchievementEnabler-OBR")
    -- bind local dependencies
    add_deps("commonlibobr")

    -- add commonlibobr plugin
    add_rules("commonlibobr.plugin", {
        name = "BakaAchievementEnabler-OBR",
        author = "shad0wshayd3"
    })

    -- define exports
    set_kind("shared")
    add_files("src/main.def")

    -- add source files
    add_files("src/**.cpp")
    add_headerfiles("src/**.h")
    add_includedirs("src")
    set_pcxxheader("src/PCH.h")

    -- add extra files
    add_extrafiles(".clang-format")

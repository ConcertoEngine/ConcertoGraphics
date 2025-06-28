add_rules("mode.debug", "mode.release", "mode.releasedbg")
add_repositories("Concerto-xrepo https://github.com/ConcertoEngine/xmake-repo.git main")
add_repositories("nazara-repo https://github.com/NazaraEngine/xmake-repo")

add_requires("imgui", {configs = {vulkan = true, sdl2 = true, debug = is_mode("debug"), with_symbols = true}})
add_requires("volk", {configs = {header_only = true}})
add_requires("concerto-core", { debug = true, configs = { asserts = true, shared = true }})
add_requires("vulkan-headers", "vulkan-memory-allocator", "stb", "libsdl2", "nzsl", "vulkan-utility-libraries", "parallel-hashmap")
add_requires("nazaraengine", { debug = is_mode("debug"), configs = { graphics = false, textrenderer = false, renderer = false, widgets = false, plugin_assimp = false, plugin_ffmpeg = false, plugin_imgui = false, entt = false, audio = false, physics2d = false, physics3d  = false, platform = false }})

option("override_runtime", { description = "Override vs runtime to MD in release and MDd in debug", default = true })
option("examples", { description = "Build examples", default = false })
option("profiling", { description = "Build with tracy profiler", default = false })
add_defines("CCT_ENABLE_ASSERTS")

if is_plat("windows") and has_config("override_runtime") then
    set_runtimes(is_mode("debug") and "MDd" or "MD")
end

if has_config("profiling") then
    add_requires("tracy[shared]", {configs = {tracy_enable = true, cmake = false}})

    target("concerto-profiler")
        set_kind("shared")
        add_includedirs("Include/", { public = true })
        add_headerfiles("Include/(Concerto/Graphics/Profiler/*.hpp)")
        add_packages("tracy", {public = true})
        add_packages("concerto-core", {public = false})
        add_defines("CCT_GFX_PROFILING", {public = true})
        add_files("Src/Profiler/**.cpp")
    target_end()
end

if has_config("examples") then
    add_requires("glslang", {configs = {binaryonly = true}})
    add_rules("utils.glsl2spv", {outputdir = "$(buildir)/$(plat)/$(arch)/$(mode)/Shaders"})
    add_files("Shaders/*.vert", "Shaders/*.frag")
    --add_rules("download.assets", "compile.shaders")
end

target("concerto-graphics")
    set_kind("shared")
    set_languages("cxx20")
    set_warnings("allextra")
    if is_mode("debug") then
        set_symbols("debug")
    end
    add_defines("CONCERTO_GRAPHICS_BUILD", { public = false })
    add_files("Src/Camera.cpp", "Src/Primitives.cpp", "Src/Camera.cpp", "Src/Window/**.cpp", "Src/DisplayManager.cpp")
    add_includedirs("Include/", { public = true })
    add_headerfiles("Include/(Concerto/Graphics/*.hpp)", "Include/(Concerto/Graphics/*.hpp)", "Include/(Concerto/Graphics/Window/*.hpp)")
    add_packages("concerto-core", "libsdl2", "vulkan-headers", "imgui", { public = true })
    if has_config("profiling") then
        add_deps("concerto-profiler", { public = false })
    end

target("concerto-vulkan-backend")
    set_kind("shared")
    set_languages("cxx20")
    set_warnings("allextra")
    if is_mode("debug") then
        set_symbols("debug")
    end
    
    add_defines("CONCERTO_GRAPHICS_VULKAN_BACKEND_BUILD", { public = false })
    add_defines("VK_NO_PROTOTYPES", { public = true })
    add_files("Src/Backend/Vulkan/**.cpp")
    add_includedirs("Include/", { public = true })
    add_headerfiles("Include/(Concerto/Graphics/Backend/Vulkan/*.hpp)",
                    "Include/(Concerto/Graphics/Backend/Vulkan/Wrapper/*.hpp)",
                    "Include/(Concerto/Graphics/Backend/Vulkan/Wrapper/*.inl)")
    add_packages("concerto-core", "volk", "vulkan-headers", "vulkan-utility-libraries", "vulkan-memory-allocator", "nzsl", { public = true })
    add_deps("concerto-graphics")
    if has_config("profiling") then
        add_deps("concerto-profiler", { public = false })
    end

target("concerto-rhi-module")
    set_kind("shared")
    set_languages("cxx20")
    set_warnings("allextra")
    if is_mode("debug") then
        set_symbols("debug")
    end
    add_packages("stb", { public = false })
    add_defines("CONCERTO_GRAPHICS_RHI_MODULE_BUILD", { public = false })
    add_files("Src/RHI/**.cpp")
    add_includedirs("Include/", { public = true })
    add_headerfiles("Include/(Concerto/Graphics/RHI/*.hpp)",
                    "Include/(Concerto/Graphics/RHI/*.inl)",
                    "Include/(Concerto/Graphics/RHI/Vulkan/*.hpp)",
                    "Include/(Concerto/Graphics/RHI/Vulkan/*.inl)")
    add_packages("concerto-core", "parallel-hashmap", { public = true })
    add_packages("nazaraengine", { public = false })
    add_deps("concerto-vulkan-backend")
    if has_config("profiling") then
        add_deps("concerto-profiler", { public = false })
    end

includes("Xmake/Rules/*.lua")

if has_config("examples") then
    includes("Examples/xmake.lua")
end

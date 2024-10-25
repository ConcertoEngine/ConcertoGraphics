add_rules("mode.debug")
add_repositories("Concerto-xrepo https://github.com/ConcertoEngine/xmake-repo.git main")
add_repositories("nazara-repo https://github.com/NazaraEngine/xmake-repo")
add_requires("imgui", {configs = {vulkan = true, glfw = true, debug = is_mode("debug"), with_symbols = true}})
add_requires("volk", {configs = {header_only = true}})
add_requires("concerto-core", "vulkan-headers", "vulkan-memory-allocator", "stb", "glfw", "nzsl")

if (has_config("examples")) then
    add_requires("glslang", {configs = {binaryonly = true}})
    add_rules("utils.glsl2spv", {outputdir = "$(buildir)/$(plat)/$(arch)/$(mode)/Shaders"})
    add_files("Shaders/*.vert", "Shaders/*.frag")
    add_rules("download.assets", "compile.shaders")
end

target("ConcertoGraphics")
    set_kind("shared")
    set_languages("cxx20")
    set_warnings("allextra")
    if is_mode("debug") then
        set_symbols("debug")
    end
    add_defines("CONCERTO_GRAPHICS_BUILD")
    add_files("Src/Camera.cpp", "Src/Primitives.cpp", "Src/Camera.cpp", "Src/Window/**.cpp")
    add_includedirs("Include/", { public = true })
    add_headerfiles("Include/(Concerto/Graphics/*.hpp)", "Include/(Concerto/Graphics/*.hpp)", "Include/(Concerto/Graphics/Window/*.hpp)")
    add_packages("concerto-core", "glfw", "vulkan-headers", "imgui", { public = true })

target("VulkanBackend")
    set_kind("shared")
    set_languages("cxx20")
    set_warnings("allextra")
    if is_mode("debug") then
        set_symbols("debug")
    end
    add_defines("CONCERTO_GRAPHICS_VULKAN_BACKEND_BUILD")
    add_defines("VK_NO_PROTOTYPES", { public = true })
    add_files("Src/Backend/Vulkan/**.cpp")
    add_includedirs("Include/", { public = true })
    add_headerfiles("Include/(Concerto/Graphics/Backend/Vulkan/*.hpp)",
                    "Include/(Concerto/Graphics/Backend/Vulkan/Wrapper/*.hpp)",
                    "Include/(Concerto/Graphics/Backend/Vulkan/Wrapper/*.inl)")
    add_packages("concerto-core", "volk", "vulkan-headers", "vulkan-memory-allocator", "nzsl", { public = true })
    add_packages("stb", { public = false })
    add_deps("ConcertoGraphics")

target("RHIModule")
    set_kind("shared")
    set_languages("cxx20")
    set_warnings("allextra")
    if is_mode("debug") then
        set_symbols("debug")
    end
    add_defines("CONCERTO_GRAPHICS_RHI_MODULE_BUILD")
    add_files("Src/RHI/**.cpp")
    add_includedirs("Include/", { public = true })
    add_headerfiles("Include/(Concerto/Graphics/RHI/*.hpp)",
                    "Include/(Concerto/Graphics/RHI/*.inl)",
                    "Include/(Concerto/Graphics/RHI/Vulkan/*.hpp)",
                    "Include/(Concerto/Graphics/RHI/Vulkan/*.inl)")
    add_packages("concerto-core", { public = true })
    add_deps("VulkanBackend")


includes("Xmake/Rules/*.lua")
includes("Examples/xmake.lua")

add_rules("mode.debug")
add_repositories("Concerto-xrepo https://github.com/ConcertoEngine/xmake-repo.git main")
add_repositories("nazara-repo https://github.com/NazaraEngine/xmake-repo")
add_requires("imgui", {configs = {glfw_vulkan = true, debug = is_mode("debug"), with_symbols = true}})
add_requires("concerto-core", "vulkan-loader", "vulkan-memory-allocator", "stb", "glfw", "nzsl")

if (has_config("examples")) then
    add_requires("glslang", {configs = {binaryonly = true}})
    add_rules("utils.glsl2spv", {outputdir = "$(buildir)/$(plat)/$(arch)/$(mode)/Shaders"})
    add_files("Shaders/*.vert", "Shaders/*.frag")
    add_rules("download.assets", "compile.shaders")
end

target("Window")
    set_kind("shared")
    set_languages("cxx20")
    set_warnings("allextra")
    if is_mode("debug") then
        set_symbols("debug")
    end
    add_defines("CONCERTO_GRAPHICS_WINDOW_BUILD")
    add_files("Src/Window/**.cpp")
    add_includedirs("Include/", { public = true })
    add_headerfiles("Include/(Concerto/Graphics/Window/*.hpp)")
    add_packages("concerto-core", "glfw", { public = true })

target("VulkanBackend")
    set_kind("shared")
    set_languages("cxx20")
    set_warnings("allextra")
    if is_mode("debug") then
        set_symbols("debug")
    end
    add_defines("CONCERTO_GRAPHICS_VULKAN_BACKEND_BUILD")
    add_files("Src/Backend/Vulkan/**.cpp", "Src/Window/**.cpp") --fixme
    add_includedirs("Include/", { public = true })
    add_headerfiles("Include/(Concerto/Graphics/Backend/Vulkan/*.hpp)",
                    "Include/(Concerto/Graphics/Backend/Vulkan/Wrapper/*.hpp)",
                    "Include/(Concerto/Graphics/Backend/Vulkan/Wrapper/*.inl)",
                    "Include/(Concerto/Graphics/Window/*.hpp)") --fixme
    add_packages("concerto-core", { public = true })
    add_packages("vulkan-loader", "vulkan-memory-allocator", "stb", { public = false })
    add_packages("glfw", { public = true }) --fixme

target("RHIBase")
    set_kind("shared")
    set_languages("cxx20")
    set_warnings("allextra")
    if is_mode("debug") then
        set_symbols("debug")
    end
    add_defines("CONCERTO_GRAPHICS_RHI_BASE_BUILD")
    add_files("Src/RHI/*.cpp")
    add_includedirs("Include/", { public = true })
    add_headerfiles("Include/(Concerto/Graphics/RHI/*.hpp)",
                    "Include/(Concerto/Graphics/RHI/*.inl)")
    add_packages("concerto-core", { public = true })

target("VkRHI")
    set_kind("shared")
    set_languages("cxx20")
    set_warnings("allextra")
    if is_mode("debug") then
        set_symbols("debug")
    end
    add_defines("CONCERTO_GRAPHICS_VKRHI_BUILD")
    add_files("Src/RHI/Vulkan/**.cpp")
    add_includedirs("Include/", { public = true })
    add_headerfiles("Include/(Concerto/Graphics/RHI/Vulkan/*.hpp)",
                    "Include/(Concerto/Graphics/RHI/Vulkan/*.inl)")
    add_deps("VulkanBackend", "RHIBase")


target("ConcertoGraphics")
    set_kind("shared")
    set_languages("cxx20")
    set_warnings("allextra")
    if is_mode("debug") then
        set_symbols("debug")
    end
    add_defines("CONCERTO_GRAPHICS_BUILD")
    add_files("Src/**.cpp")
    add_includedirs("Include/", { public = true })

    add_headerfiles("Include/(Concerto/Graphics/*.hpp)",
                    "Include/(Concerto/Graphics/thirdParty/*.h)",
                    "Include/(Concerto/Graphics/Window/*.hpp)",
                    "Include/(Concerto/Graphics/Backend/*.hpp)",
                    "Include/(Concerto/Graphics/Backend/*.inl)",
                    "Include/(Concerto/Graphics/RHI/*.hpp)",
                    "Include/(Concerto/Graphics/RHI/*.inl)",
                    "Include/(Concerto/Graphics/RHI/Vulkan/*.hpp)",
                    "Include/(Concerto/Graphics/RHI/Vulkan/*.inl)",
                    "Include/(Concerto/Graphics/Backend/Vulkan/*.hpp)",
                    "Include/(Concerto/Graphics/Backend/Vulkan/Wrapper/*.hpp)",
                    "Include/(Concerto/Graphics/Backend/Vulkan/Wrapper/*.inl)")

    add_packages("concerto-core",
                 "vulkan-memory-allocator",
                 "stb",
                 "imgui",
                 "nzsl",
                 { public = true })
    add_deps("VkRHI")

includes("Xmake/Rules/*.lua")
includes("Examples/xmake.lua")

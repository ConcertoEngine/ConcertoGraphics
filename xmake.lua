add_rules("mode.debug")
add_requires('vulkan-headers' ,'vulkan-loader','vulkan-memory-allocator','vk-bootstrap','glm','stb',"glfw", "vulkan-validationlayers")
add_requires("imgui", {configs = {glfw_vulkan = true}})

target("ConcertoGraphics")
    set_kind("binary")
    set_symbols("debug")
    if is_mode("debug") then
        add_defines("DEBUG")
    end
    set_warnings("everything")
    set_languages("cxx20")
    set_optimize("none")
    add_files('src/*.cpp', 'src/Vulkan/Wrapper/*.cpp', 'src/window/*.cpp', 'src/Vulkan/*.cpp')
    add_includedirs('include', 'include/thirdParty', 'include/window', 'include/Vulkan/Wrapper', 'include/Vulkan')
    add_packages('vulkan-headers', 'vulkan-loader', 'vulkan-memory-allocator', 'vk-bootstrap', 'glm', 'stb', 'glfw', "vulkan-validationlayers", "imgui")
    add_rules('utils.glsl2spv', {outputdir = '$(buildir)/$(plat)/$(arch)/$(mode)/shaders'})
    add_files('shaders/*.vert', 'shaders/*.frag')
    add_packages('glslang')
    after_build(function (target)
        import("net.http")
        import("utils.archive")

        local binaryPath = "$(buildir)/$(plat)/$(arch)/$(mode)"
        os.mkdir(binaryPath .. "/assets")

        print("Copying shaders to build directory...")
        os.cp("shaders/**",  binaryPath .. "/shaders")
        print("Copying shaders to build directory... Done !")

        if (not os.exists("assets/sponza")) then
            print("Downloading assets...")
            http.download("https://casual-effects.com/g3d/data10/common/model/crytek_sponza/sponza.zip", "assets/sponza.zip")
            print("Downloading assets... Done !")

            print("Extracting assets...")
                archive.extract("assets/sponza.zip", "assets/sponza")
                os.rm("assets/sponza.zip")
            print("Extracting assets... Done !")
        else
            print("Assets already downloaded !")
        end

        print("Copying assets to build directory...")
        os.cp("assets/**",  binaryPath .. "/assets")
        print("Copying assets to build directory... Done !")
    end)

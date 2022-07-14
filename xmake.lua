add_rules("mode.debug")
add_requires('vulkan-headers' ,'vulkan-loader','vulkan-memory-allocator','vk-bootstrap','glm','stb',"glfw", "vulkan-validationlayers")

target("ConcertoGraphics")
    set_kind("binary")
    set_symbols("debug")
    if is_mode("debug") then
        add_defines("DEBUG")
    end
    set_warnings("everything")
    set_languages("cxx20")
    set_optimize("none")
    add_files('src/*.cpp', 'src/wrapper/*.cpp', 'src/window/*.cpp')
    add_includedirs('include', 'include/thirdParty', 'include/window')
    add_packages('vulkan-headers', 'vulkan-loader', 'vulkan-memory-allocator', 'vk-bootstrap', 'glm', 'stb', 'glfw', "vulkan-validationlayers")
    add_rules('utils.glsl2spv', {outputdir = '$(buildir)/$(plat)/$(arch)/$(mode)/shaders'})
    add_files('shaders/*.vert', 'shaders/*.frag')
    add_packages('glslang')

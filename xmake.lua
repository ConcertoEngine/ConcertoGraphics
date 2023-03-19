add_rules('mode.debug')
add_repositories('Concerto-xrepo https://github.com/ConcertoEngine/xmake-repo.git main')
add_requires('imgui', {configs = {glfw_vulkan = true}})

if (has_config('examples')) then
    add_requires("glslang", {configs = {binaryonly = true}})
    add_rules('utils.glsl2spv', {outputdir = '$(buildir)/$(plat)/$(arch)/$(mode)/Shaders'})
    add_files('Shaders/*.vert', 'Shaders/*.frag')
    add_rules('download.assets', 'compile.shaders')
end

local packagesList = {
    'ConcertoCore',
    'vulkan-loader',
    'vulkan-memory-allocator',
    'glm',
    'stb',
    'glfw'
}

local function AddPackagesToTarget(packages)
    for _, package in ipairs(packages) do
        add_requires(package)
        add_packages(package, { public = true })
    end
end

local function AddIncludesToTarget(includes)
    for _, include in ipairs(includes) do
        add_includedirs(include, { public = true })
    end
end

AddPackagesToTarget(packagesList)

target('ConcertoGraphics')
    set_kind('shared')
    set_symbols('debug')
    if is_mode('debug') then
        add_defines('DEBUG')
    end
    add_defines('CONCERTO_BUILD')
    set_languages('cxx20')
    add_files('Src/*.cpp', 'Src/Vulkan/Wrapper/*.cpp', 'Src/window/*.cpp', 'Src/Vulkan/*.cpp')
    AddIncludesToTarget({'Include/Concerto/Graphics', 'Include/Concerto/Graphics/thirdParty', 'Include/Concerto/Graphics/window', 'Include/Concerto/Graphics/Vulkan/Wrapper', 'Include/Concerto/Graphics/Vulkan'})
    add_headerfiles('Include/(Concerto/Graphics/*.hpp)', 'Include/(ConcertoGraphics/thirdParty/**.h)', 'Include/(Concerto/Graphics/Window/**.hpp)', 'Include/(Concerto/Graphics/Vulkan/Wrapper/*.hpp)', 'Include/(Concerto/Graphics/Vulkan/*.hpp)')
    add_packages('imgui', { public = true })

includes('Xmake/Rules/*.lua')
includes('Examples/xmake.lua')

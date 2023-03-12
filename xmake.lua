add_rules('mode.debug')
add_repositories('Concerto-xrepo https://github.com/ConcertoEngine/xmake-repo.git main')
add_requires('ConcertoCore', 'vulkan-loader','vulkan-memory-allocator', 'glm','stb','glfw', 'vulkan-validationlayers')
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
    'glfw',
    'vulkan-validationlayers',
    'imgui'
}

local function AddPackagesToTarget(packages)
    for _, package in ipairs(packages) do
        add_packages(package, { public = true })
    end
end

local function AddIncludesToTarget(includes)
    for _, include in ipairs(includes) do
        add_includedirs(include, { public = true })
    end
end

target('ConcertoGraphics')
    set_kind('shared')
    set_symbols('debug')
    if is_mode('debug') then
        add_defines('DEBUG')
    end
    add_defines('CONCERTO_BUILD')
    set_languages('cxx20')
    add_files('Src/*.cpp', 'Src/Vulkan/Wrapper/*.cpp', 'Src/window/*.cpp', 'Src/Vulkan/*.cpp')
    AddIncludesToTarget({'Include', 'Include/thirdParty', 'Include/window', 'Include/Vulkan/Wrapper', 'Include/Vulkan'})
    AddPackagesToTarget(packagesList)

includes('xmake/rules/*.lua')
includes('Examples/xmake.lua')

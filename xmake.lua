add_rules('mode.debug')
add_repositories('Concerto-xrepo https://github.com/ConcertoEngine/xmake-repo.git main')
add_repositories("nazara-repo https://github.com/NazaraEngine/xmake-repo")
add_requires('imgui', {configs = {glfw_vulkan = true, debug = is_mode("debug"), with_symbols = true}})
add_requires('concerto-core', 'vulkan-loader', 'vulkan-memory-allocator', 'stb', 'glfw', 'nzsl')

if (has_config('examples')) then
    add_requires("glslang", {configs = {binaryonly = true}})
    add_rules('utils.glsl2spv', {outputdir = '$(buildir)/$(plat)/$(arch)/$(mode)/Shaders'})
    add_files('Shaders/*.vert', 'Shaders/*.frag')
    add_rules('download.assets', 'compile.shaders')
end

target('ConcertoGraphics')
    set_kind('shared')
    set_languages('cxx20')
    set_warnings('allextra')
    add_cxxflags("cl::/Zc:preprocessor", { public = true })
    if is_mode('debug') then
        set_symbols('debug')
    end
    add_defines('CONCERTO_GRAPHICS_BUILD')
    add_files('Src/**.cpp')
    add_includedirs('Include/',
                    'Include/Concerto/',
                    'Include/Concerto/Graphics',
                    'Include/Concerto/Graphics/Backend',
                    'Include/Concerto/Graphics/Backend/Vulkan/Wrapper',
                    'Include/Concerto/Graphics/Backend/Vulkan',
                    'Include/Concerto/Graphics/RHI',
                    'Include/Concerto/Graphics/RHI/Vulkan',
                    'Include/Concerto/Graphics/thirdParty',
                    'Include/Concerto/Graphics/Window',
                    { public = true })

    add_headerfiles('Include/(Concerto/Graphics/*.hpp)',
                    'Include/(Concerto/Graphics/thirdParty/*.h)',
                    'Include/(Concerto/Graphics/Window/*.hpp)',
                    'Include/(Concerto/Graphics/Backend/*.hpp)',
                    'Include/(Concerto/Graphics/Backend/*.inl)',
                    'Include/(Concerto/Graphics/RHI/*.hpp)',
                    'Include/(Concerto/Graphics/RHI/*.inl)',
                    'Include/(Concerto/Graphics/RHI/Vulkan/*.hpp)',
                    'Include/(Concerto/Graphics/RHI/Vulkan/*.inl)',
                    'Include/(Concerto/Graphics/Backend/Vulkan/*.hpp)',
                    'Include/(Concerto/Graphics/Backend/Vulkan/Wrapper/*.hpp)',
                    'Include/(Concerto/Graphics/Backend/Vulkan/Wrapper/*.inl)')

    add_packages('concerto-core',
                 'vulkan-loader',
                 'vulkan-memory-allocator',
                 'stb',
                 'glfw',
                 'imgui',
                 'nzsl',
                 { public = true })

includes('Xmake/Rules/*.lua')
includes('Examples/xmake.lua')

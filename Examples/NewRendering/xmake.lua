target('NewRendering')
    add_deps('ConcertoGraphics')
    add_packages('ConcertoGraphics')
    add_files('main.cpp')
    set_languages('cxx20')
    set_warnings('everything')
    add_rules('download.assets', 'compile.shaders')
    add_defines("GLM_ENABLE_EXPERIMENTAL")
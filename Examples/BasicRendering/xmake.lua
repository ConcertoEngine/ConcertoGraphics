target('BasicRendering')
    add_deps('ConcertoGraphics')
    add_packages('ConcertoGraphics')
    add_files('main.cpp')
    add_defines('CONCERTO_BUILD')
    set_languages('cxx20')
    add_rules('download.assets', 'compile.shaders')

env=Environment(CPPPATH='src',
                CPPDEFINES=[],
                #CXX='clang++',
                LIBS=['glfw','GLEW','GL','boost_filesystem','boost_system'],
                CCFLAGS = ['-g',
                           '-Wall',
                           '--std=c++11'])
                #            '-fcolor-diagnostics'])


# Notice the source files are referred to in the build dir
# If you dont do this, the compiled objects will be in the src dirs
mainSources = ['build/main.cpp',
               'build/world/world.cpp',
               'build/world/cube.cpp',
               'build/world/surface.cpp',
               'build/world/noise.cpp',
               'build/world/instance.cpp',
               'build/utils/fileHandler.cpp',
               'build/render/camera.cpp',
               'build/render/light.cpp',
               'build/render/utils.cpp',
               'build/render/renderer.cpp',
               'build/render/shader.cpp',
               'build/render/window_mgr.cpp']

env.VariantDir(variant_dir = 'build/', src_dir = 'src', duplicate = 0)
env.VariantDir(variant_dir = 'build/render', src_dir = 'src/render', duplicate = 0)
env.VariantDir(variant_dir = 'build/utils', src_dir = 'src/utils', duplicate = 0)
env.VariantDir(variant_dir = 'build/world', src_dir = 'src/input', duplicate = 0)

env.Program(target = 'quarfs', source = [mainSources])
# env.Program(target = 'test', source = ['build/test.cpp'])

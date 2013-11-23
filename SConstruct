
env=Environment(CPPPATH='src',
                CPPDEFINES=[],
                CXX='clang++',
                LIBS=['X11','GLU','GL','Xext' ],
                CCFLAGS = ['--std=c++11',
                          '-fcolor-diagnostics'])


# Notice the source files are referred to in the build dir
# If you dont do this, the compiled objects will be in the src dirs
mainSources = ['build/main.cpp',
               'build/world.cpp']

env.VariantDir(variant_dir = 'build/', src_dir = 'src', duplicate = 0)

env.Program(target = 'cubeLand', source = [mainSources])

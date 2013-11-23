
env=Environment(CPPPATH='src',
                CPPDEFINES=[],
                LIBS=['X11','GLU','GL','Xext' ],
                CCFLAGS = '--std=c++11') 

if ARGUMENTS.get('debug', 0):
    env.Append(CCFLAGS = ' -g')

env.Program('main', Glob('./src/*.cpp'))

Import("env")
import datetime

ts = datetime.datetime.now().strftime("%Y-%m-%dT%H:%M:%S")

#env.Append(CPPDEFINES=[("BUILD_TIME",  env.StringifyMacro(ts))])

# General options that are passed to the C and C++ compilers
env.Append(CCFLAGS=[])

# General options that are passed to the C compiler (C only; not C++).
env.Append(CFLAGS=[])

# General options that are passed to the C++ compiler
env.Append(CXXFLAGS=["-fpermissive"])

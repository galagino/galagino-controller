Import("env")

# General options that are passed to the C and C++ compilers
env.Append(CCFLAGS=[])

# General options that are passed to the C compiler (C only; not C++).
env.Append(CFLAGS=[])

# General options that are passed to the C++ compiler
env.Append(CXXFLAGS=["-fpermissive"])

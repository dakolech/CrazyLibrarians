solution "Distracted Processing"

    project "CrazyLibrarians"
        premake.gcc.cc = 'mpicc'
    
        configurations { "debug", "release" }
            language "C"
            kind "ConsoleApp"
            files "src/**.c"
            buildoptions { "-w -Wall" }
            linkoptions { "-lmpi" }
            objdir "obj/src"

        configuration { "debug" }
            targetdir "debug"
            flags "Symbols"

        configuration { "release" }
            targetdir "release"
            flags "OptimizeSpeed"

        if _ACTION == "clean" then
            os.rmdir("debug")
            os.rmdir("release")
            os.rmdir("obj")
        end
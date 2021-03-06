solution "Distracted Processing"

    project "CrazyLibrarians"
        premake.gcc.cc = 'mpic++'
    
        configurations { "debug", "release" }
            language "C++"
            kind "ConsoleApp"
            files "src/**.cpp"
            buildoptions { "-w -Wall -std=c++11 -I/usr/include/openmpi" }
            linkoptions { "-lmpi -lmpi_cxx" }
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

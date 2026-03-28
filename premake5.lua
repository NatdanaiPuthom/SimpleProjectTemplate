local name = "SimpleProject"

local projects = {
	Launcher = "Launcher",
	External = "External",
	UnitTest = "UnitTest",
	Engine   = "Engine"
}

local directories = {}
directories["Root"]					= os.realpath("\\")
directories["Bin"]					= os.realpath("Bin\\")
directories["Local"]				= os.realpath("Local\\")
directories["Source"]				= os.realpath("Source\\")
directories["Temp"]					= os.realpath("Temp\\")
directories["Dependencies"]			= os.realpath("Dependencies\\")
directories["Lib"]					= directories.Dependencies .. "Lib\\"
directories[projects.Launcher]				= directories.Source .. "Launcher\\"
directories[projects.External]				= directories.Source .. "External\\"
directories[projects.UnitTest]				= directories.Source .. "UnitTest\\"
directories[projects.Engine]				= directories.Source .. "Engine\\"

for key, path in pairs(directories) do
    if key ~= "Root" then 
        os.mkdir(directories[key])
    end
end

workspace (name)
	startproject (projects.Launcher)
	architecture "x64"
	language "C++"
	cdialect "C17"
	cppdialect "C++20"
	warnings "Extra"
	toolset "msc-v145"
	objdir (directories["Temp"] .. "/%{cfg.buildcfg}/%{prj.name}")
	libdirs (directories["Lib"] .. "/%{cfg.buildcfg}")

	configurations {
		"Debug",
		"Release",
		"Retail"
	}

	filter "configurations:Debug" 
		defines { "_DEBUG" }
		symbols "On"
		optimize "Off"
		runtime "Debug"
		linktimeoptimization "Off"
		editandcontinue "On"		

	filter "configurations:Release"
		defines { "_RELEASE" }
		symbols "On"
		optimize "Speed"
		runtime "Release"
		linktimeoptimization "On"
		editandcontinue "On"

	filter "configurations:Retail"
		defines { "_RETAIL", "NDEBUG" }
		symbols "Off"
		optimize "Full"
		runtime "Release"
		linktimeoptimization "On"
		editandcontinue "Off"

	filter "system:windows"
		systemversion "latest"
		
		flags {
			"MultiProcessorCompile"
		}

	--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	project (projects.External)
		kind "StaticLib"
		location (directories.Local) 
		targetdir (directories.Lib .. "/%{cfg.buildcfg}")
		targetname("%{prj.name}_%{cfg.buildcfg}") 
		fatalwarnings { "All" }

		includedirs {
			directories.Source,
			directories.External, 
		} 

		files {
			directories.External .. "/**.h",
			directories.External .. "/**.hpp",
			directories.External .. "/**.cpp"
		} 

  	--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	project (projects.UnitTest)
		kind "SharedLib"
		location (directories.Local) 
		targetdir (directories.Lib .. "/%{cfg.buildcfg}")
		targetname("%{prj.name}_%{cfg.buildcfg}") 
		fatalwarnings { "All" }
		pchheader ("UnitTest/pch.h")
		pchsource ("Source/UnitTest/pch.cpp") -- I don't know why I can't use directories.UnitTest .. pch.cpp
		--flags { "ExcludeFromBuild"}

		includedirs {
			directories.Source,
			directories.Engine,
			directories.UnitTest, 
		} 

		files {
			directories.UnitTest .. "/**.h",
			directories.UnitTest .. "/**.hpp",
			directories.UnitTest .. "/**.cpp"
		} 

		links {
			projects.Engine
		}

	--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	project (projects.Engine)
		kind "StaticLib"
		location (directories.Local) 
		targetdir (directories.Lib .. "/%{cfg.buildcfg}")
		targetname("%{prj.name}_%{cfg.buildcfg}") 
		fatalwarnings { "All" }

		includedirs {
			directories.Engine, 
		} 

		files {
			directories.Engine .. "/**.h",
			directories.Engine .. "/**.hpp",
			directories.Engine .. "/**.cpp"
		} 


	--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	project (projects.Launcher)
		kind "ConsoleApp"
		location (directories.Local)
		targetdir (directories.Bin .. "/%{cfg.buildcfg}")
		targetname (name.."_%{cfg.buildcfg}")
		fatalwarnings { "All" }
	
		files {
			directories.Launcher .. "/**.h",
			directories.Launcher .. "/**.hpp",
			directories.Launcher .. "/**.cpp",
			directories.Launcher .. "/**.rc"
		}

		includedirs { 
			directories.Source,
			directories.Launcher,
			directories.Engine
		}

		links {
			projects.External,
			projects.Engine
		}

		filter "system:windows"
			systemversion "latest"

	--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
local name = "SimpleProject"

local projects = {
	Launcher = "Launcher",
	External = "External"
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
	toolset "msc-v143"
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
			directories.External, 
		} 

		files {
			directories.External .. "/**.h",
			directories.External .. "/**.hpp",
			directories.External .. "/**.cpp"
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
			directories.Launcher
		}

		links {
			projects.External
		}

		filter "system:windows"
			systemversion "latest"

	--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
local dev_path = "dev"
local build_path = "build"
local dependencies_path = "dependencies"

--Project: dx12-engine
local dx12_project_include_path = dev_path .. "/dx12-engine/include"
local dx12_project_src_path = dev_path .. "/dx12-engine/src"

workspace ( "dx12-engine" )
   configurations ( { "Debug", "Release" } )
   platforms ( { "x64" } )
   location ( build_path )

project ( "dx12-engine" )
   kind ( "ConsoleApp" )
   language ( "C++" )
   cppdialect ( "C++17" )
   location ( build_path .. "/dx12-engine" )

   targetdir ( build_path .. "/bin/%{cfg.platform}-%{cfg.buildcfg}" )
   objdir ( build_path .. "/bin-int/%{cfg.platform}-%{cfg.buildcfg}" )

   includedirs {
      dependencies_path .. "/include",
      dev_path .. "/dx12-engine/include"
   }
   libdirs {
      dependencies_path .. "/lib"
   }

   links {
      "spdlog"
   }

   files {
      dx12_project_include_path .. "/**.h",
      dx12_project_src_path .. "/**.cpp"
   }

   filter ( "configurations:Debug" )
      defines ( { "DEBUG", "_DEBUG" } )
      symbols ( "On" )

   filter ( "configurations:Release" )
      defines ( { "NDEBUG", "_NDEBUG" } )
      optimize ( "On" )

   filter ( "system:windows" )
      defines( { "WINDOW_WIN32" } )

   filter ( { "system:windows", "action:vs*" } )
      buildoptions ( { "/utf-8" } )
      vpaths {
         ["Include/*"] = { dx12_project_include_path .. "/**.h" },
         ["Sources/*"] = { dx12_project_src_path .. "/**.cpp" },
      }

   filter ( "system:linux" )
      defines( { "WINDOW_XCB" } )
outputdir = "%{cfg.buildcfg}-%{cfg.architecture}"

workspace "SmartOCR"
	architecture "x64"
	configurations {"Debug","Release"}

    filter "system:windows"
      buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }
      systemversion "latest"

    filter "configurations:Debug"
      defines { "_DEBUG" }
      runtime "Debug"
      symbols "On"

    filter "configurations:Release"
      defines { "NDEBUG" }
      runtime "Release"
      optimize "On"
      symbols "Off"

project "SmartOCR"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/")
	objdir ("bin-int/" .. outputdir .. "/")

	includedirs {
		"src/",
		"vendor/tesseract/include/"
	}

	files {
		"src/**.cpp",
		"src/**.h",
		"src/**.c"
	}

	filter "configurations:Debug"
			libdirs {
				"vendor/tesseract/libs/Debug"
			}
      links {
      	"bz2d.lib",
				"gif.lib",
				"jpeg.lib",
				"leptonica-1.84.1d.lib",
				"libcurl-d.lib",
				"libpng16d.lib",
				"libssl.lib",
				"libwebp.lib",
				"libxml2.lib",
				"lz4d.lib",
				"lzma.lib",
				"openjp2.lib",
				"tesseract53d.lib",
				"tiffd.lib",
				"turbojpeg.lib",
				"zlibd.lib",
				"zstd.lib",
				"archive.lib",
				"libcrypto.lib",
				"libsharpyuv.lib",
				"Crypt32.lib"
      }

	filter "configurations:Release"
			libdirs {
				"vendor/tesseract/libs/Release"
			}
      links {
      	"bz2.lib",
				"gif.lib",
				"jpeg.lib",
				"leptonica-1.84.1.lib",
				"libcurl.lib",
				"libpng16.lib",
				"libssl.lib",
				"libwebp.lib",
				"libxml2.lib",
				"lz4.lib",
				"lzma.lib",
				"openjp2.lib",
				"tesseract53.lib",
				"tiff.lib",
				"turbojpeg.lib",
				"zlib.lib",
				"zstd.lib",
				"archive.lib",
				"libcrypto.lib",
				"libsharpyuv.lib",
				"Crypt32.lib"
      }
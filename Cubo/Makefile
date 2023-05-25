
MAKEFILE      = Makefile

EQ            = =

####### Compiler, tools and options

CC            = gcc
CXX           = g++
CFLAGS        = -pipe -O2 -Wall -W -fPIC $(DEFINES)
CXXFLAGS      = -pipe -std=c++11 -O2 -O2 -std=gnu++11 -Wall -W -fPIC $(DEFINES)
INCPATH       = -I. -I../../eigen-eigen-323c052e1731 -isystem /usr/include/freetype2
LIBS          = $(SUBLIBS) -L/usr/local/lib -lGL -lglut -lGLU -lGLEW -lproj -lfreetype   
####### Files

SOURCES       = main.cpp \
		ColorBar.cpp \
		ColorMap.cpp \
		ConfigurationFileReader.cpp \
		Data2D.cpp \
		DisplaySettings.cpp \
		DrawableIdentifier.cpp \
		Format.cpp \
		Grid2D.cpp \
		Interpolation.cpp \
		Label.cpp \
		LoadSimulationData.cpp \
		MapPoint.cpp \
		pugixml.cpp \
		RegistryGraph.cpp \
		ShaderHandling.cpp \
		Shapes.cpp \
		Squencer.cpp \
		Stroke2D.cpp \
		TextureManager.cpp \
		Triangulate.cpp \
		ViewingPlane.cpp \
		VolumeRendering.cpp \
		Compute.cpp \
		Data3D.cpp \
		Projection.cpp \
		Text3D.cpp \
		LoadShapeFileCSV.cpp \
		LoadArcASCII.cpp \
		TinyPngOut.cpp \
		C3_GL_Text.cpp \
		C3_Texture_Text.cpp \
		Utils.cpp \
		ColorChart.cpp \
		BarScale.cpp \
		Distance.cpp \
		Mesh.cpp \
		Global.cpp 
OBJECTS       = main.o \
		ColorBar.o \
		ColorMap.o \
		ConfigurationFileReader.o \
		Data2D.o \
		DisplaySettings.o \
		DrawableIdentifier.o \
		Format.o \
		Grid2D.o \
		Interpolation.o \
		Label.o \
		LoadSimulationData.o \
		MapPoint.o \
		pugixml.o \
		RegistryGraph.o \
		ShaderHandling.o \
		Shapes.o \
		Squencer.o \
		Stroke2D.o \
		TextureManager.o \
		Triangulate.o \
		ViewingPlane.o \
		VolumeRendering.o \
		Compute.o \
		Data3D.o \
		Projection.o \
		Text3D.o \
		LoadShapeFileCSV.o \
		LoadArcASCII.o \
		TinyPngOut.o \
		C3_GL_Text.o \
		C3_Texture_Text.o \
		Utils.o \
		ColorChart.o \
		BarScale.o \
		Distance.o \
		Mesh.o \
		Global.o
DIST          = ../Shaders/Texture2D.frag \
		../Shaders/Texture2D.vert \
		../Shaders/TrazaRayos.frag \
		../Shaders/PosicionesRayos.vert \
		../Shaders/Grid.frag \
		../Shaders/Grid.vert \
		../Shaders/Text.frag \
		../Shaders/Text.vert \
		Cubo.pro ColorBar.hpp \
		ColorMap.hpp \
		ConfigurationFileReader.hpp \
		Data2D.hpp \
		DisplaySettings.hpp \
		DrawableIdentifier.hpp \
		Format.hpp \
		Grid2D.hpp \
		Interpolation.hpp \
		Label.hpp \
		LoadSimulationData.hpp \
		MapPoint.hpp \
		pugiconfig.hpp \
		pugixml.hpp \
		RegistryGraph.hpp \
		ShaderHandling.hpp \
		Shapes.hpp \
		Squencer.hpp \
		Stroke2D.hpp \
		TextureManager.hpp \
		Triangulate.hpp \
		Utils.hpp \
		ViewingPlane.hpp \
		VolumeRendering.hpp \
		Data3D.hpp \
		vertex3d.hpp \
		Compute.hpp \
		Projection.hpp \
		Text3D.hpp \
		LoadShapeFileCSV.hpp \
		LoadArcASCII.hpp \
		TinyPngOut.hpp \
		C3_GL_Text.hpp \
		C3_Texture_Text.hpp \
		ColorChart.hpp \
		BarScale.hpp \
		Distance.hpp \
		Mesh.hpp \
		Global.hpp main.cpp \
		ColorBar.cpp \
		ColorMap.cpp \
		ConfigurationFileReader.cpp \
		Data2D.cpp \
		DisplaySettings.cpp \
		DrawableIdentifier.cpp \
		Format.cpp \
		Grid2D.cpp \
		Interpolation.cpp \
		Label.cpp \
		LoadSimulationData.cpp \
		MapPoint.cpp \
		pugixml.cpp \
		RegistryGraph.cpp \
		ShaderHandling.cpp \
		Shapes.cpp \
		Squencer.cpp \
		Stroke2D.cpp \
		TextureManager.cpp \
		Triangulate.cpp \
		ViewingPlane.cpp \
		VolumeRendering.cpp \
		Compute.cpp \
		Data3D.cpp \
		Projection.cpp \
		Text3D.cpp \
		LoadShapeFileCSV.cpp \
		LoadArcASCII.cpp \
		TinyPngOut.cpp \
		C3_GL_Text.cpp \
		C3_Texture_Text.cpp \
		Utils.cpp \
		ColorChart.cpp \
		BarScale.cpp \
		Distance.cpp \
		Mesh.cpp \
		Global.cpp
QMAKE_TARGET  = Cubo
DESTDIR       = 
TARGET        = Cubo

Cubo:
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES) $(INCPATH) $(LIBS)






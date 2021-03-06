HEADERS		= 	callbacks.hpp init.hpp models/graphicmodel.hpp models/models.hpp globals.hpp \
				utils/mathUtils.hpp shaders/shaders.hpp effects/Effects.hpp effects/Effect.hpp \
				effects/types/Sepia.hpp effects/types/CannyE.hpp effects/types/Cartoon.hpp \
				effects/types/Illumination.hpp effects/types/ChannelSwitch.hpp effects/types/Lomo.hpp \
				effects/types/Grayscale.hpp effects/types/FloodFilling.hpp effects/types/Vignette.hpp \
				effects/types/Pencil.hpp effects/types/Saturation.hpp visualization/theme.hpp \
				visualization/themeController.hpp visualization/personalize/coverflow.hpp \
				visualization/personalize/slideflow.hpp
				
SOURCES		=	project60337.cpp globals.cpp callbacks.cpp models/graphicmodel.cpp init.cpp models/models.cpp \
				shaders/shaders.cpp utils/mathUtils.cpp effects/Effects.cpp effects/Effect.cpp \
				visualization/theme.cpp visualization/themeController.cpp visualization/personalize/coverflow.cpp \
				visualization/personalize/slideflow.cpp

INCLUDEPATH     = 
LIBS            += -lGLEW -lglut -lGL -lX11  -lm -lGLU -lboost_filesystem -lboost_system `pkg-config opencv --libs`
TARGET			= slideshow3d
CONFIG 			-= qt warn_on

QMAKE_CXXFLAGS_WARN_ON -= -Wall -W -W1, -01 -m64
QMAKE_CXXFLAGS_RELEASE =
QMAKE_CXXFLAGS_PRECOMPILE  =
QMAKE_CXXFLAGS_USE_PRECOMPILE =
QMAKE_CXXFLAGS =
QMAKE_CXXFLAGS_DEPS =

QMAKE_CFLAGS_WARN_ON -= -Wall -W -W1, -01 -m64
QMAKE_CFLAGS_RELEASE = 
QMAKE_CFLAGS_PRECOMPILE  =
QMAKE_CFLAGS_USE_PRECOMPILE =
QMAKE_CFLAGS =
QMAKE_CFLAGS_DEPS =

DESTDIR = ../bin
OBJECTS_DIR = ../bin/genfiles
MOC_DIR = ../bin/genfiles
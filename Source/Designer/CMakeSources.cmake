# Preparation
set (DESIGNER_HEADERS "")
set (DESIGNER_SOURCES "")

# Default
set(DESIGNER_DEFAULT_HEADERS )
set(DESIGNER_DEFAULT_MODULES
	"Module.ixx"
)
set(DESIGNER_DEFAULT_SOURCES
	"Spectra.cpp"
)
list(APPEND DESIGNER_HEADERS ${DESIGNER_DEFAULT_HEADERS})
list(APPEND DESIGNER_SOURCES ${DESIGNER_DEFAULT_SOURCES})

# Menues
set(DESIGNER_MENU_HEADERS
	"Spectra/Menu/Menues.h"
	"Spectra/Menu/File.h"
	"Spectra/Menu/Edit.h"
	"Spectra/Menu/View.h"
	"Spectra/Menu/Tools.h"
	"Spectra/Menu/Help.h"
)
set(DESIGNER_MENU_SOURCES
)
list(APPEND DESIGNER_HEADERS ${DESIGNER_MENU_HEADERS})
list(APPEND DESIGNER_SOURCES ${DESIGNER_MENU_SOURCES})

# Panels
set(DESIGNER_VIEW_HEADERS
	"Spectra/Panel/Panels.h"

    "Spectra/Panel/Browser.h"
    "Spectra/Panel/Properties.h"
    "Spectra/Panel/Viewport.h"

	"Spectra/Panel/Console.h"
	"Spectra/Panel/StyleEditor.h"

	"Spectra/Panel/TextEditor.h"
)
set(DESIGNER_VIEW_SOURCES
    "Spectra/Panel/Browser.cpp"
    "Spectra/Panel/Properties.cpp"
    "Spectra/Panel/Viewport.cpp"

	"Spectra/Panel/TextEditor.cpp"
)
list(APPEND DESIGNER_HEADERS ${DESIGNER_VIEW_HEADERS})
list(APPEND DESIGNER_SOURCES ${DESIGNER_VIEW_SOURCES})

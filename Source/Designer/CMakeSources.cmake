# Preparation
set (DESIGNER_HEADERS "")
set (DESIGNER_SOURCES "")

# Default
set(DESIGNER_DEFAULT_HEADERS )
set(DESIGNER_DEFAULT_MODULES
    "resource.h"
	"Module.ixx"
	"Spectra/Designer.h"
)
set(DESIGNER_DEFAULT_SOURCES
	"Spectra.cpp"
    "Spectra.rc"
	"Spectra/Designer.cpp"
)
list(APPEND DESIGNER_HEADERS ${DESIGNER_DEFAULT_HEADERS})
list(APPEND DESIGNER_SOURCES ${DESIGNER_DEFAULT_SOURCES})

# Menues
set(DESIGNER_COMPONENT_HEADERS
	"Spectra/Component/Menu.h"
)
set(DESIGNER_COMPONENT_SOURCES
)
list(APPEND DESIGNER_HEADERS ${DESIGNER_COMPONENT_HEADERS})
list(APPEND DESIGNER_SOURCES ${DESIGNER_COMPONENT_SOURCES})

# Panels
set(DESIGNER_PANEL_HEADERS
	"Spectra/Panel/Panels.h"

    "Spectra/Panel/Browser.h"
    "Spectra/Panel/Properties.h"
    "Spectra/Panel/Viewport.h"

	"Spectra/Panel/Console.h"
	"Spectra/Panel/StyleEditor.h"

	"Spectra/Panel/TextEditor.h"
)
set(DESIGNER_PANEL_SOURCES
    "Spectra/Panel/Browser.cpp"
    "Spectra/Panel/Properties.cpp"
    "Spectra/Panel/Viewport.cpp"

	"Spectra/Panel/TextEditor.cpp"
)
list(APPEND DESIGNER_HEADERS ${DESIGNER_PANEL_HEADERS})
list(APPEND DESIGNER_SOURCES ${DESIGNER_PANEL_SOURCES})

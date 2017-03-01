TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.c \
	scv.c \
	dh.c

HEADERS +=

# CXXFLAGS ----------------------------------------------------------
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O5 -v
QMAKE_CXXFLAGS_DEBUG += -v

# CFLAGS ------------------------------------------------------------
QMAKE_CFLAGS_RELEASE -= -O2
QMAKE_CFLAGS_RELEASE += -O5 -v
QMAKE_CFLAGS_DEBUG += -v

HEAP_SIZE      = 8388208
STACK_SIZE     = 61800

PRODUCT = TidyTest.pdx

# Locate the SDK
SDK = ${PLAYDATE_SDK_PATH}
ifeq ($(SDK),)
	SDK = $(shell egrep '^\s*SDKRoot' ~/.Playdate/config | head -n 1 | cut -c9-)
endif

ifeq ($(SDK),)
$(error SDK path not found; set ENV value PLAYDATE_SDK_PATH)
endif

######
# IMPORTANT: You must add your source folders to VPATH for make to find them
######

VPATH += src
VPATH += ../tidy-html5/src

# List C source files here - including minimal set of tidy sources
SRC = src/main.c \
      ../tidy-html5/src/access.c \
      ../tidy-html5/src/alloc.c \
      ../tidy-html5/src/attrdict.c \
      ../tidy-html5/src/attrs.c \
      ../tidy-html5/src/buffio.c \
      ../tidy-html5/src/charsets.c \
      ../tidy-html5/src/clean.c \
      ../tidy-html5/src/config.c \
      ../tidy-html5/src/entities.c \
      ../tidy-html5/src/fileio.c \
      ../tidy-html5/src/gdoc.c \
      ../tidy-html5/src/istack.c \
      ../tidy-html5/src/language.c \
      ../tidy-html5/src/lexer.c \
      ../tidy-html5/src/mappedio.c \
      ../tidy-html5/src/message.c \
      ../tidy-html5/src/messageobj.c \
      ../tidy-html5/src/parser.c \
      ../tidy-html5/src/pprint.c \
      ../tidy-html5/src/sprtf.c \
      ../tidy-html5/src/streamio.c \
      ../tidy-html5/src/tagask.c \
      ../tidy-html5/src/tags.c \
      ../tidy-html5/src/tidylib.c \
      ../tidy-html5/src/tmbstr.c \
      ../tidy-html5/src/utf8.c

# List all user directories here
UINCDIR = ../tidy-html5/include ../tidy-html5/src

# List user asm files
UASRC =

# List all user C define here, like -D_DEBUG=1
UDEFS = -DSUPPORT_UTF16_ENCODINGS=1 -DSUPPORT_ASIAN_ENCODINGS=1 -DSUPPORT_ACCESSIBILITY_CHECKS=1

# Define ASM defines here
UADEFS =

# List the user directory to look for the libraries here
ULIBDIR =

# List all user libraries here
ULIBS =

include $(SDK)/C_API/buildsupport/common.mk

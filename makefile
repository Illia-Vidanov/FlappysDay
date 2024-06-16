# makefile to compile project

# Directories
SOURCEDIR := src
LIBDIR := libs
BUILDDIR := build
OBJSDIR := build/objs

# Variables
APP_NAME := "Flappy's Day.exe"
DEBUG := -g
COMPILER := g++
FLAGS := -Wall -Wextra -Wdeprecated -pedantic-errors -fmax-errors=10 -Wshadow -std=c++17
LIBS := -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
INCLUDES := -I$(SOURCEDIR) -I$(LIBDIR)/Eigen -I$(LIBDIR)/random
ARGS :=
ECHO := @
PROGRESS := 1
DEFINES :=
INCLUDE_FILES_EXTENTIONS := hpp tpp inl
ADDITIONAL_DEPENDENCIES :=
# Only one type is allowed 
SOURCE_FILES_EXTENTION := cpp
# Needed for checking if makefile has changed
MAKEFILE_NAME := makefile
.DEFAULT_GOAL := compile


# Note: I use findutils to locate files

# Find all folders in $(SOURCEDIR)
SOURCE_FOLDERS := $(shell find $(SOURCEDIR) -type d)

# Creates folders for object files and by replacing $(SOURCEDIR) to $(OBJSDIR) (using folder structure in order to prevent collisions between files with same name)
$(foreach folder,$(SOURCE_FOLDERS),$(shell mkdir -p $(subst $(SOURCEDIR),$(OBJSDIR),$(folder))))
$(shell mkdir -p $(BUILDDIR))

# Get all files we need to compile/link/include
SOURCES := $(foreach folder,$(SOURCE_FOLDERS),$(wildcard $(folder)/*.$(SOURCE_FILES_EXTENTION)))
# First find all include files from source folders
INCLUDE_FILES := $(foreach ext,$(INCLUDE_FILES_EXTENTIONS),$(foreach folder,$(SOURCE_FOLDERS),$(wildcard $(folder)/*.$(ext))))
OBJS := $(foreach file,$(SOURCES),$(file:$(SOURCEDIR)/%.$(SOURCE_FILES_EXTENTION)=$(OBJSDIR)/%.o))


# Execute before/after compile
EXECUTE_BEFORE_COMPILE :=
EXECUTE_AFTER_COMPILE := $(BUILDDIR)/$(APP_NAME);
$(shell $(EXECUTE_BEFORE_COMPILE))


# All
all: run

# Run
run: compile
	$(ECHO)if [ $(PROGRESS) ]; then \
		echo Run!; \
	fi; \
	$(BUILDDIR)/$(APP_NAME) $(ARGS)

precompile_header: $(ARGS)
	$(ECHO)if [ $(PROGRESS) ]; then \
		echo Compiling $(notdir $(ARGS))...; \
	fi; \
	$(COMPILER) $(DEBUG) $(ARGS) -c $(addsufix .gch,$(ARGS)) $(DEFIENS) $(FLAGS); \
	if [ $(PROGRESS) ]; then \
		echo Compiled!; \
	fi;

# Compile
ifneq ($(APP_NAME),)
compile: $(OBJS) $(MAKEFILE_NAME)
	$(ECHO)if [ $(PROGRESS) ]; then \
		echo Compiling $(APP_NAME); \
	fi; \
	$(COMPILER) $(DEBUG) $(OBJS) -o $(BUILDDIR)/$(APP_NAME) $(DEFIENS) $(FLAGS) $(INCLUDES) $(LIBS); \
	$(EXECUTE_AFTER_COMPILE) \
	if [ $(PROGRESS) ]; then \
		echo Compiled!; \
	fi;
else
compile: $(OBJS) $(MAKEFILE_NAME)
	$(ECHO)$(EXECUTE_AFTER_COMPILE) \
	if [ $(PROGRESS) ]; then \
		echo Compiled!; \
	fi;
endif

# Object files
define generate_rules
$(1:$(SOURCEDIR)/%.$(SOURCE_FILES_EXTENTION)=$(OBJSDIR)/%.o): $1 $(INCLUDE_FILES) $(MAKEFILE_NAME) $(ADDITIONAL_DEPENDENCIES)
	$(ECHO)if [ $(PROGRESS) ]; then \
		echo Compiling $(notdir $(1))...; \
	fi; \
	$(COMPILER) $(DEBUG) -c $$< -o $$@ $(DEFIENS) $(FLAGS) $(INCLUDES) $(LIBS) 

endef

$(foreach src,$(SOURCES),$(eval $(call generate_rules,$(src))))

# Cleaning
clean_obj_files:
	$(ECHO)rm -r $(OBJSDIR)

clean: clean_obj_files
	$(ECHO)rm -r $(BUILDDIR)
PATH_SRC   := src
PATH_LIB   := lib
PATH_BUILD := build
PATH_BIN   := bin
PATH_OBJ   := $(PATH_BUILD)/obj
PATH_DEP   := $(PATH_BUILD)/dep

include common_vars.mk

LDLIBS   += -lcryptopp -pthread
CPPFLAGS += -I $(PATH_LIB)

EXE_SERVER := server
EXE_CLIENT := client

#----------------------------------------

VPATH = $(PATH_SRC)

FILES   = $(patsubst src/%, %, $(shell find $(PATH_SRC) -name "*.cpp" -type f))
FOLDERS = $(patsubst src/%, %, $(shell find $(PATH_SRC) -mindepth 1 -type d))

FILES_NOMAIN = $(filter-out server.cpp client.cpp, $(FILES))

FILES_DEP = $(patsubst %, $(PATH_DEP)/%.d, $(basename $(FILES)))
FILES_OBJ = $(patsubst %, $(PATH_OBJ)/%.o, $(basename $(FILES_NOMAIN)))

#----------------------------------------

all: $(PATH_BIN)/$(EXE_SERVER) $(PATH_BIN)/$(EXE_CLIENT)

$(PATH_BIN)/$(EXE_SERVER): $(PATH_OBJ)/server.o $(FILES_OBJ)
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(PATH_BIN)/$(EXE_CLIENT): $(PATH_OBJ)/client.o $(FILES_OBJ)
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@


DEPFLAGS    = -MT $@ -MMD -MP -MF $(PATH_DEP)/$*.dTMP
POSTCOMPILE = @$(MOVE) $(PATH_DEP)/$*.dTMP $(PATH_DEP)/$*.d > $(NULL_DEVICE) && touch $@

$(PATH_OBJ)/%.o: %.cpp
$(PATH_OBJ)/%.o: %.cpp $(PATH_DEP)/%.d | directories
	$(CXX) $(CPPFLAGS) -c $(DEPFLAGS) $< -o $@
	$(POSTCOMPILE)

.PRECIOUS: $(FILES_DEP)
$(FILES_DEP): ;
-include $(FILES_DEP)

#----------------------------------------

directories: $(PATH_BUILD) $(PATH_BIN) $(PATH_OBJ) $(PATH_DEP) nested-folders
nested-folders: $(addprefix $(PATH_OBJ)/, $(FOLDERS)) $(addprefix $(PATH_DEP)/, $(FOLDERS))

$(PATH_BUILD): ; $(MKDIR) $@
$(PATH_BIN): ; $(MKDIR) $@
$(PATH_OBJ): ; $(MKDIR) $@
$(PATH_DEP): ; $(MKDIR) $@

$(addprefix $(PATH_OBJ)/, $(FOLDERS)): ; @$(MKDIR) $@
$(addprefix $(PATH_DEP)/, $(FOLDERS)): ; @$(MKDIR) $@

#----------------------------------------

.PHONY: all directories nested-folders \
		clean clean-obj clean-dep clean-exe delete-build \
		run-server run-client help

clean: clean-obj clean-dep clean-exe
clean-obj: ; $(RMDIR) $(PATH_OBJ)/*
clean-dep: ; $(RMDIR) $(PATH_DEP)/*
clean-exe: ; $(RM) $(PATH_BIN)/$(EXE_SERVER) $(PATH_BIN)/$(EXE_CLIENT)
delete-build: clean-exe ; $(RMDIR) $(PATH_BUILD)

ARGS ?=
run-server: ; @cd $(PATH_BIN) && ./$(EXE_SERVER) $(ARGS)
run-client: ; @cd $(PATH_BIN) && ./$(EXE_CLIENT) $(ARGS)

help:
	@echo Targets: all clean clean-obj clean-dep clean-exe delete-build run-server run-client
	@echo '(make run-x ARGS="arg1 arg2...")'

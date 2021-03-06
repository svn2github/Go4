include $(GO4SYS)/Makefile.config

TASKHANDEXAMP_NAME        = Go4TaskHandlerExample

ifdef GO4PACKAGE
TASKHANDEXAMP_DIR         = $(TASKHANDEXAMP_NAME)
else
TASKHANDEXAMP_DIR         = .
endif

TASKHANDEXAMP_LINKDEF     = $(TASKHANDEXAMP_DIR)/$(TASKHANDEXAMP_NAME)LinkDef.$(HedSuf)
TASKHANDEXAMP_LIBNAME     = lib$(TASKHANDEXAMP_NAME)

TASKHANDEXAMP_EXE1NAME    = MainGo4TaskHandlerClient
TASKHANDEXAMP_EXE2NAME    = MainGo4TaskHandlerServer

TASKHANDEXAMP_NOTLIBF     = 

## must be similar for every module

TASKHANDEXAMP_EXE1O       = $(TASKHANDEXAMP_DIR)/$(TASKHANDEXAMP_EXE1NAME).$(ObjSuf)
TASKHANDEXAMP_EXE1S       = $(TASKHANDEXAMP_DIR)/$(TASKHANDEXAMP_EXE1NAME).$(SrcSuf)
TASKHANDEXAMP_EXE1        = $(TASKHANDEXAMP_DIR)/$(TASKHANDEXAMP_EXE1NAME)$(ExeSuf)   

TASKHANDEXAMP_EXE2O       = $(TASKHANDEXAMP_DIR)/$(TASKHANDEXAMP_EXE2NAME).$(ObjSuf)
TASKHANDEXAMP_EXE2S       = $(TASKHANDEXAMP_DIR)/$(TASKHANDEXAMP_EXE2NAME).$(SrcSuf)
TASKHANDEXAMP_EXE2        = $(TASKHANDEXAMP_DIR)/$(TASKHANDEXAMP_EXE2NAME)$(ExeSuf)   

TASKHANDEXAMP_DICT        = $(TASKHANDEXAMP_DIR)/$(DICT_PREFIX)$(TASKHANDEXAMP_NAME)
TASKHANDEXAMP_DH          = $(TASKHANDEXAMP_DICT).$(HedSuf)
TASKHANDEXAMP_DS          = $(TASKHANDEXAMP_DICT).$(SrcSuf)
TASKHANDEXAMP_DO          = $(TASKHANDEXAMP_DICT).$(ObjSuf)

TASKHANDEXAMP_H           = $(filter-out $(TASKHANDEXAMP_NOTLIBF) $(TASKHANDEXAMP_DH) $(TASKHANDEXAMP_LINKDEF), $(wildcard $(TASKHANDEXAMP_DIR)/*.$(HedSuf)))
TASKHANDEXAMP_S           = $(filter-out $(TASKHANDEXAMP_EXE1S) $(TASKHANDEXAMP_EXE2S) $(TASKHANDEXAMP_NOTLIBF) $(TASKHANDEXAMP_DS), $(wildcard $(TASKHANDEXAMP_DIR)/*.$(SrcSuf)))
TASKHANDEXAMP_O           = $(TASKHANDEXAMP_S:.$(SrcSuf)=.$(ObjSuf))

TASKHANDEXAMP_DEP         =  $(TASKHANDEXAMP_O:.$(ObjSuf)=.$(DepSuf))
TASKHANDEXAMP_DDEP        =  $(TASKHANDEXAMP_DO:.$(ObjSuf)=.$(DepSuf))
TASKHANDEXAMP_EDEP        =  $(TASKHANDEXAMP_EXE1O:.$(ObjSuf)=.$(DepSuf)) $(TASKHANDEXAMP_EXE2O:.$(ObjSuf)=.$(DepSuf))

TASKHANDEXAMP_LIB         =  $(TASKHANDEXAMP_DIR)/$(TASKHANDEXAMP_LIBNAME).$(DllSuf)

# used in the main Makefile

EXAMPDEPENDENCS    += $(TASKHANDEXAMP_DEP) $(TASKHANDEXAMP_DDEP) $(TASKHANDEXAMP_EDEP)

ifdef DOPACKAGE
TASKHANDEXAMP_DISTRFILES = $(TASKHANDEXAMP_S) $(TASKHANDEXAMP_H) $(TASKHANDEXAMP_LINKDEF) $(TASKHANDEXAMP_EXE1S) $(TASKHANDEXAMP_EXE2S)
DISTRFILES         += $(TASKHANDEXAMP_DISTRFILES)
endif

##### local rules #####

all::   $(TASKHANDEXAMP_LIB) $(TASKHANDEXAMP_EXE1) $(TASKHANDEXAMP_EXE2)

$(TASKHANDEXAMP_EXE1):  $(TASKHANDEXAMP_EXE1O) $(TASKHANDEXAMP_LIB)
	$(LD) $(LDFLAGS) $(TASKHANDEXAMP_EXE1O) $(LIBS_TASKHANDSET) $(TASKHANDEXAMP_LIB) $(OutPutOpt) $(TASKHANDEXAMP_EXE1)
	@echo "$@  done"      

$(TASKHANDEXAMP_EXE2):  $(TASKHANDEXAMP_EXE2O) $(TASKHANDEXAMP_LIB)
	$(LD) $(LDFLAGS) $(TASKHANDEXAMP_EXE2O) $(LIBS_TASKHANDSET) $(TASKHANDEXAMP_LIB) $(OutPutOpt) $(TASKHANDEXAMP_EXE2)
	@echo "$@  done"      

TASKHANDEXAMP_DEPLIB = $(GO4TSKH_LIB) $(THRDMNGR_LIB) $(GO4BASE_LIB) $(BASIC_LIB_DEP)

$(TASKHANDEXAMP_LIB):   MAKELIB_SET = $(LIBS_TASKHANDSET)

$(TASKHANDEXAMP_LIB):   $(TASKHANDEXAMP_O) $(TASKHANDEXAMP_DO) $(TASKHANDEXAMP_DEPLIB)
	@$(MakeLibrary) $(TASKHANDEXAMP_LIBNAME) "$(TASKHANDEXAMP_O) $(TASKHANDEXAMP_DO)" $(TASKHANDEXAMP_DIR) $(TASKHANDEXAMP_LINKDEF) "$(TASKHANDEXAMP_DEPLIB)" $(TASKHANDEXAMP_DS) "$(TASKHANDEXAMP_H)"

$(TASKHANDEXAMP_DS): $(TASKHANDEXAMP_H)  $(TASKHANDEXAMP_LINKDEF)
	@$(ROOTCINTGO4) $(TASKHANDEXAMP_LIB) $(TASKHANDEXAMP_H) $(TASKHANDEXAMP_LINKDEF)

clean-bin::
	@rm -f $(TASKHANDEXAMP_O) $(TASKHANDEXAMP_DO)
	@rm -f $(TASKHANDEXAMP_EXE1O) $(TASKHANDEXAMP_EXE2O) $(TASKHANDEXAMP_EDEP)
	@rm -f $(TASKHANDEXAMP_DEP) $(TASKHANDEXAMP_DDEP) $(TASKHANDEXAMP_DS) $(TASKHANDEXAMP_DH)

clean:: clean-bin
	@$(CleanLib) $(TASKHANDEXAMP_LIBNAME) $(TASKHANDEXAMP_DIR)
	@rm -f $(TASKHANDEXAMP_EXE1) $(TASKHANDEXAMP_EXE2)

include $(GO4SYS)/Makefile.rules

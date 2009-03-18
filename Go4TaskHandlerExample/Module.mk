TASKHANDEXAMP_NAME        = Go4TaskHandlerExample
MODULE_NAME         = $(TASKHANDEXAMP_NAME)

ifdef GO4PACKAGE
TASKHANDEXAMP_DIR         = $(GO4SYS)/$(TASKHANDEXAMP_NAME)
else
TASKHANDEXAMP_DIR         = .
endif

TASKHANDEXAMP_LINKDEF     = $(TASKHANDEXAMP_DIR)/$(TASKHANDEXAMP_NAME)LinkDef.$(HedSuf)
TASKHANDEXAMP_LIBNAME     = lib$(TASKHANDEXAMP_NAME)

TASKHANDEXAMP_EXE1NAME    = MainGo4TaskHandlerClient
TASKHANDEXAMP_EXE2NAME    = MainGo4TaskHandlerServer
TASKHANDEXAMP_ONLYMAP     = $(TASKHANDEXAMP_DIR)/.localmap
TASKHANDEXAMP_MAP         = $(TASKHANDEXAMP_DIR)/$(ROOTMAPNAME)

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

TASKHANDEXAMP_SLIB        =  $(TASKHANDEXAMP_DIR)/$(TASKHANDEXAMP_LIBNAME).$(DllSuf)
TASKHANDEXAMP_LIB         =  $(TASKHANDEXAMP_DIR)/$(TASKHANDEXAMP_LIBNAME).$(DllSuf).$(VERSSUF)

# used in the main Makefile

EXAMPDEPENDENCS    += $(TASKHANDEXAMP_DEP) $(TASKHANDEXAMP_DDEP) $(TASKHANDEXAMP_EDEP)

ifdef DOPACKAGE
TASKHANDEXAMP_DISTRFILES = $(TASKHANDEXAMP_S) $(TASKHANDEXAMP_H) $(TASKHANDEXAMP_LINKDEF) $(TASKHANDEXAMP_EXE1S) $(TASKHANDEXAMP_EXE2S)
DISTRFILES         += $(TASKHANDEXAMP_DISTRFILES)
endif

##### local rules #####

$(TASKHANDEXAMP_EXE1):      $(BUILDGO4LIBS) $(TASKHANDEXAMP_EXE1O) $(TASKHANDEXAMP_LIB)
	$(LD) $(LDFLAGS) $(TASKHANDEXAMP_EXE1O) $(LIBS_TASKHANDSET) $(TASKHANDEXAMP_LIB) $(OutPutOpt) $(TASKHANDEXAMP_EXE1)
	@echo "$@  done"      

$(TASKHANDEXAMP_EXE2):      $(BUILDGO4LIBS) $(TASKHANDEXAMP_EXE2O) $(TASKHANDEXAMP_LIB)
	$(LD) $(LDFLAGS) $(TASKHANDEXAMP_EXE2O) $(LIBS_TASKHANDSET) $(TASKHANDEXAMP_LIB) $(OutPutOpt) $(TASKHANDEXAMP_EXE2)
	@echo "$@  done"      

$(TASKHANDEXAMP_LIB):   $(TASKHANDEXAMP_O) $(TASKHANDEXAMP_DO)
		@$(MakeLib) $(TASKHANDEXAMP_LIBNAME) "$(TASKHANDEXAMP_O) $(TASKHANDEXAMP_DO)" $(TASKHANDEXAMP_DIR)

$(TASKHANDEXAMP_DS): $(TASKHANDEXAMP_H)  $(TASKHANDEXAMP_LINKDEF)
		@$(ROOTCINTGO4) $(TASKHANDEXAMP_H) $(TASKHANDEXAMP_LINKDEF)

$(TASKHANDEXAMP_ONLYMAP): $(TASKHANDEXAMP_LINKDEF) $(TASKHANDEXAMP_LIB)
	@rm -f $(TASKHANDEXAMP_ONLYMAP)
	@$(MakeMap) $(TASKHANDEXAMP_ONLYMAP) $(TASKHANDEXAMP_SLIB) $(TASKHANDEXAMP_LINKDEF) "$(GO4TSKH_SLIB) $(THRDMNGR_SLIB) $(GO4BASE_SLIB) $(ROOTSYS)/lib/libThread.so"

all-$(TASKHANDEXAMP_NAME):     $(TASKHANDEXAMP_LIB) $(TASKHANDEXAMP_EXE1) $(TASKHANDEXAMP_EXE2) map-$(TASKHANDEXAMP_NAME)

clean-obj-$(TASKHANDEXAMP_NAME):
	@rm -f $(TASKHANDEXAMP_O) $(TASKHANDEXAMP_DO)
	@$(CleanLib) $(TASKHANDEXAMP_LIBNAME) $(TASKHANDEXAMP_DIR)
	@rm -f $(TASKHANDEXAMP_EXE1O) $(TASKHANDEXAMP_EXE1)
	@rm -f $(TASKHANDEXAMP_EXE2O) $(TASKHANDEXAMP_EXE2)

clean-$(TASKHANDEXAMP_NAME): clean-obj-$(TASKHANDEXAMP_NAME)
	@rm -f $(TASKHANDEXAMP_DEP) $(TASKHANDEXAMP_DDEP) $(TASKHANDEXAMP_DS) $(TASKHANDEXAMP_DH)
	@rm -f $(TASKHANDEXAMP_EDEP)
	@rm -f $(TASKHANDEXAMP_ONLYMAP) $(TASKHANDEXAMP_MAP)

ifdef DOMAP
map-$(TASKHANDEXAMP_NAME): $(GO4MAP) $(TASKHANDEXAMP_ONLYMAP)
	@rm -f $(TASKHANDEXAMP_MAP)
	@cat $(GO4MAP) $(TASKHANDEXAMP_ONLYMAP) > $(TASKHANDEXAMP_MAP)
else
map-$(TASKHANDEXAMP_NAME):

endif

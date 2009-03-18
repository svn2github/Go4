#include "TGo4AnalysisCommand.h"

#include "TGo4Log.h"
#include "TGo4AnalysisClientImp.h"

TGo4AnalysisCommand::TGo4AnalysisCommand(const char* name, const char* desc) :
   TGo4Command(name, desc, 7)
{
   TRACE((12,"TGo4AnalysisCommand::TGo4AnalysisCommand(const char*, const char*) ctor",__LINE__, __FILE__));

   fbIsEnabled=kTRUE;
   fbIsSynchron=kTRUE;
   SetReceiverName("AnalysisClient");  // sets the receiver name checked by command invoker
                                 // receiver name neednt be class name!
}

TGo4AnalysisCommand::TGo4AnalysisCommand() :
   TGo4Command()

{
   TRACE((12,"TGo4AnalysisCommand::TGo4AnalysisCommand() ctor",__LINE__, __FILE__));

   fbIsEnabled=kTRUE;
   fbIsSynchron=kTRUE;
   SetReceiverName("AnalysisClient");  // sets the receiver name checked by command invoker
                                 // receiver name neednt be class name!
}


TGo4AnalysisCommand::~TGo4AnalysisCommand()
{
  TRACE((12,"TGo4AnalysisCommand::~TGo4AnalysisCommand() dtor",__LINE__, __FILE__));
}


TGo4AnalysisCommand::TGo4AnalysisCommand(const TGo4AnalysisCommand &right)
  :TGo4Command(right)
{
   TRACE((12,"TGo4AnalysisCommand::TGo4AnalysisCommand() copy constructor",__LINE__, __FILE__));
   //fxReceiver=right.fxReceiver;
}

Int_t TGo4AnalysisCommand::RefuseCom()
{
   TGo4AnalysisClient* cli=dynamic_cast<TGo4AnalysisClient*> (fxReceiverBase);
   if(cli!=0)
      cli->SendStatusMessage(2,kTRUE, "%s::Command %s from GUI %s not allowed for %s (mode %d)",
                     GetTaskName(), GetName(), GetTaskName(),GetModeDescription(GetMode()), GetMode());
   else
      TGo4Command::RefuseCom();
   return 0;
}

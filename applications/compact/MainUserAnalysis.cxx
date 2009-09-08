//---------------------------------------------
// Go4 Comp analysis
// Author: Hans G. Essel
//         H.Essel@gsi.de
// GSI, Experiment Electronics, Data Processing
//---------------------------------------------

#include "Riostream.h"
#include <string.h>
#include <stdlib.h>

#include "TROOT.h"
#include "TRint.h"
#include "TApplication.h"
#include "snprintf.h"
#include "TBenchmark.h"

#include "TGo4Log.h"
#include "TGo4StepFactory.h"
#include "TGo4AnalysisStep.h"
#include "CompAnalysis.h"
#include "TGo4AnalysisClient.h"
#include "Go4EventServerTypes.h"

void usage(); // print announcement

TROOT go4application("Go4Comp","Go4 Comp analysis");

#define kGUI   2
#define kBatch 1

//==================  analysis main program ============================
int main(int argc, char **argv)
{
if(argc < 2) {
  usage(); // no argument: print usage
  exit(0);
}

//=================  init root application loop ==========================
TApplication theApp("Go4App", 0, 0);

//================= Begin analysis setup ==================================
// argv[0] program
// argv[1] "-gui" when started by GUI.
//         In this case the following args are (delivered by GUI):
// argv[2] analysis name as specified in GUI
// argv[3] hostname of GUI
// argv[4] connector port of GUI
////////////
// argv[1] "-server" when started as analysis server by GUI client
// argv[2] optional name
////////////
//         if not -gui or -server the args are for batch (designed by user):
// argv[1] -file|-trans|-stream|-evserv|-revserv|-random
// argv[2] input name (file or server)
// argv[3] optional -server // use batch arguments, but run immediately as server
// argv[4] optional -port portnumber
// argv[5] optional -output
// argv[6] optional number of events

Bool_t servermode=kFALSE; // run analysis slave as servertask
Bool_t autorun=kFALSE;    // immedeately run analysis on start
Int_t  runningMode;       // kGUI or kBatch
Int_t  maxevents = -1;    // number of events (batch mode)
Int_t  intype=GO4EV_MBS_FILE; // type of source
Text_t hostname[128];     // hostname used by GUI
UInt_t iport=5000;        // port number used by GUI
UInt_t sport=6003;        // remote event server port
Int_t  iarg;              // argument index
Text_t macro[1024];         // input name
Text_t serv[512];         // input name
Text_t Unpout[512];          // output root events
Text_t Calout[512];          // output root events
Text_t Chkout[512];          // output root events
Text_t Anlout[512];          // output root events
Text_t ASfile[512];          // auto save file (batch)
Text_t filetype[8];       // file type .lmd or .lml
Text_t odir[128];         // output directory
Text_t idir[128];         // input directory
Text_t prefix[128];         // name prefix: b for batch, i for interactive
Text_t *pc,*tmpname,*outname;

// some defaults:
strcpy(idir,"");
strcpy(odir,"");
strcpy(serv,"Go4AnalysisServer"); // name (servermode only)
strcpy(Unpout,"Unpacked");
strcpy(Calout,"Calibrated");
strcpy(Chkout,"Checked");
strcpy(Anlout,"Analyzed");
strcpy(hostname,"localhost");
// optional output path
if(getenv("COMPSTORE")!=0){
	  strcpy(odir,getenv("COMPSTORE"));
	  if(strlen(odir)>0)strcat(odir,"/");
}
else
strcpy(odir,"");
// optional input path
if(getenv("COMPSOURCE")!=0){
	  strcpy(idir,getenv("COMPSOURCE"));
	  if(strlen(idir)>0)strcat(idir,"/");
}
else
strcpy(idir,"");

if(strstr(argv[1],"-gui")) {
	if(argc < 4) {
		usage(); // too few argument for gui
		exit(0);
	}
	strcpy(prefix,"i_");
}
else if(strstr(argv[1],"-server")){
	strcpy(prefix,"s_");
}
else {
	if(argc < 3) {
		usage(); // too few argument for batch
		exit(0);
	}
	strcpy(prefix,"b_");
}
strcpy(Unpout,prefix);
strcpy(Calout,prefix);
strcpy(Chkout,prefix);
strcpy(Anlout,prefix);
strcpy(ASfile,prefix);

// Now parse arguments
   // strip any path information from input:
   // outname is name without path and postfix
   // postfix .lmd is default in all cases
   // postfix .lml is a metafile, @ is prefixed
   strcpy(filetype,".lmd");
   tmpname=argv[2]+strlen(argv[2])-4;
   if((pc=strstr(tmpname,".lmd")) != 0)*pc=0;
   if((pc=strstr(tmpname,".lml")) != 0){*pc=0;strcpy(filetype,".lml");}
   tmpname=argv[2];
   if((pc=strstr(argv[2],"@")) != 0){
	   tmpname++;
	   strcpy(idir,""); // do not use source path for lml files
   }
   outname=tmpname; // file name or node name
   if((tmpname=strrchr(outname,'/')) != 0) outname=tmpname+1;
   strncat(ASfile,outname,120);     // auto save file
   strcat(ASfile,"_AS");
   strncat(Unpout,outname,110);     // output root file
   strcat(Unpout,"_Unpacked"); // append name of output event object
   strncat(Calout,outname,110);     // output root file
   strcat(Calout,"_Calibrated"); // append name of output event object
   strncat(Chkout,outname,110);     // output root file
   strcat(Chkout,"_Checked"); // append name of output event object
   strncat(Anlout,outname,110);     // output root file
   strcat(Anlout,"_Analysis");   // append name of output event object
   strncpy(serv,argv[2],120);     // input (file with full path)

if(strstr(argv[1],"-gui"))
{
// set up arguments for GUI mode
   runningMode = kGUI;
   strncpy(hostname,argv[3],110);
   iport = (argc>4) ? atoi(argv[4]) : 5000; // port of GUI server
 }
else if(strstr(argv[1],"-server"))
 {
// set up analysis server mode started from GUI -client
   runningMode = kGUI;
   servermode=kTRUE;
   autorun=kFALSE;
 }
else
// set up arguments for batch mode
{
   runningMode = kBatch;
        if(strstr(argv[1],"-f")){intype=GO4EV_MBS_FILE;
			strncpy(serv,idir,250);
			strncat(serv,argv[2],120);     // input (file with full path)
			strcat(serv,filetype);}
   else if(strstr(argv[1],"-t")) intype=GO4EV_MBS_TRANSPORT;
   else if(strstr(argv[1],"-s")) intype=GO4EV_MBS_STREAM;
   else if(strstr(argv[1],"-e")) intype=GO4EV_MBS_EVENTSERVER;
   else if(strstr(argv[1],"-ra"))intype=GO4EV_MBS_RANDOM;
   else if(strstr(argv[1],"-r")) intype=GO4EV_MBS_REVSERV;
   else {
       cout << "invalid input type: " << argv[1] << endl;
       usage(); // print usage
       exit(0);
     }
   iarg=3;
   if(argc > iarg){
          if(strstr(argv[iarg],"-s"))servermode=kTRUE;
     else if(strstr(argv[iarg],"-p")){iarg++; sport=atoi(argv[iarg]);}
     else maxevents=atoi(argv[iarg]);
     iarg++;
   }
   if(argc > iarg){
          if(strstr(argv[iarg],"-p")){iarg++; sport=atoi(argv[iarg]);}
     else maxevents=atoi(argv[iarg]);
     iarg++;
   }
   if(argc > iarg){
     maxevents=atoi(argv[iarg]);
     iarg++;
   }
   if(argc > iarg) maxevents=atoi(argv[iarg]);

   if(maxevents == -1) maxevents = 0x7fffffff;
   cout << endl << "Comp> Input " << serv << " (" << argv[1] << ") ";
   if(strstr(argv[1],"-r")) cout << "port " << sport;
   cout << endl << "       process " << maxevents << " events" << endl;
   if(servermode){
     autorun=kTRUE;
     runningMode = kGUI;
     cout << "Comp> Analysis running in server mode. GUIs may connect!" << endl;
   }
 }//batch
// Now setup the  analysis itself
// arguments could be adjusted for other needs

//=================== log message settings =============================
TGo4Log::Instance();        // init logger object
TGo4Log::SetIgnoreLevel(1); // set this to 1 to suppress detailed debug output
                            // set this to 2 to get warnings and errors only
                            // set this to 3 to get errors only
TGo4Log::OpenLogfile("CompLogfile.txt","--Logfile for go4 Comp--",kFALSE);
TGo4Log::LogfileEnable(kFALSE); // will enable or disable logging all messages

  CompAnalysis* analysis = new CompAnalysis();
  TGo4StepFactory*  unpackfactory  = new TGo4StepFactory("UnpackFact");
  unpackfactory->DefEventProcessor("Unpacker","CompUnpackProc");// object name, class name
  unpackfactory->DefOutputEvent("Unpacked","CompUnpackEvent"); // object name, class name
  TGo4AnalysisStep* unpackstep     = new TGo4AnalysisStep("Unpacker",unpackfactory,0,0,0);
  analysis->AddAnalysisStep(unpackstep);
  if(intype==GO4EV_MBS_FILE)
    unpackstep->SetEventSource(new TGo4MbsFileParameter(serv));
  if(intype==GO4EV_MBS_TRANSPORT)
    unpackstep->SetEventSource(new TGo4MbsTransportParameter(serv));

//  TGo4StepFactory*  califactory  = new TGo4StepFactory("CaliFact");
//  califactory->DefEventProcessor("Calibrator","CompCaliProc");// object name, class name
//  califactory->DefInputEvent("Unpacked","CompUnpackEvent"); // object name, class name
//  califactory->DefOutputEvent("Calibrated","CompCaliEvent"); // object name, class name
//  TGo4AnalysisStep* calistep     = new TGo4AnalysisStep("Calibrator",califactory,0,0,0);
//  analysis->AddAnalysisStep(calistep);

  TGo4StepFactory*  checkfactory  = new TGo4StepFactory("CheckFact");
  checkfactory->DefEventProcessor("Checker","CompCheckProc");// object name, class name
  checkfactory->DefInputEvent("Unpacked","CompUnpackEvent"); // object name, class name
  checkfactory->DefOutputEvent("Checked","CompCheckEvent"); // object name, class name
  TGo4AnalysisStep* checkstep     = new TGo4AnalysisStep("Checker",checkfactory,0,0,0);
  analysis->AddAnalysisStep(checkstep);

  TGo4StepFactory*  analysisfactory  = new TGo4StepFactory("AnalysisFact");
  analysisfactory->DefEventProcessor("Analyzer","CompAnlProc");// object name, class name
  analysisfactory->DefInputEvent("Checked","CompCheckEvent"); // object name, class name
  analysisfactory->DefOutputEvent("Analyzed","CompAnlEvent"); // object name, class name
  TGo4AnalysisStep* analysisstep     = new TGo4AnalysisStep("Analyzer",analysisfactory,0,0,0);
  analysis->AddAnalysisStep(analysisstep);

// use macros to set up
  snprintf(macro,1023,".x setup.C(\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\")",
		  idir,odir,ASfile,Unpout,Calout,Chkout,Anlout);
  gROOT->ProcessLine(macro);


if(servermode)
{
   //==================== password settings for gui login (for analysis server only)
   analysis->SetAdministratorPassword("Compadmin");
   analysis->SetControllerPassword("Compctrl");
   analysis->SetObserverPassword("Compview");
   // note: do not change go4 default passwords for analysis in client mode
   // autoconnect to gui server will not work then!!!
}

// analysis->Print();
// ===================== End analysis setup ================================

if(runningMode == kBatch)
{
	cout << "Comp> Main: starting analysis in batch mode ...  " << endl;
	if (analysis->InitEventClasses() )
	{
		analysis->RunImplicitLoop(maxevents);
		delete analysis;
		cout << "Comp> Main: Done!"<<endl;
		gApplication->Terminate();
	}
	else
	{
		cout << "Comp> Main: Init event classes failed, aborting!"<<endl;
		gApplication->Terminate();
	}
}
if(runningMode == kGUI)
{
	if(servermode)  cout << "Comp> Main: starting analysis in server mode ..." << endl;
	else            cout << "Comp> Main: starting analysis in slave mode ..." << endl;
	// to start histogram server: kTRUE,"base","password"
	TGo4AnalysisClient* client = new TGo4AnalysisClient(serv, analysis, hostname, iport, kFALSE, "", "", servermode, autorun);
	// other ctor needs at least 5 argcs, disabled..
	//TGo4AnalysisClient* client = new TGo4AnalysisClient(argc,argv,analysis,kFALSE,"","",servermode, autorun);
	cout << "Comp> Main created AnalysisClient Instance: "<<client->GetName()<<endl;
}
//=================  start root application loop ==========================
cout << "Comp> Run the application loop" << endl;
theApp.Run();
return 0;
}

void usage()
{
cout << endl;
cout << "* GO4  online analysis    " << endl;
cout << "* H. Essel, GSI, Darmstadt" << endl;
cout << "* calling for batch:                " << endl;
cout << "* MainUserAnalysis -file|-trans|-stream|-evserv|-revserv|-random input [-server] [-port #] [-output] [events]" <<endl;
cout << "* calling as server:                " << endl;
cout << "* MainUserAnalysis -server name [-run]" <<endl;
cout << endl;
}
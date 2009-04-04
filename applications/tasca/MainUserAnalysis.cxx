#include "Riostream.h"
#include <string.h>
#include <stdlib.h>

#include "TROOT.h"
#include "TRint.h"
#include "TApplication.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TBenchmark.h"

#include "TGo4Log.h"
#include "TascaAnalysis.h"
#include "TGo4AnalysisClient.h"
#include "Go4EventServerTypes.h"

void usage(); // print announcement

TROOT go4application("GO4","Go4 user analysis");

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
Bool_t writeout1=kFALSE;  // write output file
Bool_t writeout2=kFALSE;  // write output file
Text_t hostname[128];     // hostname used by GUI
UInt_t iport=5000;        // port number used by GUI
UInt_t sport=6003;        // remote event server port
Int_t  iarg;              // argument index
Text_t serv[128];         // input name
Text_t out[128];          // output root events
Text_t anl[128];          // output root events
Text_t asf[128];          // auto save file (batch)
Text_t filetype[8];       // file type .lmd or .lml
Text_t *pc,*tmpname,*outname;
TascaAnalysis* analysis;

// some defaults:
strcpy(serv,"Go4AnalysisServer"); // name (servermode only)
strcpy(out,"gauss_TascaEvent");
strcpy(anl,"gauss_TascaAnl");
strcpy(hostname,"localhost");

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
   if((pc=strstr(argv[2],"@")) != 0) tmpname++;
   outname=tmpname;
   if((tmpname=strrchr(outname,'/')) != 0) outname=tmpname+1;
   strncpy(asf,outname,120);     // auto save file
   strcat(asf,"_AS");
   strncpy(out,outname,110);     // output root file
   strcat(out,"_TascaEvent"); // append name of output event object
   strncpy(anl,outname,110);     // output root file
   strcat(anl,"_TascaAnl");   // append name of output event object
   strncpy(serv,argv[2],110);     // input (file with full path)

if(strstr(argv[1],"-gui"))
{
  if(argc < 4) {
   usage(); // too few argument for gui
   exit(0);
   }
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
   if(argc < 3) {
   usage(); // too few argument for gui
   exit(0);
   }

   runningMode = kBatch;
        if(strstr(argv[1],"-f")){intype=GO4EV_MBS_FILE;strcat(serv,filetype);}
   else if(strstr(argv[1],"-t")) intype=GO4EV_MBS_TRANSPORT;
   else if(strstr(argv[1],"-s")) intype=GO4EV_MBS_STREAM;
   else if(strstr(argv[1],"-e")) intype=GO4EV_MBS_EVENTSERVER;
   else if(strstr(argv[1],"-ra")) intype=GO4EV_MBS_RANDOM;
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
     else if(strstr(argv[iarg],"-o"))writeout2=kTRUE;
     else maxevents=atoi(argv[iarg]);
     iarg++;
   }
   if(argc > iarg){
          if(strstr(argv[iarg],"-p")){iarg++; sport=atoi(argv[iarg]);}
     else if(strstr(argv[iarg],"-o"))writeout2=kTRUE;
     else maxevents=atoi(argv[iarg]);
     iarg++;
   }
   if(argc > iarg){
          if(strstr(argv[iarg],"-o"))writeout2=kTRUE;
     else maxevents=atoi(argv[iarg]);
     iarg++;
   }
   if(argc > iarg) maxevents=atoi(argv[iarg]);

   if(maxevents == -1) maxevents = 99999999;
   cout << endl << "**** Input " << serv << " (" << argv[1] << ") ";
   if(strstr(argv[1],"-r")) cout << "port " << sport;
   cout << endl << "     output step1: " << out << ".root";
   if(writeout1) cout << " enabled"; else cout << " disabled";
   cout << endl << "     output step2: " << anl << ".root";
   if(writeout2) cout << " enabled"; else cout << " disabled";
   cout << endl << "     process " << maxevents << " events" << endl;
   cout << "     auto save file: " << asf << ".root" << endl << endl;
   if(servermode){
     autorun=kTRUE;
     runningMode = kGUI;
     cout << "     Analysis running in server mode. GUIs may connect!" << endl;
   }
 }
// Now setup the  analysis itself
// arguments could be adjusted for other needs

//=================== log message settings =============================
TGo4Log::Instance();        // init logger object
TGo4Log::SetIgnoreLevel(1); // set this to 1 to suppress detailed debug output
                            // set this to 2 to get warnings and errors only
                            // set this to 3 to get errors only
TGo4Log::OpenLogfile("TascaLogfile.txt","--Logfile for go4 Tasca--",kFALSE);
TGo4Log::LogfileEnable(kFALSE); // will enable or disable logging all messages

analysis = new TascaAnalysis(serv,intype,sport,out,anl,writeout1,writeout2);
if(servermode)
{
   //==================== password settings for gui login (for analysis server only)
   analysis->SetAdministratorPassword("Tascaadmin");
   analysis->SetControllerPassword("Tascactrl");
   analysis->SetObserverPassword("Tascaview");
   // note: do not change go4 default passwords for analysis in client mode
   // autoconnect to gui server will not work then!!!
}

analysis->SetAutoSaveFile(asf);   // optional
analysis->SetAutoSaveInterval(0); // after n seconds , 0 = at termination of event loop
analysis->SetAutoSave(kFALSE);    // optional

// analysis->Print();
// ===================== End analysis setup ================================

 if(runningMode == kBatch)
   {
     cout << "**** Main: starting analysis in batch mode ...  " << endl;
     analysis->SetAutoSave(kTRUE);   // optional enable auto-save
     if (analysis->InitEventClasses() )
       {
    analysis->RunImplicitLoop(maxevents);
    delete analysis;
    cout << "**** Main: Done!"<<endl;
    gApplication->Terminate();
       }
     else
       {
    cout << "**** Main: Init event classes failed, aborting!"<<endl;
    gApplication->Terminate();
       }
   }
 if(runningMode == kGUI)
   {
     if(servermode)  cout << "**** Main: starting analysis in server mode ..." << endl;
     else            cout << "**** Main: starting analysis in slave mode ..." << endl;
     // to start histogram server: kTRUE,"base","password"
     TGo4AnalysisClient* client = new TGo4AnalysisClient(serv, analysis, hostname, iport, kFALSE, "", "", servermode, autorun);
     // other ctor needs at least 5 argcs, disabled..
     //TGo4AnalysisClient* client = new TGo4AnalysisClient(argc,argv,analysis,kFALSE,"","",servermode, autorun);
     cout << "**** Main created AnalysisClient Instance: "<<client->GetName()<<endl;
   }
//=================  start root application loop ==========================
cout << "Run the application loop" << endl;
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
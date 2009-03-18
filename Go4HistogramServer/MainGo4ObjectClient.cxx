/************************************************************************
* This example shows how to use the TGo4ObjectClient class
* to receive any registered root object from a running Go4 analysis.
* The instance of TGo4ObjectClient stores all information required to
* connect, like hostname, port,  name of the database, password.
* This information is set in the class constructor, but can be changed
* with methods
*       SetHost(const Text_t*), SetPort(Int_t)
*       SetBase(const Text_t*), SetPasswd(const Text_t*)
* without destroying the client object.
* The connection to Go4 object server is established anew at any
* time an object or the list of names is requested from the server,
* and disconnected again after the object is received.
* Method
*      TGo4AnalysisObjectNames* TGo4ObjectClient::RequestNamesList()
* requests the go4 nameslist object from the specified object server
+ and delivers pointer to the object when received. Nameslist object
* has to be deleted by the user of the client object afterwards.
* Returns 0 in case of error.
* Method
*      TObject* TGo4ObjectClient::RequestObject(const Text_t*)
* requests a root object by name from the specified object server
+ and delivers pointer to the object when received. Object
* has to be deleted by the user of the client afterwards.
* Returns 0 in case of error or object not found. Name string may
* contain the full path to the object in the go4 folder structure
*********************
* The example expects the client parameters host,port,base,passwd,
* and a (in this case) dummy command string.
* as command line parameters. Then a command input loop starts.
* Typing the object name will request object and draw it on the
* canvas, if histogram. Other objects are just printed.
* Typing "dir" will request and printout the list of objects
* available on the server.
* Typing "exit" will finish the object client.
**********************************************************************/

#include <string>
#include <stdlib.h>

#include "Riostream.h"
#include "TROOT.h"
#include "TApplication.h"
#include "TCanvas.h"

#include "TGo4Log.h"
#include "TGo4ObjClient.h"
#include "TGo4AnalysisObjectNames.h"

void usage(); // print announcement

TROOT go4application("GO4 object client","This is the Go4 object client");

int main(int argc, char **argv)
{
   TApplication theApp("App",0 , 0);
   TCanvas mycanvas("Object client", "Go4 object client test");
   TGo4Log::Instance(); // init logger object
   TGo4Log::SetIgnoreLevel(0); // set this to 1 to suppress detailed debug output
                               // set this to 2 to get warnings and errors only
                               // set this to 3 to get errors only
   TGo4Log::LogfileEnable(kFALSE); // will enable or disable logging all messages to file

   if(argc<6)
      {
         usage();
      }
   else
      {
         const char* hostname = argv[1];
         const char* connector = argv[2];
         Int_t port=atoi(connector);
         const char* base = argv[3];
         const char* pass = argv[4];
         const char* comm = argv[5];
         cout <<"Host:"<<hostname<<",\tPort:"<<port<<",\tBase:"<<base<<",\tPass:"<<pass<<",\tComm:"<<comm<<endl;
         TGo4ObjClient myclient("TestClient", base, pass, hostname,port);
         // testing repeated access:
//         TGo4AnalysisObjectNames* list=0;
//         for(Int_t t=0;t<50;++t)
//            {
//               //cout <<"Requesting nameslist "<<t << endl;
//               list=myclient.RequestNamesList();
//            }
//         if(list) list->Print();

//TObject* arr[100];
//mycanvas.Divide(10,10);
//for(Int_t t=0; t<100;++t)
//{
//    //cout <<"requesting "<<t << endl;
//    arr[t]=myclient.RequestObject(comm);
//
//}
//
//for(Int_t t=0; t<100;++t)
//{
//    mycanvas.cd(t+1);
//    if(arr[t]) arr[t]->Draw();
//    mycanvas.Update();
//}

         string inputline;
         while(1)
         {
             cout <<"Go4> Object client command:"<< endl;
             getline(cin,inputline);
             comm = inputline.c_str();

            if(!strcmp(comm,"exit"))
               {
                  cout <<"exit command..." << endl;
                  gApplication->Terminate();
               }
            else if(!strcmp(comm,"dir"))
               {
                  cout <<"getting nameslist" << endl;
                  TGo4AnalysisObjectNames* list=myclient.RequestNamesList();

                  if(list)
                     list->PrintStatus();
                  else
                     cout <<"got zero list!!!" << endl;
               }
            else
               {
                  cout <<"getting object "<< comm << endl;
                  TObject* ob=myclient.RequestObject(comm);
                  if(ob)
                     {
                        ob->Print();

                        if(ob->InheritsFrom("TH1"))
                          {
                             mycanvas.cd();
                             ob->Draw();
                             mycanvas.Modified();
                             mycanvas.Update();
                          }

                     }
                  else
                     {
                        cout <<"got zero object!!!" << endl;
                     }
               } // if(!strcmp(comm,"dir"))
         }// while(inputline...
      theApp.Run();
      }
return 0;
}

void usage()
{
   cout << "usage: MainGo4ObjectClient hostname port base passwd command "<<endl;
}

//---------------------------------------------
// Go4 Comp analysis
// Author: Hans G. Essel
//         H.Essel@gsi.de
// GSI, Experiment Electronics, Data Processing
//---------------------------------------------

#include "CompAnlProc.h"

#include "Riostream.h"

#include "TH1.h"
#include "TH2.h"
#include "TGraph.h"
#include "TMultiGraph.h"

#include "TGo4WinCond.h"

#include "CompAnlEvent.h"
#include "CompEventStack.h"
#include "CompEvent.h"
#include "CompCheckEvent.h"
#include "CompControl.h"
#include "CompParameter.h"
#include "TGo4WinCond.h"
#include "CompAnalysis.h"

//***********************************************************
CompAnlProc::~CompAnlProc()
{
  UInt_t i;
  Double_t dt=(fTimeWindow/1000000)*fiSFprocessed;
	  cout << "Comp> CompAnlProc:    Processed "<<fiEventsProcessed<<" selected "<<fiEventsWritten << endl;
	  cout << "                  stack Processed "<<fiEvprocessedTotal<<endl;
	  cout << "                  total Fissions "<<fFissions <<" processed "<<fiSFprocessed
	       << " chains "<<fChainNumber
	       << " sec "<<dt<<endl;
	  cout << "                  total Alphas   "<<fAlphas << endl;
	  cout << "                  total EVRs     "<<fEvrs << endl;
          cout << "                  Timewindow [s] "<<fTimeWindow/1000000<<endl;
	  dt=dt/1000000;
if(fControl->AnlHisto){
  for(i=0;i<fXY;i++)printf("    All   %3d,%3d %5.0f %6.0f 10^-6/s\n",
			       fX[i],fY[i],fStopXYtall->GetBinContent(fX[i]+1,fY[i]+1),fStopXYtall->GetBinContent(fX[i]+1,fY[i]+1)/dt);
  for(i=0;i<fXY;i++)printf("    Evr   %3d,%3d %5.0f %6.0f 10^-6/s %2d%%\n",
			       fX[i],fY[i],fStopXYtEvr->GetBinContent(fX[i]+1,fY[i]+1),fStopXYtEvr->GetBinContent(fX[i]+1,fY[i]+1)/dt
			   ,(UInt_t)(fStopXYtEvr->GetBinContent(fX[i]+1,fY[i]+1)*100/fStopXYtall->GetBinContent(fX[i]+1,fY[i]+1)));
  for(i=0;i<fXY;i++)printf("    Alpha %3d,%3d %5.0f %6.0f 10^-6/s %2d%%\n",
			   fX[i],fY[i],fStopXYtalp->GetBinContent(fX[i]+1,fY[i]+1)-1000,(fStopXYtalp->GetBinContent(fX[i]+1,fY[i]+1)-1000)/dt
			   ,(UInt_t)((fStopXYtalp->GetBinContent(fX[i]+1,fY[i]+1)-1000)*100/fStopXYtall->GetBinContent(fX[i]+1,fY[i]+1)));
}
if(fChainFile){
	fChainFile->Write();
	fChainFile->Close();
}
}
//***********************************************************
CompAnlProc::CompAnlProc()
  : TGo4EventProcessor(),fInput(0),fFissions(0),fAlphas(0),fEvrs(0)
{
}
//***********************************************************
// this one is used in CompAnlFact
CompAnlProc::CompAnlProc(const char* name) :
  TGo4EventProcessor(name),fInput(0),fChainFile(0),fChainNumber(0),fChainStore(kFALSE),fFissions(0),fAlphas(0),fEvrs(0)
{
  UInt_t i;
  cout << "Comp> CompAnlProc "<<name<<" created" << endl;
  //// init user analysis objects:


  anl=(CompAnalysis *)TGo4Analysis::Instance();

  fControl = (CompControl *)   anl->CreateParameter("Control","Controls");
  fParam   = (CompParameter *) anl->CreateParameter("Parameter","Parameters");
  fwinEVR     = (TGo4WinCond *)anl->CreateCondition("Check","EvrH",0,kTRUE,0,1);
  fwinAlpha   = (TGo4WinCond *)anl->CreateCondition("Check","AlphaL",0,kTRUE,0,1);
  fwinAlpha1  = (TGo4WinCond *)anl->CreateCondition("Check","Alpha1L",0,kTRUE,0,1);
  fwinAlpha2  = (TGo4WinCond *)anl->CreateCondition("Check","Alpha2L",0,kTRUE,0,1);
  fwinFission1= (TGo4WinCond *)anl->CreateCondition("Check","Fission1H",0,kTRUE,0,1);
  fwinFission2= (TGo4WinCond *)anl->CreateCondition("Check","Fission2H",0,kTRUE,0,1);
  fwinBack    = (TGo4WinCond *)anl->CreateCondition("Check","BackH",0,kTRUE,0,1);
  gROOT->ProcessLine(".x setparam.C()");
  gROOT->ProcessLine(".x setcontrol.C()");

if(fControl->AnlHisto){
	  fStopXYalp   =anl->CreateTH2I("Anl","XYLhitsAlpha","All hits Alpha","X position [stripe]","Y position [stripe]","Hits",144,0,144,48,0,48);
	  fStopXYEvr   =anl->CreateTH2I("Anl","XYHhitsEvr","All hits EVR","X position [stripe]","Y position [stripe]","Hits",144,0,144,48,0,48);
	  fStopXYSF    =anl->CreateTH2I("Anl","XYHhitsSF","All hits SF","X position [stripe]","Y position [stripe]","Hits",144,0,144,48,0,48);
	  fStopXYSFoff =anl->CreateTH2I("Anl","XYHhitsSFoff","All hits SF offbeam","X position [stripe]","Y position [stripe]","Hits",144,0,144,48,0,48);
	  fStopXYall   =anl->CreateTH2I("Anl","XYhitsAll","All hits","X position [stripe]","Y position [stripe]","Hits",144,0,144,48,0,48);
	  fStopXYcalp  =anl->CreateTH2I("Anl","XYLhitsAlphaCH","Alpha hits in chains","X position [stripe]","Y position [stripe]","Hits",144,0,144,48,0,48);
	  fStopXYcall  =anl->CreateTH2I("Anl","XYLhitsAllCH","All hits in chains","X position [stripe]","Y position [stripe]","Hits",144,0,144,48,0,48);
	  fStopXYcEvr  =anl->CreateTH2I("Anl","XYHhitsEvrCH","Evr hits in chains","X position [stripe]","Y position [stripe]","Hits",144,0,144,48,0,48);
	  fStopXYcSF   =anl->CreateTH2I("Anl","XYHhitsSFCH","SF hits in chains","X position [stripe]","Y position [stripe]","Hits",144,0,144,48,0,48);
	  fStopXYtalp  =anl->CreateTH2I("Anl","XYLhitsAlphaDT","Alpha hits before SF","X position [stripe]","Y position [stripe]","Hits",144,0,144,48,0,48);
	  fStopXYtEvr  =anl->CreateTH2I("Anl","XYHhitsEvrDT","Evr hits before SF","X position [stripe]","Y position [stripe]","Hits",144,0,144,48,0,48);
	  fStopXYtSF   =anl->CreateTH2I("Anl","XYHhitsSFDT","SF its before SF","X position [stripe]","Y position [stripe]","Hits",144,0,144,48,0,48);
	  fStopXYtSFoff=anl->CreateTH2I("Anl","XYHhitsSFoffDT","SF offbeam hits before SF","X position [stripe]","Y position [stripe]","Hits",144,0,144,48,0,48);
	  fStopXYtall  =anl->CreateTH2I("Anl","XYhitsAllDT","All hits before SF","X position [stripe]","Y position [stripe]","Hits",144,0,144,48,0,48);
	  fXY=15;// number of approved chain positions
	  i=0;
	  fX[i]=91; fY[i]=18; i++;
	  fX[i]=103; fY[i]=31; i++;
	  fX[i]=83; fY[i]=18; i++;
	  fX[i]=102; fY[i]=24; i++;
	  fX[i]=110; fY[i]=19; i++;
	  fX[i]=119; fY[i]=29; i++;
	  fX[i]=131; fY[i]=26; i++;
	  fX[i]=22; fY[i]=11; i++;
	  fX[i]=59; fY[i]=24; i++;
	  fX[i]=86; fY[i]=21; i++;
	  fX[i]=89; fY[i]=24; i++;
	  fX[i]=100; fY[i]=5; i++;
	  fX[i]=122; fY[i]=40; i++;
	  fX[i]=59; fY[i]=47; i++;
	  fX[i]=89; fY[i]=0; i++;
	  fX[i]=0; fY[i]=0; i++;
	  for(i=0;i<fXY;i++){
	    fStopXYcall->SetBinContent(fX[i]+1,fY[i]+1,100); // bin access must start from 1
	    fStopXYtalp->SetBinContent(fX[i]+1,fY[i]+1,1000);
	    // fStopXYtalp->Fill(fX[i],fY[i]);  fills the same bin as set by SetBinContent
	  }
}
  fTimeWindow=fParam->Fission2Tmax+fParam->Alpha2Tmax;
  // print description *********************************************************
  cout<<"*****************************************************************************"<<endl;
  cout<<"      Full stack, All fissions offbeam with Y, Y +-1"<<endl;
  cout<<"      Timewindow [s] "<<fTimeWindow/1000000<<endl;
  cout<<"*****************************************************************************"<<endl;

  fEventStack=new CompEventStack(fParam->EventStackSize);
  fStackIter = new TListIter(fEventStack);
  fEventIter = new TListIter(fEventStack);
  fEventCurrent=(CompEvent *) fEventStack->First();
  fStackLinkCurrent=fEventStack->FirstLink();
  fStackfilled=kFALSE;
  fFirstEvent=0;
  fiEventsProcessed=0;
  fiEventsWritten=0;
  fiSFprocessed=0;
  fiSFtaken=0;
  fiEvprocessed=0;
  fiEvprocessedTotal=0;
  fAlphaFound=kFALSE;
  fEvrFound=kFALSE;
  fiLastFissionEvent=0;
  fChainStore=fControl->writeChainTree;
  fChainNumber=fControl->ChainCounter;
  }
//***********************************************************
void CompAnlProc::PrintFission(Bool_t store){
fFissionEvent->PrintFission(kFALSE,fFissions);
fiEventsWritten++;
fFissionEvent->fiChainNumber=fChainNumber;
if(store){
	fChainBranch->SetAddress(&fFissionEvent);
	fChainTree->Fill();
}
return;
}
void CompAnlProc::PrintAlpha(Bool_t store){
fStackEvent->PrintAlpha(kFALSE,
		(Float_t)TimeDiff(fFissionEvent->fiTimeStamp,fStackEvent->fiTimeStamp)/1000000.);
fiEventsWritten++;
fStackEvent->fiChainNumber=fChainNumber;
if(store){
	fChainBranch->SetAddress(&fStackEvent);
	fChainTree->Fill();
	}
return;
}
void CompAnlProc::PrintEvr(Bool_t store){
fStackEvent->PrintEvr(kFALSE,
		(Float_t)TimeDiff(fFissionEvent->fiTimeStamp,fStackEvent->fiTimeStamp)/1000000.);
fiEventsWritten++;
fStackEvent->fiChainNumber=fChainNumber;
if(store){
	fChainBranch->SetAddress(&fStackEvent);
	fChainTree->Fill();
	}
return;
}
//-----------------------------------------------------------
void CompAnlProc::CompEventAnalysis(CompAnlEvent* poutevt)
{
poutevt->SetValid(kFALSE);       // events are not stored until kTRUE is set
fInput  = (CompCheckEvent*) GetInputEvent();
// Process only if event is valid
//cout <<"Anl: "<<fInput->fiEventNumber<< endl;
if(!fInput->IsValid()) return;
fiEventsProcessed++;
// Check if we should open output tree file
if(fChainStore){
if(fChainFile==0){
	  // open root file for output
	  strcpy(cfilename,fInput->GetEventSource()->GetActiveName());
	  char* pc=strstr(cfilename,"_Checked");
	  if(pc)*pc=0;
	  strcat(cfilename,"_Chains.root");
	  cout<< "Comp> CompAnlProc "<<"create Chain file "<<cfilename<<endl;
	  fChainFile = new TFile(cfilename,"RECREATE");
	  // create tree
	  fChainTree = new TTree("SearcherTree","new");
	  fEvent=new CompEvent();
	  // create one branch. The dot adds the event name to each member name
	  fChainBranch=fChainTree->Branch("Searched.","CompEvent",&fEvent,100000,1);
}}

if(fControl->AnlHisto){
	if(fInput->fisFission){
	fStopXYall->Fill(fInput->fiStopXHhitI,fInput->fiStopYHhitI%48);
		if(fInput->fisMacro)
		     fStopXYSF->Fill(fInput->fiStopXHhitI,fInput->fiStopYHhitI%48);
		else fStopXYSFoff->Fill(fInput->fiStopXHhitI,fInput->fiStopYHhitI%48);
	}
	if(fInput->fisAlpha){
	fStopXYall->Fill(fInput->fiStopXLhitI,fInput->fiStopYLhitI%48);
	fStopXYalp->Fill(fInput->fiStopXLhitI,fInput->fiStopYLhitI%48);
	}
	if(fInput->fisEvr){
	fStopXYall->Fill(fInput->fiStopXHhitI,fInput->fiStopYHhitI%48);
	fStopXYEvr->Fill(fInput->fiStopXHhitI,fInput->fiStopYHhitI%48);
	}
}
if(fInput->fisFission) fFissions++;
if(fInput->fisAlpha)   fAlphas++;
if(fInput->fisEvr)     fEvrs++;

if(fFirstEvent==0)fFirstEvent=fInput->fiEventNumber;

// We assume that fEventCurrent is a valid slot.
// With first event this is first slot
fInput->CopyTo(fEventCurrent); // copy event data into event entry
fEventStack->used++;
// From fission event we go back
if(fInput->fisFission&&(!fInput->fisMacro)&&(fInput->fiStopYHhitI>=0)){
	// if(fInput->fisFission&&(fInput->fiStopYHhitI>=0)){
	// if(fInput->fisFission&(fInput->ffBackHhitV>10)){
	// if(fInput->fisFission){
	if(fControl->AnlHisto)fStopXYtSF->Fill(fInput->fiStopXHhitI,fInput->fiStopYHhitI%48);
	fiSFprocessed++;
	fAlphaFound=kFALSE;
	fEvrFound=kFALSE;
	fFissionEvent=fEventCurrent; // keep that as end point
	fStackEvent=fEventCurrent;  // to step back
	fTimeDiff=fFissionEvent->fiDeltaTime;
	fStackLinkEntry=fStackLinkCurrent;
	fiFileNumber=fFissionEvent->fiFileNumber;
	// search backwards
	while((fStackLinkEntry=fStackLinkEntry->Prev())!=0){
		fStackEvent=(CompEvent *) fStackLinkEntry->GetObject();
		//if(fTimeDiff <= fParam->Alpha2Tmax){ // in alpha time window
		if(fStackEvent->fisAlpha){
			if((fFissionEvent->fiStopXHhitI    ==fStackEvent->fiStopXLhitI)
					&&((fFissionEvent->fiStopYHhitI   ==fStackEvent->fiStopYLhitI)
							||((fFissionEvent->fiStopYHhitI+1)==fStackEvent->fiStopYLhitI)
							||((fFissionEvent->fiStopYHhitI-1)==fStackEvent->fiStopYLhitI))
			){
				if(fControl->AnlHisto)fStopXYcall->Fill(fStackEvent->fiStopXLhitI,fStackEvent->fiStopYLhitI%48);
				if(fControl->AnlHisto)fStopXYcalp->Fill(fStackEvent->fiStopXLhitI,fStackEvent->fiStopYLhitI%48);
				fAlphaFound=kTRUE;
			} else {
				if(fControl->AnlHisto)fStopXYtalp->Fill(fStackEvent->fiStopXLhitI,fStackEvent->fiStopYLhitI%48);
				if(fControl->AnlHisto)fStopXYtall->Fill(fStackEvent->fiStopXLhitI,fStackEvent->fiStopYLhitI%48);
			}
		}
		if(fStackEvent->fisEvr){
			if((fFissionEvent->fiStopXHhitI    ==fStackEvent->fiStopXHhitI)
					&&((fFissionEvent->fiStopYHhitI   ==fStackEvent->fiStopYHhitI)
							||((fFissionEvent->fiStopYHhitI+1)==fStackEvent->fiStopYHhitI)
							||((fFissionEvent->fiStopYHhitI-1)==fStackEvent->fiStopYHhitI))
			){
				if(fControl->AnlHisto)fStopXYcall->Fill(fStackEvent->fiStopXHhitI,fStackEvent->fiStopYHhitI%48);
				if(fControl->AnlHisto)fStopXYcEvr->Fill(fStackEvent->fiStopXHhitI,fStackEvent->fiStopYHhitI%48);
				fEvrFound=kTRUE;
			} else {
				if(fControl->AnlHisto)fStopXYtall->Fill(fStackEvent->fiStopXHhitI,fStackEvent->fiStopYHhitI%48);
				if(fControl->AnlHisto)fStopXYtEvr->Fill(fStackEvent->fiStopXHhitI,fStackEvent->fiStopYHhitI%48);
			}
		}
		if(fTimeDiff > fTimeWindow)break; // out of time window
		fTimeDiff += fStackEvent->fiDeltaTime;
		fiEvprocessed++;
		fiEvprocessedTotal++;
	} // while back loop
	// We go now from here forward up to fission to print correct order
	fTimeDiff=0;
	if(fEvrFound&&fAlphaFound){
		if(fControl->AnlHisto)fStopXYcSF->Fill(fFissionEvent->fiStopXHhitI,fFissionEvent->fiStopYHhitI%48);
		if(fControl->AnlHisto)fStopXYcall->Fill(fFissionEvent->fiStopXHhitI,fFissionEvent->fiStopYHhitI%48);
		fiSFtaken++;
		fChainNumber++;
		fControl->ChainCounter++;
		if(fStackLinkEntry==0){
			cout<<"End of stack, entries "<<fEventStack->used<<endl;
			fStackEvent=(CompEvent *) fEventStack->First();
			fStackLinkEntry=fEventStack->FirstLink();
		}
		//printf("t018f%03d%04d.lmd\n",fiFileNumber>>16,fiFileNumber&0xffff);
		printf("Chain %4d Scope %7.3f sec Run %3d File %4d Stack %d --=========================================\n",
				fChainNumber,(Float_t)TimeDiff(fFissionEvent->fiTimeStamp,fStackEvent->fiTimeStamp)/1000000.,
				fiFileNumber>>16,fiFileNumber&0xffff,fiEvprocessed);
		fiEvprocessed=0;
		fAlphaFound=kFALSE;
		fEvrFound=kFALSE; //if set true, fake for offbeam
		while(fStackEvent!=fFissionEvent){
			if(fStackEvent->fisEvr
					&&(fFissionEvent->fiStopXHhitI    ==fStackEvent->fiStopXHhitI)
					&&((fFissionEvent->fiStopYHhitI   ==fStackEvent->fiStopYHhitI)
							||((fFissionEvent->fiStopYHhitI+1)==fStackEvent->fiStopYHhitI)
							||((fFissionEvent->fiStopYHhitI-1)==fStackEvent->fiStopYHhitI))
			){
				fEvrFound=kTRUE;
				if(fStackEvent->fiFileNumber!=fiFileNumber)
					printf("+t018f%03d%04d.lmd\n",fStackEvent->fiFileNumber>>16,fStackEvent->fiFileNumber&0xffff);
				PrintEvr(fChainStore);
			}
			else if(fStackEvent->fisAlpha
					&&(fFissionEvent->fiStopXHhitI    ==fStackEvent->fiStopXLhitI)
					&&((fFissionEvent->fiStopYHhitI   ==fStackEvent->fiStopYLhitI)
							||((fFissionEvent->fiStopYHhitI+1)==fStackEvent->fiStopYLhitI)
							||((fFissionEvent->fiStopYHhitI-1)==fStackEvent->fiStopYLhitI))
			){
				fAlphaFound=kTRUE;
				if(fStackEvent->fiFileNumber!=fiFileNumber)
					printf("+t018f%03d%04d.lmd\n",fStackEvent->fiFileNumber>>16,fStackEvent->fiFileNumber&0xffff);
				PrintAlpha(fChainStore);
			}
			else if((fStackEvent->fisFission)&&(fiLastFissionEvent==fStackEvent->fiEventNumber))
				printf("   Fission  %9d double\n",fStackEvent->fiEventNumber);
			fStackLinkEntry=fStackLinkEntry->Next();
			fStackEvent=(CompEvent *) fStackLinkEntry->GetObject();
		} // second while loop forward
	} // found Alpha or Evr
	if(fEvrFound&&fAlphaFound){
		PrintFission(fChainStore);
		fiLastFissionEvent=fFissionEvent->fiEventNumber;
	}
} // fission
// Note that we have already filled the list with preallocated events.
// Therefore the iterator runs over all entries.
 // With return fEventCurrent must point to the next free item.
fStackLinkCurrent=fStackLinkCurrent->Next();
if(fStackLinkCurrent==0){
	if(!fStackfilled)
	cout<<"Run "<<(fInput->fiFileNumber>>16)
	    <<" file "<<(fInput->fiFileNumber&0xffff)
	    <<" all "<<fEventStack->entries<<" slots used, start shuffling"<<endl;
	fStackfilled=kTRUE;
	fStackEvent=(CompEvent *) fEventStack->First(); // the oldest one
	fEventStack->Remove(fStackEvent);  // remove from first
	fEventStack->AddLast(fStackEvent); // newest at the end
	fEventCurrent=fStackEvent;
	fStackLinkCurrent=fEventStack->LastLink();

} else
	fEventCurrent=(CompEvent *) fStackLinkCurrent->GetObject();
return;
} // BuildCalEvent
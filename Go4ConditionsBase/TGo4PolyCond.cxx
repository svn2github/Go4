// $Id$
//-----------------------------------------------------------------------
//       The GSI Online Offline Object Oriented (Go4) Project
//         Experiment Data Processing at EE department, GSI
//-----------------------------------------------------------------------
// Copyright (C) 2000- GSI Helmholtzzentrum f�r Schwerionenforschung GmbH
//                     Planckstr. 1, 64291 Darmstadt, Germany
// Contact:            http://go4.gsi.de
//-----------------------------------------------------------------------
// This software can be used under the license agreements as stated
// in Go4License.txt file which is part of the distribution.
//-----------------------------------------------------------------------

#include "TGo4PolyCond.h"

#include "Riostream.h"
#include "RVersion.h"

#include "TMath.h"
#include "TROOT.h"
#include "TH2.h"
#include "TList.h"
#include "TCutG.h"

#include "TGo4PolyCondPainter.h"
#include "TGo4Log.h"

/** JAM2016: this define switches between cloning when updating condition
 * and simple copy of TCutG points*/
//#define POLYCOND_UPDATE_WITHCLONE 1


TString TGo4PolyCond::fgxURL_NPOINTS="npolygon";
TString TGo4PolyCond::fgxURL_XPRE="x";
TString TGo4PolyCond::fgxURL_YPRE="y";

TString TGo4PolyCond::NextAvailableName()
{
   TString res;
   Int_t cnt = 0;
   do {
      res.Form("CutG_%d",cnt++);
   } while (gROOT->GetListOfSpecials()->FindObject(res)!=0);
   return res;
}

// ----------------------------------------------------------
TGo4PolyCond::TGo4PolyCond() :
   TGo4Condition(),
   fxCut(0)
{
   SetDimension(2);
   //SetBit(kCanDelete, kFALSE);
   //std::cout <<"TGo4PolyCond default ctor, this="<<(long) this <<", threadid="<< (long) pthread_self()<<std::endl;
}
// ----------------------------------------------------------
TGo4PolyCond::TGo4PolyCond(const char* name, const char* title) :
   TGo4Condition(name,title),
   fxCut(0)
{
   SetDimension(2);
   //SetBit(kCanDelete, kFALSE);
   //std::cout <<"TGo4PolyCond ctor of name:"<< name<< ", this="<<(long) this << std::endl;
}
// ----------------------------------------------------------
TGo4PolyCond::~TGo4PolyCond()
{
   //std::cout <<"TGo4PolyCond dtor of this="<<(long) this <<", threadid="<< (long) pthread_self()<< std::endl;
   //UnDraw(); // JAM 2016 - do it before cut has vanished!
   if(fxCut != 0)
     {
       //std::cout <<"TGo4PolyCond dtor of this="<<(long) this <<" deletes the cut "<< (long) fxCut << std::endl;
       delete fxCut;
       fxCut=0; // JAM2016: reset for Unpaint in PolyCondView?
     }
}

Double_t TGo4PolyCond::GetXLow()
{
   if(fxCut==0) return 0;
   Int_t n=fxCut->GetN();
   Double_t* xarr=fxCut->GetX();
   fxCut->SetBit(kMustCleanup,0);
   Int_t nxmin=TMath::LocMin(n,xarr);
   return (xarr[nxmin]);
}
Double_t TGo4PolyCond::GetXUp()
{
   if(fxCut==0) return 0;
   Int_t n=fxCut->GetN();
   Double_t* xarr=fxCut->GetX();
   Int_t nxmax=TMath::LocMax(n,xarr);
   return (xarr[nxmax]);
}
Double_t TGo4PolyCond::GetYLow()
{
   if(fxCut==0) return 0;
   Int_t n=fxCut->GetN();
   Double_t* yarr=fxCut->GetY();
   Int_t nymin=TMath::LocMin(n,yarr);
   return (yarr[nymin]);
}
Double_t TGo4PolyCond::GetYUp()
{
   if(fxCut==0) return 0;
   Int_t n=fxCut->GetN();
   Double_t* yarr=fxCut->GetY();
   Int_t nymax=TMath::LocMax(n,yarr);
   return (yarr[nymax]);
}

Bool_t TGo4PolyCond::IsPolygonType()
{
   return kTRUE;
}

// ----------------------------------------------------------
TCutG* TGo4PolyCond::GetCut(Bool_t changeowner)
{
  //std::cout <<"TGo4PolyCond "<<(long) this <<" ::GetCut for cut "<< (long) fxCut << std::endl;
   TCutG* tempcut = fxCut;

   if(changeowner) {
      fxCut = 0;
      //std::cout <<"TGo4PolyCond "<<(long) this <<" ::GetCut deletes cuthistogram"<< (long) fxCutHis << std::endl;
      delete fxCutHis;
      fxCutHis=0;

   }
   return tempcut;
}

// ----------------------------------------------------------
TCutG * TGo4PolyCond::CloneCut(TGo4PolyCond * source)
{

  TCutG * tempcut = source->GetCut(false); // get fxCut pointer
  //std::cout <<"TGo4PolyCond  "<<(long) this <<" CloneCut "<< (long) tempcut<<"from polycond "<< (long) source << std::endl;
  if(tempcut)
  {
    CleanupSpecials(); // JAM2016: Clone might delete cut of same name from list of specials, remove it first
    TCutG* ret= (TCutG *)tempcut->Clone(GetName());
    CleanupSpecials();
    return ret;
  }
  else return 0;
}
// ----------------------------------------------------------
void TGo4PolyCond::SetValues(TCutG * newcut)
{
   if(newcut==0) return;
#ifdef POLYCOND_UPDATE_WITHCLONE
   if(fxCut!=0) delete fxCut;
   CleanupSpecials(); // JAM2016: Clone might delete cut of same name from list of specials, remove it first
   //fxCut = (TCutG*) newcut->Clone(NextAvailableName());
   fxCut = (TCutG*) newcut->Clone(GetName());
   fxCut->SetBit(kCanDelete,kFALSE);
   CleanupSpecials();
#else
   Int_t pn = newcut->GetN();
   if(fxCut==0)
   {
     fxCut = new TCutG(GetName(),pn);
     fxCut->SetBit(kMustCleanup);
     TGo4PolyCond::CleanupSpecials(); // JAM2016
   }
   else
   {
     fxCut->Set(pn);
   }
   Double_t xp=0;
   Double_t yp=0;
   for(Int_t i=0; i<pn; ++i) {
     newcut->GetPoint(i,xp,yp);
     fxCut->SetPoint(i,xp,yp);
   }


#endif
   // when updated from view, we store graphical attributes:
   SetLineColor(newcut->GetLineColor());
   SetLineWidth(newcut->GetLineWidth());
   SetLineStyle(newcut->GetLineStyle());
   SetFillColor(newcut->GetFillColor());
   SetFillStyle(newcut->GetFillStyle());
   //std::cout << "TGo4PolyCond::SetValues deletes cut histogram " << (long) fxCutHis << std::endl;
   delete fxCutHis; // fxCut changed, so discard previous fxCut histogram
   fxCutHis=0;
   //std::cout << "TGo4PolyCond::SetValues fxCut " << (long) fxCut << " from " << (long) newcut << std::endl;
}
 // ----------------------------------------------------------
void TGo4PolyCond::SetValuesDirect(TCutG * newcut)
{
   if(newcut==0) return;
   if(fxCut!=0 && fxCut!=newcut) delete fxCut;

   fxCut = newcut;
   //fxCut->SetName(NextAvailableName()); // JAM2016
   fxCut->SetName(GetName());

   // when updated from view, we store graphical attributes:
   SetLineColor(newcut->GetLineColor());
   SetLineWidth(newcut->GetLineWidth());
   SetLineStyle(newcut->GetLineStyle());
   SetFillColor(newcut->GetFillColor());
   SetFillStyle(newcut->GetFillStyle());

   delete fxCutHis; // fxCut changed, so discard previous fxCut histogram
   fxCutHis=0;
   //std::cout << "TGo4PolyCond::SetValuesDirect  fxCut " << (long) fxCut << " from " << (long)newcut << std::endl;
}


// ----------------------------------------------------------
void TGo4PolyCond::SetValues(Double_t * x, Double_t * y, Int_t len)
{
   if(fxCut != 0) delete fxCut;
   TGo4PolyCond::CleanupSpecials(); // JAM2016
   //fxCut = new TCutG(NextAvailableName(), len, x, y);
   fxCut = new TCutG(GetName(), len, x, y);
   fxCut->SetBit(kMustCleanup);
   //fxCut->SetBit(kCanDelete,kFALSE);
   TGo4PolyCond::CleanupSpecials(); // JAM2016

   //std::cout << "TGo4PolyCond::SetValues() Set new fxCut " << (long) fxCut << std::endl;
   delete fxCutHis; // discard previous fxCut histogram
   fxCutHis=0;
}
// ----------------------------------------------------------
Bool_t TGo4PolyCond::Test(Double_t x, Double_t y)
{
   IncCounts();
   if((!IsEnabled()) || (fxCut == 0)){
      if(FixedResult()) IncTrueCounts();
      return FixedResult();
   }
   Bool_t outside = (fxCut->IsInside(x,y) == 0);
   if(outside) return IsFalse();
   IncTrueCounts();
   return IsTrue();
}
// ----------------------------------------------------------
void TGo4PolyCond::PrintCondition(Bool_t points)
{
   TGo4Condition::PrintCondition();
   if(points) {
      if(fxCut == 0)
         std::cout << "No polygon specified!" << std::endl;
      else
         fxCut->Print(0);
   }
}

// -----------------------------------------------
// PrintBar switch is handled in condition baseclass now.
//void TGo4PolyCond::Print(Option_t* opt) const{
//// this trick is needed since root defines Print as const function...
//   TGo4PolyCond* const localthis= const_cast<TGo4PolyCond* const>(this);
//   localthis->PrintBar();
//}
// ----------------------------------------------------------
Bool_t TGo4PolyCond::UpdateFrom(TGo4Condition * cond, Bool_t counts)
{
  if(!TGo4Condition::UpdateFrom(cond,counts)) return kFALSE;
  if(cond->InheritsFrom(TGo4PolyCond::Class()))
     {

#ifdef POLYCOND_UPDATE_WITHCLONE
       TCutG * temp = CloneCut((TGo4PolyCond*)cond);  // get clone from source, still valid there!
       CleanupSpecials(); // remove all references to cloned TCutG from list of specials
       //std::cout << "Update " << GetName() << " from " << temp << std::endl;
       if(temp != 0)
          {
             TCutG* old=fxCut; // JAM2016 change cut before deleting the old one!
             fxCut=temp;
             //std::cout << "Delete fxCut " << (long) old << std::endl;
             if(old != 0) delete old;
             delete fxCutHis;
             fxCutHis=0;
             //std::cout << "Update new fxCut " << (long) fxCut << std::endl;
             return kTRUE;
          }
       else
          {
             return kFALSE;
          }
#else
       /* JAM2016: try to avoid streaming the cut multiple times when updating condition:*/
       TGo4PolyCond* source=(TGo4PolyCond*)cond;
       TCutG * srccut = source->GetCut(false);
       //std::cout << "TGo4PolyCond::UpdateFrom without Clone of" << GetName() << ", srccut="<<(long )srccut<<", fxCut="<< (long)fxCut << std::endl;

       if(srccut==0) return kFALSE;
       CleanupSpecials(); // redundant? do it to get rid of entries from streamer!?
       Int_t pn = srccut->GetN();
       fxCut->Set(pn);
       Double_t xp=0;
       Double_t yp=0;
       for(Int_t i=0; i<pn; ++i) {
         srccut->GetPoint(i,xp,yp);
         fxCut->SetPoint(i,xp,yp);
       }

       // still need this to reassign the bins of statistics histogram:
       delete fxCutHis;
       fxCutHis=0;

      return kTRUE;
#endif

    }
 else
   {
      std::cout << "Cannot update " << GetName() << " from " << cond->ClassName() << std::endl;
      return kFALSE;
   }
}


Bool_t TGo4PolyCond::UpdateFromUrl(const char* rest_url_opt){
  if(!TGo4Condition::UpdateFromUrl(rest_url_opt)) return kFALSE;
  TString message;
  message.Form("TGo4PolyCond::UpdateFromUrl - condition %s: with url:%s", GetName(), rest_url_opt);
  TGo4Log::Message(1,message.Data());

  // evaluate options that change array of points
  if (UrlOptionHasKey(TGo4PolyCond::fgxURL_NPOINTS))
   {
     Int_t npoints = GetUrlOptionAsInt(TGo4PolyCond::fgxURL_NPOINTS, -1);
     if(npoints>=0)
       {
       TString xname,yname;
        Double_t* X = new Double_t[npoints];
        Double_t* Y = new Double_t[npoints];
        for(Int_t i=0; i<npoints;++i) {
             xname.Form("%s%d",TGo4PolyCond::fgxURL_XPRE.Data(), i);
             yname.Form("%s%d",TGo4PolyCond::fgxURL_YPRE.Data(), i);
             X[i]=GetUrlOptionAsDouble(xname.Data(),0);
             Y[i]=GetUrlOptionAsDouble(yname.Data(),0);
             message.Form(" i:%d, X=%f, Y=%f\n",i,X[i],Y[i]);
           }
        SetValues(X, Y, npoints);
        delete[] X; delete[] Y;
       }
     message.Form(" - setting Polygon condition to new values!");
     TGo4Log::Message(1,message.Data());

   }
  return kTRUE;
}



Double_t TGo4PolyCond::GetIntegral(TH1* histo, Option_t* opt)
{
//// root >4.00/08 only:
#if ROOT_VERSION_CODE >= ROOT_VERSION(4,0,8)
   if(fxCut)
   #if ROOT_VERSION_CODE >= ROOT_VERSION(5,25,1)
      return (fxCut->IntegralHist(dynamic_cast<TH2*>(histo),opt));
   #else
      return (fxCut->Integral(dynamic_cast<TH2*>(histo),opt));
   #endif
   else
      return 0;
#else
   if(fxCutHis==0) fxCutHis=CreateCutHistogram(histo);
   if(fxCutHis!=0)
      return (fxCutHis->Integral(opt));
   else
      return-1;
#endif // 40008
}

Double_t TGo4PolyCond::GetMean(TH1* histo, Int_t axis)
{
   if(fxCutHis==0) fxCutHis=CreateCutHistogram(histo);
   if(fxCutHis!=0)
      return (fxCutHis->GetMean(axis));
   else
      return-1;
}
Double_t TGo4PolyCond::GetRMS(TH1* histo, Int_t axis)
{
   if(fxCutHis==0) fxCutHis=CreateCutHistogram(histo);
   if(fxCutHis!=0)
      return (fxCutHis->GetRMS(axis));
   else
      return -1;
}
Double_t TGo4PolyCond::GetSkewness(TH1* histo, Int_t axis)
{
   if(fxCutHis==0) fxCutHis=CreateCutHistogram(histo);
   if(fxCutHis!=0)
      return (fxCutHis->GetSkewness(axis));
   else
      return -1;
}
Double_t TGo4PolyCond::GetCurtosis(TH1* histo, Int_t axis)
{
   if(fxCutHis==0) fxCutHis=CreateCutHistogram(histo);
   if(fxCutHis!=0)
      return (fxCutHis->GetKurtosis(axis));
   else
      return -1;
}
Double_t TGo4PolyCond::GetXMax(TH1* histo)
{
   Double_t result=0;
   if(fxCutHis==0) fxCutHis=CreateCutHistogram(histo);
   if(fxCutHis!=0)
      {
          TAxis* xax=fxCutHis->GetXaxis();
          Int_t maxbin=fxCutHis->GetMaximumBin();
          Int_t xmaxbin=maxbin%(fxCutHis->GetNbinsX()+2);
          result=xax->GetBinCenter(xmaxbin);
      }
   else
      {
         result=-1;
      }
   return result;
}
Double_t TGo4PolyCond::GetYMax(TH1* histo)
{
   Double_t result=0;
   if(fxCutHis==0) fxCutHis=CreateCutHistogram(histo);
   if(fxCutHis!=0)
      {
         TAxis* yax=fxCutHis->GetYaxis();
         Int_t maxbin=fxCutHis->GetMaximumBin();
         Int_t maxybin=maxbin/(fxCutHis->GetNbinsX()+2);
         result=yax->GetBinCenter(maxybin);
      }
   else
      {
         result=-1;
      }
   return result;
}
Double_t TGo4PolyCond::GetCMax(TH1* histo)
{
   if(fxCutHis==0) fxCutHis=CreateCutHistogram(histo);
   if(fxCutHis!=0)
      return(fxCutHis->GetMaximum());
   else
      return -1;
}

void TGo4PolyCond::SetPainter(TGo4ConditionPainter* painter)
{
if(painter==0) return;
if(painter->InheritsFrom(TGo4PolyCondPainter::Class()))
   {
      if(fxPainter) delete fxPainter;
      fxPainter=painter;
      fxPainter->SetCondition(this);
   }
else
   {
      TGo4Log::Warn("Could not set painter of class %s for TGo4PolyCond %s",
         painter->ClassName(),GetName());
   }


}


TGo4ConditionPainter* TGo4PolyCond::CreatePainter()
{
   TGo4ConditionPainter* painter=new TGo4PolyCondPainter(GetName());
   painter->SetCondition(this);
   //std::cout<<"TGo4PolyCond::CreatePainter() creates new painter"<< (long) painter<< std::endl;
   return painter;
}

TH2* TGo4PolyCond::CreateCutHistogram(TH1* source)
{
   TH2* his=dynamic_cast<TH2*>(source);
   if(his==0) return 0;
   TH2* work= (TH2*) his->Clone();
   //std::cout<<"TGo4PolyCond::CreateCutHistogram creates new cut histogram"<< (long) work <<" from source:"<< (long)source<< std::endl;
   Int_t nx=work->GetNbinsX();
   Int_t ny=work->GetNbinsY();
   TAxis* xaxis = work->GetXaxis();
   TAxis* yaxis = work->GetYaxis();
   xaxis->SetRange(0,0); // expand work histogram to full range
   yaxis->SetRange(0,0);
   // set all bins outside fxCut to zero:
   for(Int_t i=0; i<nx;++i)
      {
       Double_t x = xaxis->GetBinCenter(i);
       for(Int_t j=0; j<ny;++j)
         {
            Double_t y = yaxis->GetBinCenter(j);
            if(fxCut && !(fxCut->IsInside(x,y)))
               work->SetBinContent(i,j,0);
         }
      }
   // prepare statistics:
   Stat_t s[11]={0}; // dimension is kNstat of TH1.cxx
   work->PutStats(s); // reset previous stats
   work->GetStats(s); // recalculate
   work->PutStats(s); // put back
   work->SetDirectory(0); // important for first draw from marker setup file!
   return work;
}

// ----------------------------------------------------------
void TGo4PolyCond::CleanupSpecials()
{
  //std::cout<<"TGo4PolyCond::CleanupSpecials()..."<< std::endl;
   TSeqCollection* specials=gROOT->GetListOfSpecials();
   TIter iter(specials);
   TObject* ob=0;
   while((ob = iter())!=0) {
     if(ob->InheritsFrom(TCutG::Class())) {
        specials->Remove(ob);
        //std::cout <<">>>>>>>>>> removed fxCut" << (long) ob<<" :" << ob->GetName() <<" from list of specials "<< std::endl;
     }
   }//while
}

Int_t TGo4PolyCond::GetMemorySize()
{
   Int_t size = sizeof(*this);
   if (GetName()!=0) size+=strlen(GetName());
   if (GetTitle()!=0) size+=strlen(GetTitle());
   if (fxCut!=0) {
      size += sizeof(*fxCut);
#if ROOT_VERSION_CODE > ROOT_VERSION(4,0,8)
      size += fxCut->GetMaxSize()*2*sizeof(Double_t);
#else
      size += fxCut->GetN()*2*sizeof(Double_t);
#endif
   }
   return size;
}

void TGo4PolyCond::SavePrimitive(std::ostream& out, Option_t* opt)
{
   static int cnt = 0;
   TString line, varname = MakeScript(out, Form("polycond%d", cnt++), opt);

   if ((fxCut==0) || (fxCut->GetN()==0))
      line.Form("   %s->SetValues(0, 0, 0);", varname.Data());
   else {
      TString xname = varname;
      xname.ReplaceAll("->At(","_sub");
      xname.ReplaceAll(")","");
      TString yname = xname + "_y";
      xname = xname + "_x";
      line.Form("   Double_t %s[%d], %s[%d];", xname.Data(), fxCut->GetN(), yname.Data(), fxCut->GetN());
      out << line << std::endl;
      for (Int_t n=0;n<fxCut->GetN();n++) {
         Double_t x,y;
         fxCut->GetPoint(n, x, y);
         line.Form("   %s[%d] = %f; %s[%d] = %f;", xname.Data(), n, x, yname.Data(), n, y);
         out << line << std::endl;
      }
      line.Form("   %s->SetValues(%s, %s, %d);", varname.Data(), xname.Data(), yname.Data(), fxCut->GetN());
   }

   out << line << std::endl;
}

//---------------------------------------------
// Go4 Tasca analysis
// Author: Hans G. Essel
//         H.Essel@gsi.de
// GSI, Experiment Electronics, Data Processing
//---------------------------------------------

#ifndef TascaEvent_H
#define TascaEvent_H
#include "TascaCheckEvent.h"


class TascaEvent : public TObject {
   public:
      TascaEvent();
      TascaEvent(const char* name);
      virtual ~TascaEvent();

      Int_t PrintEvent();

      UInt_t fiFileNumber;
      UInt_t fiEventNumber;
  	  UInt_t fiChainNumber;
      Bool_t fisTof;
      Bool_t fisChopper;
      Bool_t fisMacro;
      Bool_t fisMicro;
      Bool_t fisEvr;
      Bool_t fisFission;
      Bool_t fisAlpha;

      UInt_t fiSystemMysec;
      UInt_t fiTimeStamp;
      UInt_t fiGammaMysec;
      UInt_t fiDeltaSystemTime;
      UInt_t fiDeltaGammaTime;
      UInt_t fiDeltaTime;
      UInt_t fiGammaMulti;
      Float_t ffGammaMax;
      Float_t ffGammaSum;
     // index of maximum hit, if we had more than one hit
      Int_t fiStopXLhitI;
      Int_t fiStopXHhitI;
      Int_t fiStopYLhitI;
      Int_t fiStopYHhitI;
      Int_t fiBackHhitI;
      Int_t fiBackLhitI;
      Int_t fiVetoHhitI;
      Int_t fiVetoLhitI;
      // value of maximum hit, if we had more than one hit
      Float_t ffStopXLhitV;
      Float_t ffStopXHhitV;
      Float_t ffStopYLhitV;
      Float_t ffStopYHhitV;
      Float_t ffBackHhitV;
      Float_t ffBackLhitV;
      Float_t ffVetoHhitV;
      Float_t ffVetoLhitV;

   ClassDef(TascaEvent,1)
};

#endif

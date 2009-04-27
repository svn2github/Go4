#ifndef TascaCaliEVENT_H
#define TascaCaliEVENT_H

#include "TGo4EventElement.h"

class TascaCaliProc;
class TGo4FileSource;

class TascaCaliEvent : public TGo4EventElement {
   public:
      TascaCaliEvent();
      TascaCaliEvent(const char* name) ;
      virtual ~TascaCaliEvent() ;
      /**
       * Method called by the event owner (analysis step) to fill the
       * event element from the set event source. Event source can
       * be the source of the analysis step (if this is a raw event)
       * or the event processor (if this is a reduced event).
       */
      Int_t Fill();

      /**
       * Method called by the event owner (analysis step) to clear the
       * event element.
       */
      void Clear(Option_t *t=""){
	   memset((void*) &ffStopXL[0],0, sizeof(ffStopXL));
	   memset((void*) &ffStopXH[0],0, sizeof(ffStopXH));
	   memset((void*) &ffStopYL[0],0, sizeof(ffStopYL));
	   memset((void*) &ffStopYH[0],0, sizeof(ffStopYH));
	   memset((void*) &ffBackL[0], 0, sizeof(ffBackL));
	   memset((void*) &ffBackH[0], 0, sizeof(ffBackH));
	   memset((void*) &ffVetoL[0], 0, sizeof(ffVetoL));
	   memset((void*) &ffVetoH[0], 0, sizeof(ffVetoH));
      }

      Int_t Init();

      Float_t ffTimeStamp;
      Float_t ffStopXL[144];  // 3x 8x6
      Float_t ffStopXH[144];
      Float_t ffStopYL[96];  // 2x 8x6
      Float_t ffStopYH[96];
      Float_t ffBackH[64]; // 8x8
      Float_t ffBackL[64];
      Float_t ffVetoH[16]; // 8x6
      Float_t ffVetoL[16];
      Float_t ffGammaMysec[8];
      Float_t ffGammaKev[8];

   private:
      TascaCaliProc * fxTascaEP;  //! Don't put this to file
      TGo4FileSource * fxTascaFS;  //! Don't put this to file

   ClassDef(TascaCaliEvent,1)
};
#endif //TascaCaliEVENT_H



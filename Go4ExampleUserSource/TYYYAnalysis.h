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

#ifndef TYYYANALYSIS_H
#define TYYYANALYSIS_H

#include "TGo4Analysis.h"

class TH1D;
class TYYYRawEvent;
class TYYYUnpackEvent;
class TYYYParameter;

class TYYYAnalysis : public TGo4Analysis {
   public:
      TYYYAnalysis();
      TYYYAnalysis(int argc, char** argv);
      virtual ~TYYYAnalysis() ;
      virtual Int_t UserPreLoop();
      virtual Int_t UserEventFunc();
      virtual Int_t UserPostLoop();
   private:
      TYYYRawEvent    *fRawEvent;
      TYYYUnpackEvent *fUnpackEvent;
      TH1D            *fSize;
      TYYYParameter   *fPar;
      Int_t            fEvents;
      Int_t            fLastEvent;

   ClassDef(TYYYAnalysis,1)
};
#endif //TYYYANALYSIS_H




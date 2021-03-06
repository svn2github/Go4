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

#include "TGo4UserSourceParameter.h"

#include "TGo4Status.h"
#include "TGo4Log.h"
#include "Go4EventServerTypes.h"

TGo4UserSourceParameter::TGo4UserSourceParameter(const char* name, const char* expr, Int_t port)
: TGo4EventSourceParameter(name, GO4EV_USER), fiPort(port), fxExpression(expr)
{
}

TGo4UserSourceParameter::TGo4UserSourceParameter()
: TGo4EventSourceParameter("default user event source", GO4EV_USER), fiPort(0)
{
}

TGo4UserSourceParameter::~TGo4UserSourceParameter()
{
}

Int_t TGo4UserSourceParameter::PrintParameter(Text_t* buffer, Int_t buflen)
{
   GO4TRACE((12,"TGo4UserSourceParameter::PrintParameter()",__LINE__, __FILE__));
   Int_t locallen=128000;
   Text_t localbuf[128000];
   if(buflen<0 && buffer!=0)
      return 0;
   Int_t size=0;
   Text_t* current=localbuf;
   Int_t restlen=locallen;

   Int_t delta=TGo4EventSourceParameter::PrintParameter(current,restlen);
   restlen-=delta;
   current+=delta;
   current=TGo4Status::PrintIndent(current,restlen);
   current=TGo4Status::PrintBuffer(current,restlen, "  Expression: %s\n",GetExpression());
   if(buffer==0)
      {
      std::cout << localbuf << std::endl;
      }
   else
      {
         size=locallen-restlen;
         if(size>buflen-1)
            size=buflen-1;
         strncpy(buffer,localbuf,size);
      }
   return size;
}

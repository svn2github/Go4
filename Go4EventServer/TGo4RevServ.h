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

#ifndef TGO4REVSERV_H
#define TGO4REVSERV_H

#include "TGo4MbsSource.h"

class TGo4RevServParameter;

/**
 * Implements the gsi root remote event server into the
 * go4 classes. May be a simple wrapper for the mrevlib or liblea stuff.
 * @author J. Adamczewski
 * @since 1/2001
 */
class TGo4RevServ : public TGo4MbsSource {
  public:

    TGo4RevServ(const char* name);

    TGo4RevServ();

    TGo4RevServ(TGo4RevServParameter* par);

    ~TGo4RevServ();

    /** Open the file or connection. */
    virtual Int_t Open();

    void SetPort(Int_t port) { fiPort=port; }

    /** default remote eventserver portnumber */
    static const Int_t fgiDEFAULTPORT;

  private:
    /** Port number for remote event server only.  */
    Int_t fiPort;

  ClassDef(TGo4RevServ,2)
};

#endif //TGO4REVSERV_H

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

#ifndef TGO4SERVERPROXY_H
#define TGO4SERVERPROXY_H

#include "TGo4Proxy.h"
#include "TString.h"

/** special base class for remote data servers like DABC, HTTP, hist server */

class TGo4ServerProxy : public TGo4Proxy {
   protected:
      TGo4Slot*        fxParentSlot;            //!
      Bool_t           fbAnalysisSettingsReady; // true when settings are specified

      TString         fNodeName; // name of remote node
      TString         fInfoStr;
   public:
      TGo4ServerProxy();
      virtual ~TGo4ServerProxy();

      virtual void Initialize(TGo4Slot* slot) { fxParentSlot = slot; }

      TGo4Slot* ParentSlot() { return fxParentSlot; }
      TGo4Slot* SettingsSlot();
      TGo4Slot* RatemeterSlot();

      virtual const char* GetServerName() const { return fNodeName.Data(); }
      virtual const char* GetUserName() const { return ""; }
      virtual const char* GetContainedObjectInfo();
      virtual Bool_t IsGo4Analysis() const { return kFALSE; }

      virtual Bool_t IsConnected() { return kFALSE; }
      virtual Bool_t IsViewer()  { return kFALSE; }
      virtual Bool_t IsController()  { return kFALSE; }
      virtual Bool_t IsAdministrator()  { return kFALSE; }

      virtual Bool_t NamesListReceived() { return kFALSE; }
      virtual Bool_t RefreshNamesList() { return kFALSE; }
      virtual Bool_t DelayedRefreshNamesList(Int_t delay_sec) { return kFALSE; }

      void SetAnalysisSettingsReady(Bool_t on = kTRUE) { fbAnalysisSettingsReady = on ; }
      Bool_t IsAnalysisSettingsReady() const { return fbAnalysisSettingsReady; }

      virtual Bool_t CanSubmitObjects() { return kFALSE; }
      virtual void RequestAnalysisSettings() {}
      virtual void SubmitAnalysisSettings() {}
      virtual void CloseAnalysisSettings() {}

      virtual Bool_t IsAnalysisRunning() const { return kFALSE; }
      virtual void StartAnalysis() {}
      virtual void StopAnalysis() {}

      virtual Bool_t RequestObjectStatus(const char* objectname, TGo4Slot* tgtslot) { return kFALSE; }

      virtual Bool_t UpdateAnalysisObject(const char* objectname, TObject* obj) { return kFALSE; }

      virtual void ClearAllAnalysisObjects() {}
      virtual void ClearAnalysisObject(const char* fullpath) {}

      virtual void RemoteTreeDraw(const char* treename,
                                  const char* varexp,
                                  const char* cutcond,
                                  const char* hname) {}

      virtual void RequestEventStatus(const char* evname, Bool_t astree, TGo4Slot* tgtslot) {}

      virtual void RemotePrintEvent(const char* evname,
                                    Int_t evnumber,
                                    Int_t subid,
                                    Bool_t ishex,
                                    Bool_t islong) {}

      virtual void ChageObjectProtection(const char* fullpath, const char* flags) {}

      virtual void RemoveObjectFromAnalysis(const char* fullpath) {}

      virtual void ExecuteLine(const char* line) {}

      virtual void PrintDynListEntry(const char* fullpath) {}

      virtual void LoadConfigFile(const char* fname) {}
      virtual void SaveConfigFile(const char* fname) {}

      virtual void WriteAutoSave(const char* fname,
                                 Int_t complevel,
                                 Bool_t overwrite) {}

      virtual Int_t NumberOfWaitingProxyes() { return 0; }

   ClassDef(TGo4ServerProxy, 1);
};


#endif
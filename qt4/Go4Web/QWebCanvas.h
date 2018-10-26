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

#ifndef QWEBCANVAS_H
#define QWEBCANVAS_H

#include "rootwebview.h"

class TCanvas;
class TPad;
class TObject;
class QTimer;

class QWebCanvas : public QWidget {

   Q_OBJECT

public:
   QWebCanvas(QWidget *parent = 0);
   virtual ~QWebCanvas();

   /// returns canvas shown in the widget
   TCanvas *getCanvas() { return fCanvas; }

   bool isStatusBarVisible();

   void Modified();
   void Update();

signals:

   void CanvasDropEvent(QDropEvent*, TPad*);

public slots:

   void actiavteEditor(TPad *pad = 0, TObject *obj = 0);
   void activateStatusLine();

   void setStatusBarVisible(bool flag);

protected slots:

   void dropView(QDropEvent* event);

   void processRepaintTimer();

protected:

   virtual void resizeEvent(QResizeEvent *event);

   virtual void dropEvent(QDropEvent* event);

   RootWebView *fView;  ///< qt webwidget to show

   TCanvas *fCanvas;

   QTimer *fRepaintTimer;
};

#endif

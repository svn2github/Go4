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

#include "QGo4LineEdit.h"

#include "qdragobject.h"

QGo4LineEdit::QGo4LineEdit(QWidget* parent, const char* name) :
   QLineEdit(parent, name)
{
   setAcceptDrops( TRUE );
}

QGo4LineEdit::~QGo4LineEdit()
{
}

void QGo4LineEdit::dragEnterEvent(QDragEnterEvent* event)
{
   QString str;
   if (!QTextDrag::decode(event,str)) event->ignore();
}

void QGo4LineEdit::dropEvent(QDropEvent* event)
{
   QString str;
   if (!QTextDrag::decode(event, str)) return;
   setText(str);
   emit textDropped();

//   clear();
//   insert(str);
//   e->acceptAction();
}

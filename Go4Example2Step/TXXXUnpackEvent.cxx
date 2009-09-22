#include "TXXXUnpackEvent.h"


//***********************************************************
TXXXUnpackEvent::TXXXUnpackEvent() :
   TGo4EventElement()
{
}
//***********************************************************
TXXXUnpackEvent::TXXXUnpackEvent(const char* name) :
   TGo4EventElement(name)
{
}
//***********************************************************
TXXXUnpackEvent::~TXXXUnpackEvent()
{
}
//***********************************************************
void  TXXXUnpackEvent::Clear(Option_t *t)
{
   void* destfield;
   destfield = (void*) &fiCrate1[0];
   memset(destfield, 0, sizeof(fiCrate1));
   destfield = (void*) &fiCrate2[0];
   memset(destfield, 0, sizeof(fiCrate2));
   destfield = (void*) &fiCrate3[0];
   memset(destfield, 0, sizeof(fiCrate3));
   destfield = (void*) &fiCrate4[0];
   memset(destfield, 0, sizeof(fiCrate4));
}

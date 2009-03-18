#include "TGo4StartClient.h"
#include <QDir>
#include <QFileInfo>
#include <Q3FileDialog>
#include "TGo4QSettings.h"

TGo4StartClient::TGo4StartClient( QWidget* parent, const char* name, bool modal, Qt::WFlags fl )
    : QDialog( parent, name, modal, fl )
{ 
	//setObjectName(name);
	setupUi(this);
			// put slot connections here!
			// note: Qt4 uic will add all existing connections 
			// from ui file to the setupUI
   LineEditClientName->setText(go4sett->getClientName());
   LineEditClientNode->setText(go4sett->getClientNode());
   LineEditClientDir->setText(go4sett->getClientDir());
   LineEditClientExec->setText(go4sett->getClientExec());
   ClientShellGroup->setButton(go4sett->getClientShellMode());
   ClientTermGroup->setButton(go4sett->getClientTermMode());

   bool isserver = go4sett->getClientIsServer();
   ServerModeCombo->setCurrentItem(isserver ? 1 : 0);
   qt_selected->setEnabled(!isserver);
   if (isserver && ClientTermGroup->selectedId()==1)
      ClientTermGroup->setButton(2);
}

void TGo4StartClient::getResults()
{
   go4sett->setClientName(LineEditClientName->text());
   go4sett->setClientNode(LineEditClientNode->text());
   go4sett->setClientDir(LineEditClientDir->text());
   go4sett->setClientExec(LineEditClientExec->text());
   go4sett->setClientShellMode(ClientShellGroup->selectedId());
   go4sett->setClientTermMode(ClientTermGroup->selectedId());
   go4sett->setClientIsServer(ServerModeCombo->currentItem()==1);
}

void TGo4StartClient::SelectDir()
{
   Q3FileDialog fd(this, "file name", TRUE);
   fd.setMode(Q3FileDialog::DirectoryOnly);
   fd.setName("Select your working dir");
   fd.setCaption("Select your working dir");

   if (fd.exec() != QDialog::Accepted ) return;

   QString fileName = fd.selectedFile();
   LineEditClientDir->setText(fileName);
   QDir::setCurrent(fileName);
}

void TGo4StartClient::SelectProg()
{
   Q3FileDialog fd(this, "file name", TRUE);
   fd.setMode(Q3FileDialog::ExistingFile);
   fd.setName("Select your analysis program");
   fd.setCaption("Select your analysis program");

   if (fd.exec() != QDialog::Accepted) return;

   QFileInfo fi(fd.selectedFile());
   LineEditClientExec->setText(fi.fileName());
   LineEditClientDir->setText(fi.dirPath(TRUE));
   QDir::setCurrent(fi.dirPath(TRUE));
}


void TGo4StartClient::ServerModeCombo_activated(int id)
{
   bool isserver = (id==1);

   qt_selected->setEnabled(!isserver);

   if (isserver) {
     if (ClientTermGroup->selectedId()==1)
       ClientTermGroup->setButton(2);
   }
}

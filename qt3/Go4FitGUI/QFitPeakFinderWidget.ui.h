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

/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/

TGo4FitPeakFinder * QFitPeakFinderWidget::GetPF() {
  return dynamic_cast<TGo4FitPeakFinder*> (GetObject());
}

void QFitPeakFinderWidget::FillSpecificData() {
   QFitNamedWidget::FillSpecificData();
   TGo4FitPeakFinder* finder = GetPF();
   if (finder) {
      DataNameEdt->setText(finder->GetDataName());
      ClearModelsChk->setChecked(finder->GetClearModels());

      UsePolynChk->setChecked(finder->GetUsePolynom());
      PolynSpin->setValue(finder->GetPolynomOrder());
      PolynSpin->setEnabled(finder->GetUsePolynom());

      FindersTab->setCurrentPage(finder->GetPeakFinderType());

      ThresholdEdt->setText(QString::number(finder->Get0MaxAmplFactor()));
      MinWidthEdt->setText(QString::number(finder->Get0MinWidth()));
      MaxWidthEdt->setText(QString::number(finder->Get0MaxWidth()));

      WidthEdit->setText(QString::number(finder->Get1LineWidth()));

      NoiseFactorEdit->setText(QString::number(finder->Get2NoiseFactor()));
      NoiseMinEdit->setText(QString::number(finder->Get2NoiseMinimum()));
      SumUpSpin->setValue(finder->Get2ChannelSum());
   }
}


void QFitPeakFinderWidget::DataNameEdt_textChanged( const QString & name)
{
  if (!fbFillWidget && GetPF())
    GetPF()->SetDataName(name);
}


void QFitPeakFinderWidget::ClearModelsChk_toggled( bool zn)
{
  if (!fbFillWidget && GetPF())
    GetPF()->SetClearModels(zn);
}


void QFitPeakFinderWidget::UsePolynChk_toggled( bool zn)
{
  if (!fbFillWidget && GetPF()) {
    GetPF()->SetUsePolynom(zn);
    PolynSpin->setEnabled(zn);
  }
}


void QFitPeakFinderWidget::PolynSpin_valueChanged( int zn)
{
  if (!fbFillWidget && GetPF())
    GetPF()->SetPolynomOrder(zn);
}


void QFitPeakFinderWidget::FindersTab_currentChanged( QWidget * w )
{
  if (!fbFillWidget && GetPF())
    GetPF()->SetPeakFinderType(FindersTab->indexOf(w));
}


void QFitPeakFinderWidget::ThresholdEdt_textChanged( const QString & value)
{
  if (!fbFillWidget && GetPF()) {
    bool ok = FALSE;
    double zn = value.toDouble(&ok);
    if (ok && (zn>0) && (zn<=1.)) GetPF()->Set0MaxAmplFactor(zn);
  }
}


void QFitPeakFinderWidget::MinWidthEdt_textChanged( const QString & value)
{
  if (!fbFillWidget && GetPF()) {
    bool ok = FALSE;
    double zn = value.toDouble(&ok);
    if (ok && (zn>=0)) GetPF()->Set0MinWidth(zn);
  }
}


void QFitPeakFinderWidget::MaxWidthEdt_textChanged( const QString & value)
{
  if (!fbFillWidget && GetPF()) {
    bool ok = FALSE;
    double zn = value.toDouble(&ok);
    if (ok && (zn>0)) GetPF()->Set0MaxWidth(zn);
  }
}


void QFitPeakFinderWidget::WidthEdit_textChanged( const QString & value )
{
  if (!fbFillWidget && GetPF()) {
    bool ok = FALSE;
    double zn = value.toDouble(&ok);
    if (ok && (zn>0)) GetPF()->Set1LineWidth(zn);
  }
}


void QFitPeakFinderWidget::NoiseFactorEdit_textChanged( const QString & value )
{
  if (!fbFillWidget && GetPF()) {
    bool ok = FALSE;
    double zn = value.toDouble(&ok);
    if (ok && (zn>0)) GetPF()->Set2NoiseFactor(zn);
  }
}


void QFitPeakFinderWidget::NoiseMinEdit_textChanged( const QString & value)
{
  if (!fbFillWidget && GetPF()) {
    bool ok = FALSE;
    double zn = value.toDouble(&ok);
    if (ok && (zn>=0)) GetPF()->Set2NoiseMinimum(zn);
  }
}


void QFitPeakFinderWidget::SumUpSpin_valueChanged( int zn)
{
  if (!fbFillWidget && GetPF())
    GetPF()->Set2ChannelSum(zn);
}

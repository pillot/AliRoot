/**************************************************************************
 * Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 *                                                                        *
 * Author: The ALICE Off-line Project.                                    *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/

/*
$Log$
Revision 1.4  2006/04/05 08:35:38  hristov
Coding conventions (S.Arcelli, C.Zampolli)

Revision 1.3  2006/03/28 14:58:08  arcelli
updates to handle new V5 geometry & some re-arrangements

Revision 1.2  2006/02/13 17:22:26  arcelli
just Fixing Log info

Revision 1.1  2006/02/13 16:10:48  arcelli
Add classes for TOF Calibration (C.Zampolli)

author: Chiara Zampolli, zampolli@bo.infn.it
*/  

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// class for TOF calibration : strips                                        //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "TObject.h"
#include "TROOT.h"
#include "TBrowser.h"
#include "TClass.h"
#include "AliLog.h"
#include "AliTOFGeometryV5.h"
#include "AliTOFChannel.h"
#include "AliTOFCalStrip.h"
#include "AliTOFCalPadZ.h"

ClassImp(AliTOFCalStrip)

//________________________________________________________________

AliTOFCalStrip::AliTOFCalStrip(){
  //main ctor
  fCh = 0;
  fGeom= 0x0; 
  fNpadZ = 0;
  fNpadX = 0;
}
//________________________________________________________________

AliTOFCalStrip::AliTOFCalStrip(AliTOFChannel *ch):
  fCh(ch)
{
  // ctor with channel
  fGeom= 0x0; 
  fNpadZ = 0;
  fNpadX = 0;
}
//________________________________________________________________

AliTOFCalStrip::AliTOFCalStrip(AliTOFGeometry *geom){
  //ctor with geom
  fCh = 0;
  fGeom = geom;
  fNpadZ = fGeom->NpadZ();
  fNpadX = fGeom->NpadX();
}
//________________________________________________________________

AliTOFCalStrip::AliTOFCalStrip(AliTOFGeometry *geom,AliTOFChannel *ch):
  fCh(ch)
{
  //ctor with channel and geom
  fGeom = geom;
  fNpadZ = fGeom->NpadZ();
  fNpadX = fGeom->NpadX();
}
//________________________________________________________________

AliTOFCalStrip::~AliTOFCalStrip()
{
  //dtor
  delete[] fCh;
}

//________________________________________________________________

AliTOFCalStrip::AliTOFCalStrip(const AliTOFCalStrip& strip):
  TObject(strip)
  {
    //copy ctor
    fCh = strip.fCh;
    fNpadZ = strip.fNpadZ;
    fNpadX = strip.fNpadX;

  }
//________________________________________________________________

AliTOFCalStrip& AliTOFCalStrip::operator=(const AliTOFCalStrip& strip)
  {
    //assignment operator
    this->fCh = strip.fCh;
    this->fNpadZ = strip.fNpadZ;
    this->fNpadX = strip.fNpadX;
    return *this;

  }
//________________________________________________________________

void AliTOFCalStrip::Browse(TBrowser *b){
  //add obj to list of browsables
  if(fGeom==0x0){
    AliTOFGeometry *geom = new AliTOFGeometryV5();
    AliInfo("V5 TOF Geometry is taken as the default");
    fNpadZ = geom->NpadZ();
    fNpadX = geom->NpadX();
    delete geom;
  }
  char name[10];
  for(Int_t i=0; i<fNpadZ; ++i) {
    snprintf(name,sizeof(name),"PadZ %2.2d",i);
    b->Add(new AliTOFCalPadZ(&fCh[i*fNpadX]),name);
  }
}

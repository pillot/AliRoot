/* Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice                               */

// $Id$
// $MpId: AliMpSlatSegmentation.h,v 1.4 2005/09/19 19:01:09 ivana Exp $

/// \ingroup slat
/// \class AliMpSlatSegmentation
/// \brief Implementation of AliMpVSegmentation for St345 slats.
/// 
/// Author: Laurent Aphecetche

#ifndef ALI_MP_SLAT_SEGMENTATION_H
#define ALI_MP_SLAT_SEGMENTATION_H

#ifndef ROOT_TString
#include "TString.h"
#endif

#ifndef ALI_MP_V_SEGMENTATION_H
#include "AliMpVSegmentation.h"
#endif

#ifndef ALI_MP_PAD_H
#include "AliMpPad.h"
#endif

class AliMpMotifPosition;
class AliMpPCB;
class AliMpSlat;

class AliMpSlatSegmentation : public AliMpVSegmentation
{
 public:
  AliMpSlatSegmentation();
  AliMpSlatSegmentation(const AliMpSlat* slat);
  virtual ~AliMpSlatSegmentation();

  virtual AliMpVPadIterator* CreateIterator(const AliMpArea& area) const;

  virtual AliMpPad PadByLocation(const AliMpIntPair& location, 
			 Bool_t warning) const;

  virtual AliMpPad PadByIndices(const AliMpIntPair& indices,  
			Bool_t warning) const;

  virtual AliMpPad PadByPosition(const TVector2& position,
			 Bool_t warning) const;

  Int_t MaxPadIndexX();
  Int_t MaxPadIndexY();

  Bool_t HasPad(const AliMpIntPair& indices) const;
   
  const AliMpSlat* Slat() const;
  
 private:
  const AliMpSlat* fkSlat; // Slat

  ClassDef(AliMpSlatSegmentation,1) // A slat for stations 3,4,5
};

#endif

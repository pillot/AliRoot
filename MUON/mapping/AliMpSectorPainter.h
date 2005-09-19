/* Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice                               */

// $Id$
// $MpId: AliMpSectorPainter.h,v 1.6 2005/08/26 15:43:36 ivana Exp $

/// \ingroup graphics
/// \class AliMpSectorPainter
/// \brief Class for drawing a sector into canvas
//
/// Authors: David Guez, IPN Orsay

#ifndef ALI_MP_SECTOR_PAINTER_H
#define ALI_MP_SECTOR_PAINTER_H

#include "AliMpVPainter.h"

class AliMpSector;

class AliMpSectorPainter : public AliMpVPainter
{
 public:
  AliMpSectorPainter();
  AliMpSectorPainter(AliMpSector *sector);
  virtual ~AliMpSectorPainter();
  
  virtual void Draw(Option_t* option);
  virtual void Paint(Option_t* /*option*/);
  virtual void DumpObject(); // -MENU-
  virtual TVector2 GetPosition() const;
  virtual TVector2 GetDimensions() const;

 protected:
  AliMpSectorPainter(const AliMpSectorPainter& right);
  AliMpSectorPainter&  operator = (const AliMpSectorPainter& right);

 private:
  AliMpSector *fSector;          // the sector to draw
  ClassDef(AliMpSectorPainter,1) // Sector painter
};
#endif //ALI_MP_SECTOR_PAINTER_H

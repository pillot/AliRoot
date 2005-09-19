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

// $Id$
// $MpId: AliMpSlatSegmentation.cxx,v 1.4 2005/09/19 19:01:31 ivana Exp $

#include "AliMpSlatSegmentation.h"

#include "AliLog.h"
#include "AliMpConnection.h"
#include "AliMpMotif.h"
#include "AliMpMotifPosition.h"
#include "AliMpMotifType.h"
#include "AliMpPCB.h"
#include "AliMpSlat.h"
#include "AliMpSlatPadIterator.h"

ClassImp(AliMpSlatSegmentation)

//_____________________________________________________________________________
AliMpSlatSegmentation::AliMpSlatSegmentation() 
: AliMpVSegmentation(),
fkSlat(0)
{
  //
  // Default ctor. Not to be used really.
  //
  AliDebug(1,Form("this=%p Empty ctor",this));
}

//_____________________________________________________________________________
AliMpSlatSegmentation::AliMpSlatSegmentation(const AliMpSlat* slat) 
: AliMpVSegmentation(), 
fkSlat(slat)
{
  //
  // Normal ctor.
  //
  AliDebug(1,Form("this=%p Normal ctor slat=%p",this,slat));
}

//_____________________________________________________________________________
AliMpSlatSegmentation::~AliMpSlatSegmentation()
{
  //
  // Dtor (empty).
  //
  AliDebug(1,Form("this=%p",this));			
}

//_____________________________________________________________________________
AliMpVPadIterator*
AliMpSlatSegmentation::CreateIterator(const AliMpArea& area) const
{
  //
  // Returns an iterator to loop over the pad contained within given area.
  //
  
  return new AliMpSlatPadIterator(fkSlat,area);
}

//_____________________________________________________________________________
Bool_t
AliMpSlatSegmentation::HasPad(const AliMpIntPair& indices) const
{
  //
  // Test if this slat has a pad located at the position referenced
  // by the integer indices.
  //
  
  return PadByIndices(indices,kFALSE) != AliMpPad::Invalid();
}

//_____________________________________________________________________________
Int_t 
AliMpSlatSegmentation::MaxPadIndexX()
{
  //
  // Returns the value of the largest pad index in x-direction.
  //
  
  return fkSlat->GetNofPadsX()-1;
}

//_____________________________________________________________________________
Int_t 
AliMpSlatSegmentation::MaxPadIndexY()
{
  //
  // Returns the value of the largest pad index in y-direction.
  //
  
  return fkSlat->GetMaxNofPadsY()-1;
}

//_____________________________________________________________________________
AliMpPad
AliMpSlatSegmentation::PadByLocation(const AliMpIntPair& location, 
                                     Bool_t warning) const
{
  //
  // Returns the pad specified by its location, where location is the 
  // pair (ManuID,ManuChannel).
  // If warning=kTRUE and the pad does not exist, a warning message is 
  // printed.
  //
  // AliMpPad::Invalid() is returned if there's no pad at the given location.
  //
  Int_t manuID = location.GetFirst();
	
  AliMpMotifPosition* motifPos = fkSlat->FindMotifPosition(manuID);
	
  if (!motifPos)
	{
		if (warning)
		{
			AliWarning(Form("Manu ID %d not found in slat %s",
			                 manuID, fkSlat->GetID()));
			return AliMpPad::Invalid();
		}
	}
  AliMpVMotif* motif = motifPos->GetMotif();
  AliMpIntPair localIndices = 
    motif->GetMotifType()->FindLocalIndicesByGassiNum(location.GetSecond());
	
  if (!localIndices.IsValid()) 
	{
		if (warning) 
		{
			AliWarning(Form("The pad number %d doesn't exists",
			                location.GetSecond()));
		}
		return AliMpPad::Invalid();
	}
	
  return AliMpPad(location,
                  motifPos->GlobalIndices(localIndices),
		  motifPos->Position() 
		    + motif->PadPositionLocal(localIndices) 
		    - TVector2(fkSlat->DX(),fkSlat->DY()),
		  motif->GetPadDimensions(localIndices));  
}

//_____________________________________________________________________________
AliMpPad
AliMpSlatSegmentation::PadByIndices(const AliMpIntPair& indices, 
                                    Bool_t warning) const
{
  //
  // Returns the pad specified by its integer indices.
  // If warning=kTRUE and the pad does not exist, a warning message is 
  // printed.
  //
  // AliMpPad::Invalid() is returned if there's no pad at the given location.
  //
  //  
  // FIXME: except for the FindMotifPosition below, this method
  // is exactly as the one in AliMpSectorSegmentation.
  // See if we can merge them somehow.
	
  AliMpMotifPosition* motifPos = fkSlat->FindMotifPosition(indices.GetFirst(),
																									 indices.GetSecond());
  if (!motifPos)
	{
		if ( warning ) 
		{
			AliWarning(Form("No motif found containing pad location (%d,%d)",
			                 indices.GetFirst(),indices.GetSecond()));	  
		}
		return AliMpPad::Invalid();
	}
	
  AliMpVMotif* motif = motifPos->GetMotif();
  AliMpMotifType* motifType = motif->GetMotifType();
  AliMpIntPair localIndices(indices-motifPos->GetLowIndicesLimit());
  AliMpConnection* connection = 
    motifType->FindConnectionByLocalIndices(localIndices);
  
  if (!connection)
	{
		if ( warning )
		{
			AliWarning(Form("No connection for pad location (%d,%d)",
			                indices.GetFirst(),indices.GetSecond()));
			return AliMpPad::Invalid();
		}      
	}
	
  return AliMpPad(AliMpIntPair(motifPos->GetID(),connection->GetGassiNum()),
                  indices,
		  motifPos->Position()
		    + motif->PadPositionLocal(localIndices)
		    - TVector2(fkSlat->DX(), fkSlat->DY()),
		  motif->GetPadDimensions(0));
}

//_____________________________________________________________________________
AliMpPad
AliMpSlatSegmentation::PadByPosition(const TVector2& position, 
                                     Bool_t warning) const
{
  //
  // Returns the pad specified by its (floating point) position.
  // If warning=kTRUE and the pad does not exist, a warning message is 
  // printed.
  //
  // AliMpPad::Invalid() is returned if there's no pad at the given location.
  //
  
  AliMpMotifPosition* motifPos = fkSlat->FindMotifPosition(position.X(),position.Y());
	
  if (!motifPos)
	{
		if (warning) 
		{
			AliWarning(Form("Position (%e,%e) mm outside limits",
			           position.X(),position.Y()));
		}
		return AliMpPad::Invalid();
	}
	
  AliMpVMotif* motif =  motifPos->GetMotif();  
  AliMpIntPair localIndices 
    = motif->PadIndicesLocal(position-motifPos->Position());
	
  AliMpConnection* connect = 
    motif->GetMotifType()->FindConnectionByLocalIndices(localIndices);
	
  if (!connect)
	{
		if (warning) 
		{
			AliWarning("Position outside motif limits");
		}
		return AliMpPad::Invalid();
	}
  
  return AliMpPad(AliMpIntPair(motifPos->GetID(),connect->GetGassiNum()),
                  motifPos->GlobalIndices(localIndices),
		  motifPos->Position()
		    + motif->PadPositionLocal(localIndices)
		    - TVector2(fkSlat->DX(),fkSlat->DY()),
		  motif->GetPadDimensions(localIndices));  
}

//_____________________________________________________________________________
const AliMpSlat* 
AliMpSlatSegmentation::Slat() const
{
  //
  // Returns the pointer to the referenced slat.
  //
  
  return fkSlat;
}

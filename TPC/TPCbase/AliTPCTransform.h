#ifndef ALITPCTRANSFORM_H
#define ALITPCTRANSFORM_H

/* Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice                               */


/// \class AliTPCTransform
/// \brief Class for tranformation of the coordinate frame
///
/// Transformation
///  local coordinate frame (sector, padrow, pad, timebine) ==>
///  rotated global (tracking) cooridnate frame (sector, lx,ly,lz)

class AliTPCRecoParam;
class AliTPCChebCorr;
#include "AliTransform.h"

class AliTPCTransform:public AliTransform {
public:
  AliTPCTransform();
  AliTPCTransform(const AliTPCTransform& transform);

  virtual ~AliTPCTransform();
  virtual void Transform(Double_t *x,Int_t *i,UInt_t time,
			 Int_t coordinateType);
  void SetPrimVertex(Double_t *vtx);
  void Local2RotatedGlobal(Int_t sec,  Double_t *x) const;
  void RotatedGlobal2Global(Int_t sector,Double_t *x) const;
  void Global2RotatedGlobal(Int_t sector,Double_t *x) const;
  void GetCosAndSin(Int_t sector,Double_t &cos,Double_t &sin) const;
  UInt_t GetCurrentTimeStamp() const { return fCurrentTimeStamp;}
  const AliTPCRecoParam * GetCurrentRecoParam() const {return fCurrentRecoParam;}
  AliTPCRecoParam * GetCurrentRecoParamNonConst() const {return fCurrentRecoParam;}
  UInt_t GetCurrentRunNumber() const { return fCurrentRun;}
  const AliTPCChebCorr* GetCorrMapCache0() const {return fCorrMapCache0;}
  const AliTPCChebCorr* GetCorrMapCache1() const {return fCorrMapCache1;}
  //
  // set current values
  //
  void SetCurrentRecoParam(AliTPCRecoParam* param){fCurrentRecoParam=param;}
  void SetCurrentRun(Int_t run){fCurrentRun=run;}
  void SetCurrentTimeStamp(Int_t timeStamp);
  void ApplyTransformations(Double_t *xyz, Int_t volID);
  //
  void UpdateTimeDependentCache();
private:
  AliTPCTransform& operator=(const AliTPCTransform&); // not implemented
  Double_t fCoss[18];  ///< cache the transformation
  Double_t fSins[18];  ///< cache the transformation
  Double_t fPrimVtx[3];///< position of the primary vertex - needed for TOF correction
  AliTPCRecoParam * fCurrentRecoParam; //!<! current reconstruction parameters
  const AliTPCChebCorr* fCorrMapCache0;      //!<! current correction map0 (for 1st time bin if time-dependent)
  const AliTPCChebCorr* fCorrMapCache1;      //!<! current correction map1 (for 2nd time bin if time-dependent)
  Int_t    fCurrentRun;                //!<! current run
  UInt_t   fCurrentTimeStamp;          //!<! current time stamp
  /// \cond CLASSIMP
  ClassDef(AliTPCTransform,2)
  /// \endcond
};

#endif

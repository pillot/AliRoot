//-*- Mode: C++ -*-
// $Id$

#ifndef ALIHLTTTREEPROCESSOR_H
#define ALIHLTTTREEPROCESSOR_H
//* This file is property of and copyright by the ALICE HLT Project        * 
//* ALICE Experiment at CERN, All rights reserved.                         *
//* See cxx source for full Copyright notice                               *

/// @file   AliHLTTTreeProcessor.h
/// @author Timur Pocheptsov, Matthias Richter
/// @date   05.07.2010
/// @brief  Generic component for data collection in a TTree

#include <list>

#include <TString.h>

#include "AliHLTProcessor.h"

class TTree;
class TH1;
class TStopwatch;

/**
 * @class AliHLTTTreeProcessor
 * Generic component for data collection in a TTree, or as a special case
 * in a TNtuple (which is a tree with only float branches). Child components
 * implement the creation and filling of the tree, which is dependent on the
 * data itself.
 *
 * Child components have to implement the basic component property methods
 * like GetComponentID(), GetInputDataTypes(), and Spawn(). Default
 * implementations of GetOutputDataSize() and GetOutputDataType() are already
 * provided by the base class.
 * 
 * The base class keeps a circular TTree of a specific event count. Histograms
 * are periodically generated by applying a table of selections and cuts. The
 * table can be configured and changed at run-time and the data sample in the
 * tree can be reset.
 *
 * @ingroup alihlt_base
 */
class AliHLTTTreeProcessor : public AliHLTProcessor {
private:
  enum EDefaults {
    kMaxEntries = 1000,
    kDefaultNBins = 200,
    kInterval = 5
  };
public:
  /// default constructor
  AliHLTTTreeProcessor();
  /// destructor
  virtual ~AliHLTTTreeProcessor();

  /// inherited from AliHLTComponent, get the output data type
  virtual AliHLTComponentDataType GetOutputDataType();

  /// inherited from AliHLTComponent, get the output data size estimator
  virtual void GetOutputDataSize(unsigned long& constBase, double& inputMultiplier);

protected:
  /// initialization, overloaded from AliHLTComponent
  int DoInit(int argc, const char** argv);
  /// deinitialization, overloaded from AliHLTComponent
  int DoDeinit();
  /// inherited from AliHLTProcessor, data processing
  int DoEvent(const AliHLTComponentEventData& evtData,
              AliHLTComponentTriggerData& trigData);
  using AliHLTProcessor::DoEvent;

  class AliHLTHistogramDefinition {
  public:
    AliHLTHistogramDefinition()
        : fName(), fSize(0), fExpr(), fCut(), fOpt()
    {
    }

	const TString& GetName()const{return fName;}
	void SetName(const TString& name){fName = name;}

	int GetSize()const{return fSize;}
	void SetSize(int size){fSize = size;}

	const TString& GetExpression()const{return fExpr;}
	void SetExpression(const TString& expr){fExpr = expr;}

	const TString& GetCut()const{return fCut;}
	void SetCut(const TString& cut){fCut = cut;}

	const TString& GetDrawOption()const{return fOpt;}
	void SetDrawOption(const TString& opt){fOpt = opt;}

  private:

    TString fName;
    int     fSize;
    TString fExpr;
    TString fCut;
    TString fOpt;
  };

  std::list<AliHLTHistogramDefinition> fDefinitions;
  typedef std::list<AliHLTHistogramDefinition>::iterator list_iterator;
  typedef std::list<AliHLTHistogramDefinition>::const_iterator list_const_iterator;


private:
  /// inherited from AliHLTComponent, scan argument
  int ScanConfigurationArgument(int argc, const char** argv);
  /// create the tree instance and all branches
  virtual TTree* CreateTree(int argc, const char** argv) = 0;
  /// process input blocks and fill tree
  virtual int FillTree(TTree* pTree, const AliHLTComponentEventData& evtData, 
                       AliHLTComponentTriggerData& trigData ) = 0;
  /// dtOrigin for PushBack.
  virtual AliHLTComponentDataType GetOriginDataType()const = 0;
  /// spec for PushBack
  virtual AliHLTUInt32_t GetDataSpec()const = 0;
  /// default histogram definitions.
  virtual void FillHistogramDefinitions() = 0;

  /// create a histogram from the tree
  TH1* CreateHistogram(const AliHLTHistogramDefinition& def);
  /// parse arguments, containing histogram definition.
  int ParseHistogramDefinition(int argc, const char** argv, int pos, AliHLTHistogramDefinition& dst)const;

  /// the TTree
  TTree* fTree; //! the tree instance
  /// max entries
  int fMaxEntries; //! maximum number of entries in the circular tree
  /// publish interval in s
  unsigned fPublishInterval; //! publish interval in s
  /// time stamp - publish or not.
  unsigned fLastTime; //! last time the histogramms were published

  TStopwatch* fpEventTimer; //! stopwatch for event processing
  TStopwatch* fpCycleTimer; //! stopwatch for event cycle
  AliHLTUInt32_t fMaxEventTime; //! allowed maximum processing time in usec
  AliHLTUInt32_t fNofEventsForce; //! number of events to ignore the processing time
  AliHLTUInt32_t fForcedEventsCount; //! event count for the forced events
  AliHLTUInt32_t fSkippedEventsCount; //! number of skipped events
  AliHLTUInt32_t fNewEventsCount; //! number of new events since last publishing

  static const AliHLTUInt32_t fgkTimeScale;

  /// copy constructor prohibited
  AliHLTTTreeProcessor(const AliHLTTTreeProcessor&);
  /// assignment operator prohibited
  AliHLTTTreeProcessor& operator = (const AliHLTTTreeProcessor&);

  ClassDef(AliHLTTTreeProcessor, 0)
};
#endif

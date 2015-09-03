#ifndef ExtrToTracker_h
#define ExtrToTracker_h 1

#include <marlin/Processor.h>
#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <time.h>
#include <math.h>
#include <sstream>
#include <assert.h>

#include <UTIL/LCRelationNavigator.h>
#include <UTIL/BitField64.h>
#include <UTIL/ILDConf.h>
#include <UTIL/BitSet32.h>

#include <EVENT/TrackerHit.h>
#include <EVENT/SimTrackerHit.h>
#include <EVENT/TrackerHitPlane.h>
#include <IMPL/TrackerHitImpl.h>
#include <EVENT/LCCollection.h>
#include <EVENT/LCRelation.h>
#include <IMPL/LCCollectionVec.h>
#include <IMPL/LCFlagImpl.h>
#include <IMPL/LCRelationImpl.h>
#include <EVENT/Track.h>
#include <IMPL/TrackStateImpl.h>
#include <IMPL/TrackImpl.h>

#include "KiTrack/SubsetHopfieldNN.h"
#include "KiTrack/SubsetSimple.h"

#include "marlin/Global.h"

//---- GEAR ----
/* #include "gear/GEAR.h" */
/* #include <gear/ZPlanarParameters.h> */
/* #include <gear/ZPlanarLayerLayout.h> */
/* #include "gear/BField.h" */

#include "DDRec/Surface.h"
#include "DDRec/SurfaceManager.h" 


// ----- include for verbosity dependend logging ---------
#include "marlin/VerbosityLevels.h"

#include "MarlinTrk/Factory.h"
#include "MarlinTrk/IMarlinTrack.h"
#include "MarlinTrk/MarlinTrkUtils.h"
#include "MarlinTrk/HelixTrack.h"






using namespace KiTrack;
namespace MarlinTrk{
  class IMarlinTrkSystem ;
}


class ExtrToTracker : public marlin::Processor {


public:


  virtual marlin::Processor*  newProcessor() { return new ExtrToTracker ; }
  
  ExtrToTracker() ;
  
  /** Called at the begin of the job before anything is read.
   * Use to initialize the processor, e.g. book histograms.
   */
  virtual void init() ;
  
  /** Called for every run.
   */
  virtual void processRunHeader( lcio::LCRunHeader* run ) ;
  
  /** Called for every event - the working horse.
   */
  virtual void processEvent( lcio::LCEvent * evt ) ; 
  
  
  virtual void check( lcio::LCEvent * evt ) ; 
  
  
  /** Called after data processing for clean up.
   */
  virtual void end() ;

  int FitInit2( Track* track , MarlinTrk::IMarlinTrack* _marlinTrk ) ;

  struct compare_r {
    bool operator()( EVENT::TrackerHit* a, EVENT::TrackerHit* b)  const { 
      double r_a_sqd = a->getPosition()[0] * a->getPosition()[0] + a->getPosition()[1] * a->getPosition()[1] ; 
      double r_b_sqd = b->getPosition()[0] * b->getPosition()[0] + b->getPosition()[1] * b->getPosition()[1] ; 
      return ( r_a_sqd < r_b_sqd ) ; 
    }
  } ;
  
  

  TrackerHitPlane* getSiHit(LCCollection*& sitHitsCol, int fitElID, MarlinTrk::IMarlinTrack*& marlin_trk, int& nHitsOnDetEl);  
  //TrackerHit* getSiHit(LCCollection*& sitHitsCol, int fitElID, MarlinTrk::IMarlinTrack*& marlin_trk, int& nHitsOnDetEl);  
  //bool getSiHit(LCCollection*& sitHitsCol, int fitElID, MarlinTrk::IMarlinTrack*& marlin_trk, TrackerHit*& selectedHit);
  
  

  void addHitOnNextElID(int elementID, MarlinTrk::IMarlinTrack*& marlin_trk, EVENT::TrackerHitVec& trkHits, LCCollection*& sitHitsCol, LCCollection*& otHitsCol, int& iL, int& nSITR, int& TotalSITHits, int& SITHitsPerTrk, int& SITHitsFitted, int& SITHitsNonFitted);


  void fillVecSubdet(lcio::LCEvent*& evt);



protected:
  
  /* helper function to get collection using try catch block */
  lcio::LCCollection* GetCollection( lcio::LCEvent * evt, std::string colName ) ;
  
  /* /\* helper function to get relations using try catch block *\/ */
  /* lcio::LCRelationNavigator* GetRelations(lcio::LCEvent * evt, std::string RelName ) ; */
  
  /** Input track collection name for refitting.
   */
  std::string _input_track_col_name ;
  
  
  /** refitted track collection name.
   */
  std::string _output_track_col_name ;
  
  /** Output track relations name for refitting.
   */
  std::string _output_track_rel_name ;
  
  /** pointer to the IMarlinTrkSystem instance 
   */
  MarlinTrk::IMarlinTrkSystem* _trksystem ;
  
  /* std::string _mcParticleCollectionName ; */

  bool _MSOn ;
  bool _ElossOn ;
  bool _SmoothOn ;
  double _Max_Chi2_Incr ;
  double _searchSigma ;
  
  int _n_run ;
  int _n_evt ;
  int SITHitsFitted ;
  int SITHitsNonFitted ;
  int TotalSITHits ;
  int _nHitsChi2 ;

  float _maxChi2PerHit;
  float _bField;

  bool _performFinalRefit ;
 

  const DD4hep::DDRec::SurfaceMap* _map ;

  

  //processor parameters

  StringVec _vecDigiHits;
  StringVec _vecSubdetName;
  std::vector<int > _vecSubdetNLayers;
  std::vector<int > _vecSubdetID;
  std::vector<LCCollection* > _vecDigiHitsCol;


  
  
} ;





#endif



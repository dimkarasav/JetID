import FWCore.ParameterSet.Config as cms 

process = cms.Process('jetToolbox')

process.load('PhysicsTools.PatAlgos.producersLayer1.patCandidates_cff')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')

from CommonTools.PileupAlgos.customizePuppiTune_cff import UpdatePuppiTuneV15
process.load('CommonTools.PileupAlgos.Puppi_cff')
process.load('CommonTools.PileupAlgos.customizePuppiTune_cff')
UpdatePuppiTuneV15(process,False)

## ----------------- Global Tag ------------------
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
#process.GlobalTag.globaltag = '102X_dataRun2_Sep2018Rereco_v1'
process.GlobalTag.globaltag = THISGLOBALTAG #<< = COMMENT OUT FOR LOCAL RUNNING

#--------------------- Report and output ---------------------------
# Note: in grid runs this parameter is not used.
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(1000))

process.load('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 1000


process.TFileService=cms.Service("TFileService",
                                ##  fileName=cms.string('test.root'),
                                  fileName=cms.string(THISROOTFILE), #<< = COMMENT OUT FOR LOCAL RUNNING
                                 closeFileFast = cms.untracked.bool(True)
                                 )

## --- suppress long output ---> wantSummary = cms.untracked.bool(False) 

process.options = cms.untracked.PSet(
        allowUnscheduled = cms.untracked.bool(True),
        wantSummary = cms.untracked.bool(False),
)


############## output  edm format ###############
#process.out = cms.OutputModule('PoolOutputModule',                                                                                                                  
#                               fileName = cms.untracked.string('jettoolbox.root'),                                                                              
 #                              outputCommands = cms.untracked.vstring([                                                                 
 #                                                                     'keep *_slimmedJets_*_*',                                                                  
#                                                                      'keep *_slimmedJetsPuppi_*_*',                                                                  
#                                                                       ])                                                                                           
 #                              )

#change here Dimitris for AK8 Puppi
process.out = cms.OutputModule('PoolOutputModule',                                                                                                                  
                               fileName = cms.untracked.string('jettoolbox.root'),                                                                              
                               outputCommands = cms.untracked.vstring([                                                                 
                                                                      'keep *_slimmedJets_*_*',                                                                  
                                                                      'keep *_slimmedJetsAK8_*_*',                                                                  
                                                                       ])                                                                                           
                               )


#from PhysicsTools.PatAlgos.tools.jetTools import *
#addJetCollection(process,labelName = 'AK4PFPUPPI', jetSource = cms.InputTag('ak4PuppiJets'), algo = 'AK', rParam=0.4, genJetCollection=cms.InputTag('slimmedJets')

# FOR PUPPI JETS
# An example where puppi jet specifics are computed
from PhysicsTools.PatAlgos.patPuppiJetSpecificProducer_cfi import patPuppiJetSpecificProducer

process.patPuppiJetSpecificProducer = patPuppiJetSpecificProducer.clone(
 # src=cms.InputTag("slimmedJetsPuppi"),
  src=cms.InputTag("slimmedJetsAK8"),#change here Dimitris for AK8 Puppi
)

# addedh


from PhysicsTools.PatAlgos.patTemplate_cfg import patAlgosToolsTask
patAlgosToolsTask.add(process.patPuppiJetSpecificProducer)
# added
from PhysicsTools.PatAlgos.tools.jetTools import updateJetCollection
updateJetCollection(
   process,
   labelName = 'PuppiJetSpecific',
 #  labelName = 'PFJetSpecific',
 #  labelName = 'SubJets',    #change here Dimitris for AK8 Puppi
#   jetSource = cms.InputTag('slimmedJetsPuppi'),
   jetSource = cms.InputTag('slimmedJetsAK8'),  #change here Dimitris for AK8 Puppi
)

# new
process.updatedPatJetsPuppiJetSpecific.userData.userFloats.src = ['patPuppiJetSpecificProducer:puppiMultiplicity','patPuppiJetSpecificProducer:neutralPuppiMultiplicity','patPuppiJetSpecificProducer:neutralHadronPuppiMultiplicity','patPuppiJetSpecificProducer:photonPuppiMultiplicity','patPuppiJetSpecificProducer:HFHadronPuppiMultiplicity','patPuppiJetSpecificProducer:HFEMPuppiMultiplicity']
# End  For PUPPI jets





# Added 'vertexRef().isNonnull() &&' check for 80X data compatibility. Juska
process.chs = cms.EDFilter('CandPtrSelector', src = cms.InputTag('packedPFCandidates'), cut = cms.string('vertexRef().isNonnull() && fromPV'))

from RecoJets.JetProducers.ak4GenJets_cfi import ak4GenJets
process.slimmedGenJetsAK8 = ak4GenJets.clone(src = 'packedGenParticles', rParam = 0.8)


#-------------------------------------------------------
# Gen Particles Pruner
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")


process.prunedGenParticlesDijet = cms.EDProducer('GenParticlePruner',
    src = cms.InputTag("prunedGenParticles"),
    select = cms.vstring(
    "drop  *  ", # by default
    "keep ( status = 3 || (status>=21 && status<=29) )", # keep hard process particles
    )
)



#------------- Recluster Gen Jets to access the constituents -------
#already in toolbox, just add keep statements

process.out.outputCommands.append("keep *_slimmedGenJets_*_*")
process.out.outputCommands.append("keep *_slimmedGenJetsAK8_*_*")


##-------------------- Define the source  ----------------------------



# Note: for grid running it does not matter what's here, as input data is
# handled separately there.

process.source = cms.Source("PoolSource",
    #fileNames = cms.untracked.vstring("root://eoscms//eos/cms/store/data/Run2016B/JetHT/MINIAOD/PromptReco-v2/000/273/411/00000/10CB3C59-721B-E611-AFB4-02163E012711.root")
    #fileNames = cms.untracked.vstring("file:/afs/cern.ch/user/j/juska/eos/cms/store/data/Run2016B/JetHT/MINIAOD/PromptReco-v2/000/273/411/00000/10CB3C59-721B-E611-AFB4-02163E012711.root")
    #fileNames = cms.untracked.vstring("file:/afs/cern.ch/user/j/juska/eos/cms/store/data/Run2016B/JetHT/MINIAOD/PromptReco-v2/000/273/730/00000/EA345ED4-B821-E611-BEA5-02163E0138E2.root")
   # fileNames = cms.untracked.vstring("file:/eos/cms/store/data/Run2018C/EGamma/MINIAOD/17Sep2018-v1/110000/BD446D92-0EB7-834D-890E-F2D7CF8A901F.root")
	fileNames = cms.untracked.vstring("file:/eos/cms/store/data/Run2018C/EGamma/MINIAOD/17Sep2018-v1/70000/C90F3CF6-CAFC-7B49-93AD-7FBB6A22911D.root") #THHIS   WORKS
#"file:/eos/cms/store/data/Run2017F/JetHT/MINIAOD/17Nov2017-v1/50000/FE551C87-8FDF-E711-8DAC-002590200A80.root")
    #filenames = cms.untracked.vstring("file:/eos/cms/store/group/phys_exotica/dijet/Dijet13TeV/magda/bigtrees_Run2017F-17Nov2017-v1/JetHT/crab_JetHT__Run2017F-17Nov2017-v1__MINIAOD/180105_194136/0000/*.root")
    
)

##-------------------- User analyzer  --------------------------------


# Residue from AOD and RECO running
calo_collection=''
cluster_collection=''
pfcalo_collection=''
   

process.dijets     = cms.EDAnalyzer('DijetTreeProducer',

  # There's no avoiding this in Consumes era
  isData          = cms.bool(True),

  ## JETS/MET ########################################
  jetsAK4             = cms.InputTag('slimmedJets'), 
#  jetsAK8             = cms.InputTag('slimmedJetsPuppi'),
# ANDREAS FOR NEW PUPPI
  jetsAK8          = cms.InputTag('updatedPatJetsPuppiJetSpecific'), 
  rho              = cms.InputTag('fixedGridRhoFastjetAll'),
  met              = cms.InputTag('slimmedMETs'),
  vtx              = cms.InputTag('offlineSlimmedPrimaryVertices'),
  ptMinAK4         = cms.double(10),
  ptMinAK8         = cms.double(10),
  
  ## MC ########################################
  pu               = cms.untracked.InputTag('slimmedAddPileupInfo'), # Updated from untracked to 80X by Juska
  ptHat            = cms.untracked.InputTag('generator'), # Why do these need to be 'untracked' anyway?
  genParticles     = cms.InputTag('prunedGenParticlesDijet'),
  genJetsAK4             = cms.InputTag('slimmedGenJets'), 
  genJetsAK8             = cms.InputTag('slimmedGenJetsAK8'),  


  ## trigger ###################################
  #triggerAlias     = cms.vstring('Fat','PFHT650','PFNoPUHT650','HT750','HT550'),
  ##### For 0T data  #####
  #triggerAlias     = cms.vstring('L1Jet68','L1Jet36','L1Jet16','L1EG20','L1EG5'),
  ##### For JetHT PD ##### 
   triggerAlias     = cms.vstring('PFHT800','PFHT1050','PFHT900',
                                 'PFJET400','PFJET200','PFJET500','PFJET550',
                                 'Mu50',
		                 'CaloJet500NoJetID','CaloJet550NoJetID',                               'HLT_PFHT500_PFMET100_PFMHT100_IDTight','HLT_PFHT500_PFMET110_PFMHT110_IDTight','HLT_PFHT700_PFMET85_PFMHT85_IDTight','HLT_PFHT700_PFMET95_PFMHT95_IDTight',
'HLT_PFHT800_PFMET75_PFMHT75_IDTight','HLT_PFHT800_PFMET85_PFMHT85_IDTight',
'PFHT380_SixJet32_DoubleBTagCSV_p075', 
'HLT_PFHT430_SixJet40_BTagCSV_p080'),   

#  triggerAlias     = cms.vstring('HLT_AK8PFHT800_TrimMass50_v12','HLT_AK8PFHT850_TrimMass50_v11','HLT_AK8PFHT900_TrimMass50_v11',
 #                                'HLT_AK8PFJet200_v15','HLT_AK8PFJet400_v16'),                                 
#

  triggerSelection = cms.vstring(

     ###
     ### For JetHT PD ###
     ###
     'HLT_PFHT800_v*',  # it exists and it is the PFHT prescaled 
     'HLT_PFHT1050_v*',  # it exists and it is the PFHT prescaled
     'HLT_PFHT900_v*', # it exists and it is the PFHT unprescaled
     'HLT_PFJet400_v*', # it exists and it is prescaled
     'HLT_PFJet200_v*', # it exists and it is unprescaled
     'HLT_PFJet500_v*', # it exists and it is unprescaled
     'HLT_PFJet550_v*', # it exists and it is unprescaled
     'HLT_Mu50_v*', 	# it exists and it is unprescaled
     'HLT_CaloJet500_NoJetID_v*', # it exists and it is unprescaled
     'HLT_CaloJet550_NoJetID_v*', # it exists and it is unprescaled
     'HLT_PFHT500_PFMET100_PFMHT100_IDTight_v*', # it exists and it is unprescaled
     'HLT_PFHT500_PFMET110_PFMHT110_IDTight_v*', # it exists and it is unprescaled 
     'HLT_PFHT700_PFMET85_PFMHT85_IDTight_v*', # it exists and it is unprescaled
     'HLT_PFHT700_PFMET95_PFMHT95_IDTight_v*', # it exists and it is unprescaled
     'HLT_PFHT800_PFMET75_PFMHT75_IDTight_v*', # it exists and it is unprescaled
     'HLT_PFHT800_PFMET85_PFMHT85_IDTight_v*', # it exists and it is unprescaled
     'HLT_PFHT380_SixJet32_DoubleBTagCSV_p075_v*', # it exists and it is unprescaled
     'HLT_PFHT430_SixJet40_BTagCSV_p080_v*',       # it exists and it is unprescaled
     ###
  ),

#    triggerSelection = cms.vstring(

     ###
     ### For JetHT PD ###
     ###
 #    'HLT_AK8PFHT800_TrimMass50_v12',  # it exists and it is the PFHT prescaled 
 #    'HLT_AK8PFHT850_TrimMass50_v11',  # it exists and it is the PFHT prescaled
 #    'HLT_AK8PFHT900_TrimMass50_v11', # it exists and it is the PFHT unprescaled
 #    'HLT_AK8PFJet200_v15', # it exists and it is prescaled
 #    'HLT_AK8PFJet400_v16', # it exists and it is unprescaled
 # ),



  triggerConfiguration = cms.PSet(
    hltResults            = cms.InputTag('TriggerResults','','HLT'),
    l1tResults            = cms.InputTag(''),
    daqPartitions         = cms.uint32(1),
    l1tIgnoreMaskAndPrescale = cms.bool(False),
    #l1tIgnoreMask         = cms.bool(False),
   # l1techIgnorePrescales = cms.bool(False),
    throw                 = cms.bool(False)
  ),


  ## Noise Filters ###################################


  noiseFilterSelection_HBHENoiseFilter = cms.string('Flag_HBHENoiseFilter'),
  noiseFilterSelection_globalSuperTightHalo2016Filter = cms.string('Flag_globalSuperTightHalo2016Filter'),
  noiseFilterSelection_HBHENoiseIsoFilter = cms.string('Flag_HBHENoiseIsoFilter'),
  noiseFilterSelection_EcalDeadCellTriggerPrimitiveFilter = cms.string('Flag_EcalDeadCellTriggerPrimitiveFilter'),
  noiseFilterSelection_goodVertices = cms.string('Flag_goodVertices'),
  noiseFilterSelection_eeBadScFilter = cms.string('Flag_eeBadScFilter'),
  noiseFilterSelection_BadChargedCandidateFilter = cms.string('Flag_BadChargedCandidateFilter'),
  noiseFilterSelection_BadPFMuonFilter = cms.string('Flag_BadPFMuonFilter'),
  #noiseFilterSelection_ecalLaserCorrFilter = cms.string('Flag_ecalLaserCorrFilter'),
  #noiseFilterSelection_trkPOGFilters = cms.string('Flag_trkPOGFilters'),
#noiseFilterSelection_CSCTightHaloFilter = cms.string('Flag_CSCTightHaloFilter'),
  #noiseFilterSelection_hcalLaserEventFilter = cms.string('Flag_hcalLaserEventFilter'),

#noiseFilterSelection_trackingFailureFilter = cms.string('Flag_trackingFailureFilter'),
  # and the sub-filters
 # noiseFilterSelection_trkPOG_manystripclus53X = cms.string('Flag_trkPOG_manystripclus53X'),
  #noiseFilterSelection_trkPOG_toomanystripclus53X = cms.string('Flag_trkPOG_toomanystripclus53X'),
  #noiseFilterSelection_trkPOG_logErrorTooManyClusters = cms.string('Flag_trkPOG_logErrorTooManyClusters'),

  noiseFilterConfiguration = cms.PSet(
    hltResults            = cms.InputTag('TriggerResults','','RECO'), #for prompt reco
    l1tResults            = cms.InputTag(''),
    daqPartitions         = cms.uint32(1),
    l1tIgnoreMaskAndPrescale = cms.bool(False),
    #l1tIgnoreMask         = cms.bool(False),
    #l1techIgnorePrescales = cms.bool(False),
    throw                 = cms.bool(False)
  ),


  ## JECs ################
  redoJECs  = cms.bool(True),

  ## Version Summer15_25nsV3 ( https://hypernews.cern.ch/HyperNews/CMS/get/JetMET/ )
  # Note that it hardly matters what is put in here, as these should be overriden in analysis step anyway. Juska.
  # That's also why these JEC's are greatly dated.
  L1corrAK4_DATA = cms.FileInPath('CMSDIJET/DijetRootTreeMaker/data/Summer15_25nsV3_DATA/Summer15_25nsV3_DATA_L1FastJet_AK4PFchs.txt'),
  L2corrAK4_DATA = cms.FileInPath('CMSDIJET/DijetRootTreeMaker/data/Summer15_25nsV3_DATA/Summer15_25nsV3_DATA_L2Relative_AK4PFchs.txt'),
  L3corrAK4_DATA = cms.FileInPath('CMSDIJET/DijetRootTreeMaker/data/Summer15_25nsV3_DATA/Summer15_25nsV3_DATA_L3Absolute_AK4PFchs.txt'),
  ResCorrAK4_DATA = cms.FileInPath('CMSDIJET/DijetRootTreeMaker/data/Summer15_25nsV3_DATA/Summer15_25nsV3_DATA_L2L3Residual_AK4PFchs.txt'),
  L1corrAK8_DATA = cms.FileInPath('CMSDIJET/DijetRootTreeMaker/data/Summer15_25nsV3_DATA/Summer15_25nsV3_DATA_L1FastJet_AK8PFchs.txt'),
  L2corrAK8_DATA = cms.FileInPath('CMSDIJET/DijetRootTreeMaker/data/Summer15_25nsV3_DATA/Summer15_25nsV3_DATA_L2Relative_AK8PFchs.txt'),
  L3corrAK8_DATA = cms.FileInPath('CMSDIJET/DijetRootTreeMaker/data/Summer15_25nsV3_DATA/Summer15_25nsV3_DATA_L3Absolute_AK8PFchs.txt'),
  ResCorrAK8_DATA = cms.FileInPath('CMSDIJET/DijetRootTreeMaker/data/Summer15_25nsV3_DATA/Summer15_25nsV3_DATA_L2L3Residual_AK4PFchs.txt'),
  L1corrAK4_MC = cms.FileInPath('CMSDIJET/DijetRootTreeMaker/data/Summer15_25nsV3_MC/Summer15_25nsV3_MC_L1FastJet_AK4PFchs.txt'),
  L2corrAK4_MC = cms.FileInPath('CMSDIJET/DijetRootTreeMaker/data/Summer15_25nsV3_MC/Summer15_25nsV3_MC_L2Relative_AK4PFchs.txt'),
  L3corrAK4_MC = cms.FileInPath('CMSDIJET/DijetRootTreeMaker/data/Summer15_25nsV3_MC/Summer15_25nsV3_MC_L3Absolute_AK4PFchs.txt'),
  L1corrAK8_MC = cms.FileInPath('CMSDIJET/DijetRootTreeMaker/data/Summer15_25nsV3_MC/Summer15_25nsV3_MC_L1FastJet_AK8PFchs.txt'),
  L2corrAK8_MC = cms.FileInPath('CMSDIJET/DijetRootTreeMaker/data/Summer15_25nsV3_MC/Summer15_25nsV3_MC_L2Relative_AK8PFchs.txt'),
  L3corrAK8_MC = cms.FileInPath('CMSDIJET/DijetRootTreeMaker/data/Summer15_25nsV3_MC/Summer15_25nsV3_MC_L3Absolute_AK8PFchs.txt')

#Add MET filters#
#process.load('RecoMET.METFilters.BadPFMuonFilter_cfi')
#process.BadPFMuonFilter.muons = cms.InputTag("slimmedMuons")
#process.BadPFMuonFilter.PFCandidates = cms.InputTag("packedPFCandidates")

#process.load('RecoMET.METFilters.BadChargedCandidateFilter_cfi')
#process.BadChargedCandidateFilter.muons = cms.InputTag("slimmedMuons")
#process.BadChargedCandidateFilter.PFCandidates = cms.InputTag("packedPFCandidates")

)


# ------------------ path --------------------------


process.p = cms.Path()
process.p +=                      process.patPuppiJetSpecificProducer
process.p +=                      process.updatedPatJetsPuppiJetSpecific
process.p +=                      process.chs
process.p +=                      process.dijets
#Add MET Filters
#process.p +=  process.BadPFMuonFilter
#process.p +=  process.BadChargedCandidateFilter 

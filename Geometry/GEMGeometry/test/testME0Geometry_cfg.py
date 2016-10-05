import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")
# process.load("Configuration.Geometry.GeometryExtended2023Muon_cff")
# process.load("Configuration.Geometry.GeometryExtended2023MuonReco_cff")
process.load("Configuration.Geometry.GeometryExtended2023D1ME0Dev_cff")
process.load("Configuration.Geometry.GeometryExtended2023D1ME0DevReco_cff")
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

# process.load('FWCore.MessageLogger.MessageLogger_cfi')
# process.MessageLogger.cout.threshold = cms.untracked.string('DEBUG')
# process.MessageLogger.debugModules = cms.untracked.vstring('ME0GeometryBuilderFromDDD')
# process.MessageLogger.destinations = cms.untracked.vstring("cout")
# process.MessageLogger.cout = cms.untracked.PSet(threshold = cms.untracked.string("DEBUG"))

### TO ACTIVATE LogTrace NEED TO COMPILE IT WITH:
### -----------------------------------------------------------
### --> scram b -j8 USER_CXXFLAGS="-DEDM_ML_DEBUG"             
### Make sure that you first cleaned your CMSSW version:       
### --> scram b clean                                          
### before issuing the scram command above                     
###############################################################
process.load("FWCore.MessageLogger.MessageLogger_cfi")
# process.MessageLogger.categories.append("GEMGeometryBuilderFromDDD")
# process.MessageLogger.categories.append("GEMNumberingScheme")
# process.MessageLogger.categories.append("ME0GeometryBuilderFromDDD")
# process.MessageLogger.categories.append("ME0NumberingScheme")
process.MessageLogger.debugModules = cms.untracked.vstring("*")
process.MessageLogger.destinations = cms.untracked.vstring("cout","junk")
process.MessageLogger.cout = cms.untracked.PSet(
    threshold = cms.untracked.string("DEBUG"),
    default = cms.untracked.PSet( limit = cms.untracked.int32(0) ),
    FwkReport = cms.untracked.PSet( limit = cms.untracked.int32(-1) ),
    # GEMGeometryBuilderFromDDD   = cms.untracked.PSet( limit = cms.untracked.int32(-1) ),
    # GEMNumberingScheme          = cms.untracked.PSet( limit = cms.untracked.int32(-1) ),
    # ME0GeometryBuilderFromDDD   = cms.untracked.PSet( limit = cms.untracked.int32(-1) ),
    # ME0NumberingScheme          = cms.untracked.PSet( limit = cms.untracked.int32(-1) ),
)




from Configuration.AlCa.GlobalTag import GlobalTag
# process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:upgradePLS3', '')
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')

process.load("Geometry.GEMGeometryBuilder.me0Geometry_cfi")
process.load("Geometry.GEMGeometryBuilder.gemGeometry_cfi")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
)
process.source = cms.Source("EmptySource")

# process.MessageLogger = cms.Service("MessageLogger")

process.test = cms.EDAnalyzer("ME0GeometryAnalyzer")

process.p = cms.Path(process.test)


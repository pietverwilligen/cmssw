import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")
# process.load("Geometry.MuonCommonData.muonIdealGeometryXML_cfi")
# process.load("Configuration.Geometry.GeometryExtended2015Reco_cff")
process.load("Configuration.Geometry.GeometryExtended2023_cff")
# process.load("Configuration.Geometry.GeometryExtended2015Reco_RPC2Gap_cff")
# process.load("Configuration.Geometry.GeometryExtended2015Reco_RPC4RE11_cff")

process.load("Geometry.RPCGeometry.rpcGeometry_cfi")

process.load("Geometry.MuonNumbering.muonNumberingInitialization_cfi")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
)
process.source = cms.Source("EmptySource")

process.MessageLogger = cms.Service("MessageLogger",
    # --- get more debug printout ---
    # activate LogDebug messages
    # more info: https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideMessageLoggerDebug
    destinations = cms.untracked.vstring(
        'detailedInfo',
         'critical'
    ),
    detailedInfo = cms.untracked.PSet(
        threshold = cms.untracked.string('DEBUG')
    ),
    debugModules = cms.untracked.vstring(
        'RPCGeometryESModule',
        'RPCGeometryBuilderFromDDD',
        'MuonNumberingInitialization' 
    )
)

process.demo = cms.EDAnalyzer("RPCGEO")

process.p = cms.Path(process.demo)


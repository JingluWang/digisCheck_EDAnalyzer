// system include files
#include <memory>

// user includes files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

// CMSSW includes
#include "DataFormats/HGCalDigi/interface/HGCalDigiHostCollection.h"
#include "DataFormats/HGCalDigi/interface/alpaka/HGCalDigiDeviceCollection.h"
#include "CondFormats/DataRecord/interface/HGCalElectronicsMappingRcd.h"
#include "CondFormats/HGCalObjects/interface/HGCalMappingModuleIndexer.h"

// include for debug info
#include "FWCore/MessageLogger/interface/MessageLogger.h"


class HGCalRaw2DigiTransferTest : public edm::EDAnalyzer {
    public:
        explicit HGCalRaw2DigiTransferTest(const edm::ParameterSet&);
        ~HGCalRaw2DigiTransferTest();

        static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

    private:
        virtual void beginJob();
        virtual void analyze(const edm::Event&, const edm::EventSetup&);
        virtual void endJob();
        virtual void beginRun(edm::Run const&, edm::EventSetup const&);
        virtual void endRun(edm::Run const&, edm::EventSetup const&);
        virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
        virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);

        // digi
        const edm::EDGetTokenT<hgcaldigi::HGCalDigiHostCollection> digisToken_;

        // config tokens and objects
        edm::ESWatcher<HGCalElectronicsMappingRcd> mapWatcher_;
        HGCalMappingModuleIndexer moduleIndexer_;// denseIdx
};

HGCalRaw2DigiTransferTest::HGCalRaw2DigiTransferTest(const edm::ParameterSet& iConfig)
    : digisToken_{consumes<hgcaldigi::HGCalDigiHostCollection>(iConfig.getParameter<edm::InputTag>("digis"))},
      n_hits_scale{iConfig.getParameter<int>("n_hits_scale")},
      moduleIndexToken_(esConsumes<edm::Transition::BeginRun>())
{
    esToken_ = esConsumes(iConfig.getParameter<edm::ESInputTag>("eventSetupSource"));
}

HGCalRaw2DigiTransferTest::~HGCalRaw2DigiTransferTest()
{
}

void
HGCalRaw2DigiTransferTest::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    using namespace edm;

    auto queue = iEvent.queue();

    // read digis
    auto const& deviceCalibrationParameterProvider = iSetup.getData(esToken_);
    auto const& hostDigisIn = iEvent.get(digisToken_);

    int oldSize = hostDigisIn.view().metadata().size();
    int newSize = oldSize * n_hits_scale;
    auto hostDigis = HGCalDigiHostCollection(newSize, queue);

    for (int i = 0; i < oldSize; i++) {
        denseIdx = moduleIndexer_(hostDigisIn.view()[i].electronicsId());

        if ( hostDigis.view()[i%newSize].tctp()  == hostDigis.view()[denseIdx].tctp()  )
        if ( hostDigis.view()[i%newSize].adcm1() == hostDigis.view()[denseIdx].adcm1() )
        if ( hostDigis.view()[i%newSize].adc()   == hostDigis.view()[denseIdx].adc()   )
        if ( hostDigis.view()[i%newSize].tot()   == hostDigis.view()[denseIdx].tot()   )
        if ( hostDigis.view()[i%newSize].toa()   == hostDigis.view()[denseIdx].toa()   )
        if ( hostDigis.view()[i%newSize].cm()    == hostDigis.view()[denseIdx].cm()    )
        if ( hostDigis.view()[i%newSize].flags() == hostDigis.view()[denseIdx].flags() )
    }

}

void 
HGCalRaw2DigiTransferTest::beginJob()
{
}

void 
HGCalRaw2DigiTransferTest::endJob()
{
}

void
HGCalRaw2DigiTransferTest::beginRun(edm::Run const&, edm::EventSetup const&)
{
    if ( mapWatcher_.check(iSetup)) {
        moduleIndexer_ = iSetup.getData(moduleIndexToken_);
    }
}

void
HGCalRaw2DigiTransferTest::endRun(edm::Run const&, edm::EventSetup const&)
{
}

void
HGCalRaw2DigiTransferTest::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

void
HGCalRaw2DigiTransferTest::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

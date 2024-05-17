# digisCheck_EDAnalyzer

## Directory of EDAnalyzer @lxplus9

#### ssh lxplus9 (with necessary libraries)
    ssh -Y jinglu@lxplus9.cern.ch 
#### go to working dir (digisCheck_EDAnalyzer/) in workspace (/afs/cern.ch/work/j/jinglu)
    . toWorkspace.sh
    cd private/HGCAL/digisCheck_EDAnalyzer

### Folder 1 -- This "EDAnalyzer" for modification/GitHub updating
####
    cd EDAnalyzer/

### Folder 2 -- CMSSW real work (compiling & running)
####
    cd CMSSW_14_1_X/src/HGCalCommissioning/SystemTestEventFilters/

- Backup: env setup (from HGCAL DPG Mini Hackathons, May 14, 2024)
    #### in working dir
        source /cvmfs/cms.cern.ch/cmsset_default.sh

        # install
        export SCRAM_ARCH="el9_amd64_gcc12"
        cmsrel CMSSW_14_1_X_2024-05-13-2300 -n CMSSW_14_1_X # custom project name
        cd CMSSW_14_1_X/src/
        cmsenv
        git cms-merge-topic -u CMS-HGCAL:dev/hackathon_base_CMSSW_14_1_X
        
        # load external tools & data
        git clone https://github.com/pfs/Geometry-HGCalMapping.git  $CMSSW_BASE/src/Geometry/HGCalMapping/data
        git clone https://gitlab.cern.ch/hgcal-dpg/hgcal-comm.git HGCalCommissioning

        # compile
        scram b -j 10
    

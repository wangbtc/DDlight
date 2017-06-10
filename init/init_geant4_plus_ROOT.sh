
echo Setting up cvmfs-based Geant4 environment...

source /cvmfs/geant4.cern.ch/etc/login.sh
source /cvmfs/sft.cern.ch/lcg/contrib/gcc/4.9.3/x86_64-slc6/setup.sh
source /cvmfs/geant4.cern.ch/geant4/10.2.p02/x86_64-slc6-gcc49-opt/CMake-setup.sh

echo Setting up ROOT...

#source /software/root/v6.02.04/bin/thisroot.sh
source /cvmfs/sft.cern.ch/lcg/views/LCG_88/x86_64-slc6-gcc49-opt/setup.sh

echo ROOT setup finished

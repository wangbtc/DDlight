# DDM
Direct DM Pythia toy model


## Setting up environment and compile

```bash
source init/init_geant4_plus_ROOT.sh
```

compile:

```bash
mkdir build
cd build
cmake -DGeant4_DIR=/cvmfs/geant4.cern.ch/geant4/10.2.p02/x86_64-slc6-gcc49-opt/lib64/  ../
make -j
```


## instructions 

The code is based on CERN scientific linux. Vagrant allows to develop and execute platform independent. All dependencies and
instructions are enclosed in the single 'vagrantfile'. The machine has to have installed:

* Virtualbox
* Install Vagrant The Vagrant box in use is the CernVM.

NOTE: By default this setup will use 2 cores and 2 GB of RAM. If you want to adjust these numbers, please edit the Vagrantfile before continuing.

Just perform the following steps:

1. Clone repository: git clone
Open terminal (or powershell on Windows)
cd into the project directory
vagrant up # can take a while
vagrant ssh
cd /vagrant


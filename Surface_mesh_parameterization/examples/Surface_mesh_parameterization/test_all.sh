#!/bin/bash

# Intensive test: test all surface parameterization methods with all models in data folder

echo "***************************************************************"

# ./test_model.sh cube
# ./test_model.sh holes
# ./test_model.sh mannequin-devil
# ./test_model.sh mask_cone
# ./test_model.sh nefertiti
# ./test_model.sh rotor
# ./test_model.sh sphere966
# ./test_model.sh three_peaks

echo "***************************************************************"

for TST in Authalic_parameterization Simple_parameterization Taucs_parameterization Mesh_cutting_parameterization Square_border_parameterization polyhedron_ex_parameterization
do
    echo " "
    echo "*** $TST ***"

    # Find executable name (different on Windows and Unix)
    [ -f ./release/$TST.exe ] && PARAM_APPLICATION="./release/$TST.exe"
    [ -x ./$TST ] && PARAM_APPLICATION="./$TST"

    $PARAM_APPLICATION `cat $TST.cmd`
done

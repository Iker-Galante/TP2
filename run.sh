#!/bin/bash
# Para debuggear, ejecutar con el argumento -d
if(($#==1))
then
    if [ $1 == "-d" ]
    then
    echo "Debuggeando..."
    qemu-system-x86_64 -hda Image/x64BareBonesImage.qcow2 -m 512 -device intel-hda -device hda-output -d -S -s
    fi
else
    qemu-system-x86_64 -hda Image/x64BareBonesImage.qcow2 -m 512 -device intel-hda -device hda-output
fi
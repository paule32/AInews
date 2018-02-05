#!/bin/bash
echo "Building help documentation..."
cd /home/jens/AInews/help
/home/jens/Qt5.10/5.10.0/gcc_64/bin/qhelpgenerator help.qhp -o help.qhc
/home/jens/Qt5.10/5.10.0/gcc_64/bin/qcollectiongenerator help.qhcp -o help.qhc

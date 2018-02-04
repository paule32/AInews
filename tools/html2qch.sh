#!/bin/bash
echo "Building help documentation..."
/home/jens/Qt5.10/5.10.0/gcc_64/bin/qhelpgenerator ../help/dBaseHelp.qhp -o ../help/dBaseHelp.qch
/home/jens/Qt5.10/5.10.0/gcc_64/bin/qcollectiongenerator ../help/dBaseHelp.qhcp -o ../help/dBaseHelp.qhc


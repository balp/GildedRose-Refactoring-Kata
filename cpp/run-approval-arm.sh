#!/bin/bash

if [[ -x ./dockcross-linux-armv7 ]]; then
    docker run --rm dockcross/linux-armv7 > ./dockcross-linux-armv7
fi

./dockcross-linux-armv7 bash -c "cmake -Bxcbuild -H. " && ./dockcross-linux-armv7 bash -c "cmake --build xcbuild --target GildedRoseApprovalTests" && docker run -v $(pwd):/work -w /work/xcbuild -it --rm multiarch/ubuntu-core:armhf-bionic /work/xcbuild/GildedRoseApprovalTests

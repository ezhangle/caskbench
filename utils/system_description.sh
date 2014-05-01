#!/bin/bash

export DISPLAY=:0 

. /etc/lsb-release

cpu=$(grep ^model\ name /proc/cpuinfo | head -n1 | cut -d: -f2)
xserver_version=$(/usr/bin/Xorg -version |& grep ^xorg-server | cut -d' ' -f2)
mesa_version=$(glxinfo  | grep "OpenGL version")
xpci=$(xpci)
kernel=$(uname -r)
platform=$(uname -po)

cat <<EOF
System
------------------------------------------------------------------------
cpu:            ${cpu}
platform:       ${platform}
distribution:   ${DISTRIB_DESCRIPTION}
linux kernel:   ${kernel}
xserver:        ${xserver_version}
mesa:           ${mesa_version}
video:          ${xpci}
------------------------------------------------------------------------

EOF

# TODO: Detect this
cat <<EOF
Caskbench
------------------------------------------------------------------------
description:    TBD
respository:    https://github.com/SRA-SiliconValley/caskbench
version:        $(git log | head -n 1 | cut -d' ' -f2)
config:         Default
patches:        None
build-type:     Release

EOF

# TODO: Detect this
cat <<EOF
Cairo
------------------------------------------------------------------------
description:    filters-wip branch of the cairogles fork
repository:     https://github.com/SRA-SiliconValley/cairogles
version:        1a15478f9398ac5d0afbae977e548079bfc9d43a
config:         All defaults, with
                --enable-egl --enable-gles3
patches:        None
build-type:     Release
------------------------------------------------------------------------

EOF

# TODO: Detect this
cat <<EOF
Skia
------------------------------------------------------------------------
description:    Manually installed skia-sra from SRA-SiliconValley
repository:     https://github.com/SRA-SiliconValley/skia
version:        a850b9f1e9da4dbf0956543ae7f843d0d0883919
config:         -Dskia_build_for_tizen=1 \\
                -Duse_system_libjpeg=1 \\
                -Duse_system_libpng=1 \\
                -Dskia_shared_lib=1 \\
                -Dskia_arch_width=64 \\
                -Dskia_arch_type=x86
build-type:     Release
------------------------------------------------------------------------

EOF


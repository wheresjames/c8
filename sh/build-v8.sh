#!/usr/bin/env bash

set -e

#===================================================================
Log()
{
    echo "[v8-build] $@"
}

#===================================================================
if [ $# -lt 6 ]; then
    Log "Usage: $0 <BuildType> <Platform> <CPU> <SourceDir> <BuildDir> <InstallDir>"
    Log "Example: $0 Release linux x86_64 ./build ./install"
    exit 1
fi

BUILD_TYPE="${1,,}"         # release, debug
PLATFORM="${2,,}"           # linux, mac, windows, etc.
CPU="${3,,}"                # x86_64, arm64, etc.
SOURCE_DIR="${4,,}"
BUILD_DIR="${5,,}"
INSTALL_DIR="${6,,}"
V8_VERSION="${7,,}"

if [[ "$CPU" == "x86_64" ]]; then
    CPU="x64"
fi

IS_DEBUG="false"
if [[ "$BUILD_TYPE" == "debug" ]]; then
    IS_DEBUG="true"
fi

#===================================================================
# Dump parameters
Log "BUILD_TYPE    : ${BUILD_TYPE}"
Log "PLATFORM      : ${PLATFORM}"
Log "CPU           : ${CPU}"
Log "SOURCE_DIR    : ${SOURCE_DIR}"
Log "BUILD_DIR     : ${BUILD_DIR}"
Log "INSTALL_DIR   : ${INSTALL_DIR}"
Log "V8_VERSION    : ${V8_VERSION}"

#===================================================================
if [[ ! "$BUILD_TYPE" =~ ^(release|rebug)$ ]]; then
    Log "Error: BuildType must be 'Release' or 'Debug'."
    exit 1
fi
if [[ ! "$PLATFORM" =~ ^(linux|mac|win)$ ]]; then
    Log "Error: Platform must be 'linux', 'mac', or 'win'."
    exit 1
fi
if [[ ! "$CPU" =~ ^(x64|x86_64|arm64)$ ]]; then
    Log "Error: CPU must be 'x86_64' or 'arm64'."
    exit 1
fi
if [ -z "$SOURCE_DIR" ]; then
    Log "Error: SourceDir cannot be empty."
    exit 1
fi
if [ -z "$BUILD_DIR" ]; then
    Log "Error: BuildDir cannot be empty."
    exit 1
fi
if [ -z "$INSTALL_DIR" ]; then
    Log "Error: InstallDir cannot be empty."
    exit 1
fi
if [ -z "$V8_VERSION" ]; then
    V8_VERSION="13.7.81"
    Log "Warning: V8_VERSION not specified, using default version ${V8_VERSION}."
fi


#===================================================================
DEPOT_TOOLS_DIR="${SOURCE_DIR}/depot_tools"


#===================================================================
if [ ! -d "${DEPOT_TOOLS_DIR}" ]; then
    Log "[+] Cloning depot_tools..."
    git clone --depth 1 https://chromium.googlesource.com/chromium/tools/depot_tools.git "${DEPOT_TOOLS_DIR}"
fi
export PATH="${DEPOT_TOOLS_DIR}:$PATH"


#===================================================================
if [ ! -d "${SOURCE_DIR}/v8" ]; then
    Log "[+] Fetching V8 source..."
    mkdir -p "${SOURCE_DIR}"
    cd "${SOURCE_DIR}"
    fetch v8
else
    Log "[=] V8 source already exists..."
fi


#===================================================================
Log "[+] Syncing V8 source..."
cd "${SOURCE_DIR}/v8"
git checkout "${V8_VERSION}"
gclient sync


#===================================================================
cd "${SOURCE_DIR}/v8"

Log "[+] Configuring GN build..."
tools/dev/v8gen.py ${CPU}.${BUILD_TYPE,,}

Log "[+] Createing GN args file..."
cat > "${BUILD_DIR}/args.gn" <<EOF
is_debug = ${IS_DEBUG}
current_cpu = "${CPU}"
target_cpu = "${CPU}"
target_os = "${PLATFORM}"
target_platform = ""
v8_current_cpu = "${CPU}"
v8_target_cpu = "${CPU}"
v8_monolithic = true
v8_use_external_startup_data = false
is_component_build = false
use_thin_lto = false
v8_enable_pointer_compression=false
is_clang = false
use_custom_libcxx=false
use_sysroot = true
treat_warnings_as_errors = false
EOF

# Log "[+] Listing GN args..."
# gn args "${BUILD_DIR}" --list --short


#===================================================================
Log "[+] Generating GN build files..."
gn gen "${BUILD_DIR}"
if [ $? -ne 0 ]; then
    Log "Error: GN configuration failed."
    exit 1
fi

#===================================================================
Log "[+] Building V8 (v8_monolith)..."
ninja -C "${BUILD_DIR}" v8_monolith
if [ $? -ne 0 ]; then
    Log "Error: V8 build failed."
    exit 1
fi


#===================================================================
Log "[+] Installing to ${INSTALL_DIR}"

INSTALL_LIB_DIR="${INSTALL_DIR}/lib"
INSTALL_INCLUDE_DIR="${INSTALL_DIR}/include"

mkdir -p "${INSTALL_LIB_DIR}" "${INSTALL_INCLUDE_DIR}"

cp "${BUILD_DIR}/obj/libv8_monolith.a" "${INSTALL_LIB_DIR}/"
if [ $? -ne 0 ]; then
    Log "Error: Failed to copy libv8_monolith.a."
    exit 1
fi

cp -r "${SOURCE_DIR}/v8/include/"* "${INSTALL_INCLUDE_DIR}/"
if [ $? -ne 0 ]; then
    Log "Error: Failed to copy include files."
    exit 1
fi

Log "[✓] Build and install complete."
Log "    Library:  ${INSTALL_LIB_DIR}/libv8_monolith.a"
Log "    Includes: ${INSTALL_INCLUDE_DIR}/"

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

BUILT_DIR="${SOURCE_DIR}/v8-install"

#===================================================================
# Dump parameters
Log "BUILD_TYPE    : ${BUILD_TYPE}"
Log "PLATFORM      : ${PLATFORM}"
Log "CPU           : ${CPU}"
Log "SOURCE_DIR    : ${SOURCE_DIR}"
Log "BUILD_DIR     : ${BUILD_DIR}"
Log "INSTALL_DIR   : ${INSTALL_DIR}"
Log "V8_VERSION    : ${V8_VERSION}"
Log "BUILT_DIR     : ${BUILT_DIR}"

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
    V8_VERSION="14.5.202"
    Log "Warning: V8_VERSION not specified, using default version ${V8_VERSION}."
fi

# Is it already installed
if [ -d "${INSTALL_DIR}" ]; then
    Log "[✓] Build and install exists: ${INSTALL_DIR}"
    exit 0
fi

# Is there already a built version
if [ -d "${BUILT_DIR}" ]; then
    mkdir -p "${INSTALL_DIR}"
    cp -r "${BUILT_DIR}/"* "${INSTALL_DIR}/"
    Log "[✓] Restored from: ${BUILT_DIR}"
    exit 0
fi

# exit -1

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
# Patch for V8_VERSION is < 14.8.* "avx10.2-512" error in highway
if [[ "$(printf '%s\n' "$V8_VERSION" "14.8.0" | sort -V | head -n1)" == "$V8_VERSION" && "$V8_VERSION" != "14.8.0" ]]; then
    Log "[+] Applying patch for highway/ops/set_macros-inl.h..."
    HWY_FILE=$(find . -name "set_macros-inl.h")
    if [ -f "$HWY_FILE" ]; then
        Log "    Patching $HWY_FILE"
        sed -i 's/avx10.2-512/avx10.2/g' "$HWY_FILE"
    else
        Log "    Warning: set_macros-inl.h not found, skipping patch."
    fi
fi

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
v8_static_library = true
enable_rust_cxx = true
v8_enable_temporal_support = true
rust_compiler_enabled = true
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

mkdir -p "${BUILT_DIR}"

cp -r "${INSTALL_DIR}/"* "${BUILT_DIR}/"

Log "[✓] Build and install complete."
Log "    Library:  ${INSTALL_LIB_DIR}/libv8_monolith.a"
Log "    Includes: ${INSTALL_INCLUDE_DIR}/"

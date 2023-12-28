set(VCPKG_TARGET_ARCHITECTURE x64)
set(VCPKG_CRT_LINKAGE dynamic)
set(VCPKG_LIBRARY_LINKAGE static)

set(VCPKG_LINKER_FLAGS "-lc++abi -fuse-ld=lld")

set(VCPKG_CHAINLOAD_TOOLCHAIN_FILE "${CMAKE_CURRENT_LIST_DIR}/toolchain/linux-llvm.cmake")
set(VCPKG_CMAKE_SYSTEM_NAME Linux)
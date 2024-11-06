vcpkg_check_linkage(ONLY_STATIC_LIBRARY)

vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO YGNI-RType/GEngine
    REF v0.5.0
    SHA512 e2080f9d406a9e33ef23517bc0f9bf6434778453ceec55a90d1b62acdb99c7b3ab3e0208416cc0982c2b11eba7e53ab233047d323197fd6a79b4f92392f4a172
)

set(ENV{NOT_INSTALL_VCPKG} true)

vcpkg_cmake_configure(
    SOURCE_PATH "${SOURCE_PATH}"
)

vcpkg_cmake_install()

vcpkg_cmake_config_fixup(PACKAGE_NAME "GEngine")

file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")

file(INSTALL "${SOURCE_PATH}/LICENSE" DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}" RENAME copyright)

configure_file("${CMAKE_CURRENT_LIST_DIR}/usage" "${CURRENT_PACKAGES_DIR}/share/${PORT}/usage" COPYONLY)

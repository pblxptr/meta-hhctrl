
FILESEXTRAPATHS_prepend := "${THISDIR}/files:"
FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"

SRC_URI_append = " file://0001-Added-device-tree-overlay-for-hatch2sr.patch;md5sum=4334eec835f914ae9da7120e3c79fbe1"
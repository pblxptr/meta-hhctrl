
FILESEXTRAPATHS_prepend := "${THISDIR}/files:"
FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"

SRC_URI_append = " file://0001-Added-device-tree-overlay-for-hatch2sr.patch;md5sum=bf51bcbe7baf831178485322b404b4e2"
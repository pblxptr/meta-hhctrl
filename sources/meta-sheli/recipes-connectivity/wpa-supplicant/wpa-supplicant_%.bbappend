FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

SRC_URI += "\
  file://wifi-dev.conf \
  file://wifi-integration.conf \
"

do_install_append () {
    cat ${WORKDIR}/wifi-${DISTRO_STAGE}.conf >>  ${D}${sysconfdir}/wpa_supplicant.conf
}
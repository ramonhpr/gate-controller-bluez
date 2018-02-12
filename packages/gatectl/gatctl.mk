################################################################################
#
# gatectl
#
################################################################################

GATECTL_VERSION = master
GATECTL_SITE = https://github.com/ramonhpr/gate-controller-bluez.git
GATECTL_SITE_METHOD = git
GATECTL_INSTALL_STAGING = NO
GATECTL_INSTALL_TARGET = YES
GATECTL_DEPENDENCIES = host-pkgconf libell
GATECTL_AUTORECONF = YES
GATECTL_TOOLS_AUTORECONF_OPTS = --force --install

define GATECTL_BOOTSTRAP
        cd $(@D) && ./bootstrap
endef

GATECTL_PRE_CONFIGURE_HOOKS += GATECTL_BOOTSTRAP

$(eval $(autotools-package))


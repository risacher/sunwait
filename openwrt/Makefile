include $(TOPDIR)/rules.mk

PKG_NAME:=sunwait
PKG_VERSION:=1.0
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.xz
PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=https://github.com/risacher/sunwait.git
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

define Package/sunwait
	SUBMENU:=Utilities
	SECTION:=util
	CATEGORY:=Utilities
	TITLE:=Sunwait
	URL:=URL:=https://github.com/risacher/sunwait
	DEPENDS:=+libc
endef

define Package/sunwait/description
	Wait until the specified solar event
endef

define Build/Compile
	$(TARGET_CC) $(TARGET_CPPFLAGS) -o $(PKG_BUILD_DIR)/sunwait.o -c $(PKG_BUILD_DIR)/sunwait.c
	$(TARGET_CC) $(TARGET_CPPFLAGS) -o $(PKG_BUILD_DIR)/sunriset.o -c $(PKG_BUILD_DIR)/sunriset.c
	$(TARGET_CC) $(TARGET_CPPFLAGS) -o $(PKG_BUILD_DIR)/print.o -c $(PKG_BUILD_DIR)/print.c
	$(TARGET_CC) $(TARGET_LDFLAGS) -o $(PKG_BUILD_DIR)/$1 $(PKG_BUILD_DIR)/sunwait.o $(PKG_BUILD_DIR)/sunriset.o $(PKG_BUILD_DIR)/print.o -lm
endef

define Package/sunwait/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/sunwait $(1)/usr/bin/
endef

$(eval $(call BuildPackage,sunwait))

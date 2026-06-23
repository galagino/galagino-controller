DEVICE ?= 0
DEVICE_BASE := /dev/ttyUSB
DEVICE_BASE := /dev/ttyACM
DEVICE_PATH := $(DEVICE_BASE)$(DEVICE)

DEFAULT_HST := galagino-108ef4
DEFAULT_ENV := d1mini32

ENV ?= $(DEFAULT_ENV)
hostname ?= $(DEFAULT_HST)

export PLATFORMIO_CORE_DIR ?= ~/.platformio-bluepad32

ifeq ($(V),)
        VERBOSE :=
else
        VERBOSE := --verbose --jobs=1
endif

FIRMWARE := .pio/build/$(ENV)/firmware.bin

build: include/web.h
	pio run $(VERBOSE) -e $(ENV)

monitor:
	pio device monitor -e $(ENV) --port $(DEVICE_PATH)

config:
	pio project config

upload: include/web.h
	pio run $(VERBOSE) -e $(ENV) -t upload --upload-port $(DEVICE_PATH)

upload-monitor: include/web.h
	pio run $(VERBOSE) -e $(ENV) -t upload --upload-port $(DEVICE_PATH) -t monitor --monitor-port $(DEVICE_PATH)

include/web.h: assets/index.html assets/style.css assets/index.js
	$(MAKE) web_assets
	bin2c include/web.h assets/index.html.gz assets/style.css.gz assets/index.js.gz

.PHONY: web_assets
web_assets:
	gzip -9 -c assets/index.html > assets/index.html.gz
	gzip -9 -c assets/style.css  > assets/style.css.gz
	gzip -9 -c assets/index.js   > assets/index.js.gz

.PHONY: ota
ota: MD5:=$(shell md5sum .pio/build/$(ENV)/firmware.bin | awk '{print $$1}')
ota: sz:=$(shell stat --format %s .pio/build/$(ENV)/firmware.bin)
ota:
	@echo OTA firmware.bin $(sz) $(MD5) "-> $(hostname)"
	@curl -v --connect-timeout 5 --max-time 90 -qsSi -X POST -F md5=$(MD5) -F filedata=@.pio/build/$(ENV)/firmware.bin http://$(hostname)/ota


.PHONY: reboot
reboot:
	@curl -qI http://$(hostname)/reboot

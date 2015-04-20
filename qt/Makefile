include Makefile.inc

APP = C4

DIRS = src

BUILD_DIR = build

.phony: all test cucumber clean

all: test

test: cucumber

cucumber: bin/$(APP)
	cucumber

bin/$(APP): $(BUILD_DIR) bin
	$(ECHO) Commandline build in $(DESKTOP_BUILD_DIR)
	cd $(BUILD_DIR); qmake ../$(APP).pro
	cd $(BUILD_DIR); make
	cd $(BUILD_DIR); cp C4 ../bin

bin:
	mkdir -p bin

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	$(ECHO) cleaning up in ./
	rm -rf bin
	rm -rf $(BUILD_DIR)
	#for d in $(DIRS); do (cd $$d; $(MAKE) clean ); done


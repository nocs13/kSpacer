all: build

build:
	mkdir -p assets
	cp ../Data/* ./assets/.
	$(NDK_ROOT)/ndk-build -B
	ant clean && ant debug

release:
	mkdir -p assets
	cp ../Data/* ./assets/.
	$(NDK_ROOT)/ndk-build -B
	ant clean && ant release

clean:
	ant clean
	rm -rf obj libs assets/*
run:
	~/Install/android-sdk-linux/platform-tools/adb uninstall com.kgmEngine.kSpacer
	~/Install/android-sdk-linux/platform-tools/adb install bin/kSpacer-debug.apk


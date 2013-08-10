all: build

build:
	mkdir -p assets
	cp ../Data/* ./assets/.
	$(NDK_ROOT)/ndk-build -B
	ant clean && ant debug

clean:
	ant clean
	rm -rf obj libs assets/*
run:
	~/Install/android-sdk-linux/platform-tools/adb uninstall com.example.kSpacer
	~/Install/android-sdk-linux/platform-tools/adb install bin/kSpacer-debug.apk


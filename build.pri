OUTPUT_DIR=$$(OUTPUT_DIR)
linux:PLATFORM=linux
macx:PLATFORM=darwin
win32:PLATFORM=windows
isEmpty(OUTPUT_DIR):OUTPUT_DIR=$$PWD/build/$$PLATFORM
rmdir /S /Q ".\.idea"
rmdir /S /Q ".\.pioenvs"
del .\CMakeListsPrivate.txt
platformio init --ide clion --board teensy31
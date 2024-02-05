@echo off
xcopy x64\Release\VoxelMiner.exe Build\ /y
xcopy Data Build\Data\ /s /e /h /y
del Build\Data\Textures\*.kra
del Build\Data\Textures\*.kra~
del Build\Data\Textures\*.png
del Build\Data\Textures\*.png~
del Build\Data\Textures\*.exe
del Build\Data\Textures\*.bat
mkdir Build\Worlds\
echo Clean build complete
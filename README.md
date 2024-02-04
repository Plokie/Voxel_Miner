#  Voxel Miner 
### A C++ DX11 Minecraft clone
## Info
This was originally started as  university project, but I kept working on it and decided to make the code public. There's probably some iffy code in places but I'm overall pretty happy with it!

(P.S. The entire lighting system is old and bad and doesn't work, it's overdue a complete remake. Try to ignore it for now :p)

## Todo / Roadmap
You can see the roadmap at https://trello.com/b/w3MwgsMt/programming
  
## Building
Building is only intended on Windows, Visual Studio (I used Visual Studio 2022)
The only dependency required is DirectXTK

 - Download DXTK from https://github.com/microsoft/DirectXTK
 - Place folder at `../../` from the repo , ( the path should look like :

        ====/source/repos/VoxelMiner/ *[this is the repo]*
    	====/source/repos/
        ====/source/DirectXTK/ *[DirectXTKs root should be here]*
	Alternatively, change the Visual studio project properties to your desired location, but that could be messy

 - Build Desktop_2022_Win10\x64 *Debug* and *Release*. This should place it in ../../DirectXTK/bin/x64/*
 - Load the VoxelMiner Visual studio project and build either x64 Debug or x64 Release
## Packaging build as exe
I need to make this automatic at some point, but for now here are instruction on how to isolate the exe and its necessary resources from the messy build folder
Required files+folders:

    ] Data/** 										(copy the entire data folder here)
    ] x64/Release(or Debug if debug)/*.cso			(put all .cso files here, need to fix this)
    ] Worlds/										(empty worlds folder)
    ] VoxelMiner.exe								(the actual exe)
 
## Credits
 - Music: Undercover Vampire Policeman - Chris Zabriskie https://chriszabriskie.com/ (CC4.0)
 - Sound effects made using JSFXR https://sfxr.me/

## License
Under the GPL-2.0 license. Info can be seen at https://github.com/Plokie/dx11-voxel/blob/master/LICENSE
##

<3

This folder contains all DLLs needed to run the samples. If you delete this folder or rename the DLLs you will need to change for each project:

- The reference path of the libpxcclr.cs.dll;
- The "Post-build event command line" that copies the unmanaged library libpxcclr.cs.dll to the project output file.
	
As reference during the creation of new projects, this is the line that is being executed after the project build:

copy /b/y "$(SolutionDir)Libs\libpxccpp2c.dll" "$(TargetDir)"

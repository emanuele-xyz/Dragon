# Dragon

Sandbox for experimenting with graphics programming in general and [RTT](https://en.wikipedia.org/wiki/Real-time_tactics) style gameplay.

## Build

Prerequisites:
- The latest version of [Visual Studio](https://visualstudio.microsoft.com/it/) with the 'Desktop development with C++' workload.
- The latest version of [vcpkg](https://vcpkg.io). Don't forget to [set it up](https://learn.microsoft.com/vcpkg/get_started/get-started-msbuild?pivots=shell-powershell#1---set-up-vcpkg) before trying to build the game.
- All the game assets. You can download them from [here](https://mega.nz/folder/V7slVbYb#BZXlpcZbX40GSD12Cb-k0g).

The build process:

- Place the downloaded game assets inside a folder called `cwd` located at the solution's root directory.
- Open Dreagon.sln.
- Open the project's property pages > Debugging > set Working Directory to `$(SolutionDi)\cwd`.

## Minimum Requirements

- D3D11 compatible GPU.
- Windows 10 or Windows 11.

# CSC 474 Base Code
Base code for computer graphics.

#### Controls:
- P - toggle polygon mode (wireframe or solid)
- WASD - forward and lateral movement
- IKJLUO - local camera rotations along each axis (IK: x axis, JL: y axis, UO: z axis)
- SPACE - capture/hide the mouse (this allows for infinite mouse movement in each direction)
- mouse movement - If captured, the mouse position will affect the camera direction
- mouse click and drag - updates the camera direction

![Screenshot](https://github.com/calpoly-graphics/CSC-474-Base-Code/blob/master/resources/screenshot.png)

## Build Instructions
To build this project, first download and install [CMake](https://cmake.org/download/) using the binary distribution that matches your system. 

Then follow these platform-specific steps (some steps are the same for all platforms).

### Mac
1. Download [Xcode](https://itunes.apple.com/us/app/xcode/id497799835)
2. Ensure Xcode Command Line Tools are installed (Xcode->Preferences->Locations->Command Line Tools - select the highest version if the dropdown is blank)
3. Open Terminal
4. Navigate to the directory where you downloaded this project (use the "cd" command) (Pro Tip: Open Finder and drag the folder icon at the top of the window into Terminal to copy/paste the path)
5. Create a build directory ("mkdir build")
6. Navigate into the build directory ("cd build")
7. Run CMake on the commandline to generate the Xcode project ("cmake -G Xcode ..") (if you can't run CMake on the commandline, open the CMake app, and select Tools->How To Install For Command Line Use and then follow the instructions - I recommend the second option)
8. Open the newly created Xcode project
9. Change the scheme to the name of the project (Product->Scheme->lab)
10. Add a commandline argument to fix the path to the resources directory (Product->Scheme->Edit Scheme->Run->Arguments - Click the + button and add "../../resources"). This is required because the executable is run within the Debug directory within the build directory, so it is two levels deeper than the resources directory.
11. Run the project (Click the triangle button in the upper left)

### Windows
1. Download [Visual Studio](https://www.visualstudio.com/vs/) and make sure you select the "Desktop development with C++" workload when configuring what additional packages to install with Visual Studio.
2. Open Command Prompt
3. Navigate to the directory where you downloaded this project (use the "cd" command)
4. Create a build directory ("mkdir build")
5. Open the CMake GUI, click "Browse Source..." and choose the project directory, click "Browse Build..." and choose the build folder you just created.
6. In the CMake GUI, click "Configure", and in the popup select the most recent Visual Studio version that matches your system (probably the default one that it shows) and then click "Finish".
7. In the CMake GUI, click "Generate", and then close the CMake GUI
8. In the build folder, open the "lab.sln"
9. In the Solution Explorer within Visual Studio, right click on "lab" and click "Set as StartUp Project"
10. Run the project

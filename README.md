Steps to setup C (GCC) project using RayLib:
1. Download w64devkit-x.xx.x.zip from https://github.com/skeeto/w64devkit 
- Unzip to C:\w64devkit
- Add C:\w64devkit\bin to PATH
2. Create project folder
3. Download raylib-5.0_win64_mingw-w64.zip from https://github.com/raysan5/raylib/releases
- Unzip include and lib to the folder you just created
4. Create main file in project
5. Install C/C++ extention in VS Code
6. Press Ctrl+Shift+P and select C/C++: Edit configurations (UI)
- Add Include path - ${workspaceFolder}/**
7. Press Run in the right top corner of editor
- In created tasks.json add args: 
    [..., 
    "-I", "include", 
    "-L", "lib", "-lraylib",
    "-lgdi32",
    "-lwinmm"]
- Set command as "gcc"
8. Press Run
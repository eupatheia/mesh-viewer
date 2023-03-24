# mesh-viewer

Implements a simple PLY viewer

<img width="497" alt="bike" src="https://user-images.githubusercontent.com/75283980/227418303-7b1abcee-46ee-405a-bb8e-e80ea7313200.png">

## How to build

*Windows*

Open git bash to the directory containing this repository.

```
mesh-viewer $ mkdir build
mesh-viewer $ cd build
mesh-viewer/build $ cmake ..
mesh-viewer/build $ start mesh-viewer.sln
```

Your solution file should contain two projects: `mesh-viewer` and `test-ply-mesh`.
To run from the git bash command shell, 

```
mesh-viewer/build $ ../bin/Debug/test-ply-mesh.exe
mesh-viewer/build $ ../bin/Debug/mesh-viewer.exe
```

*macOS*

Open terminal to the directory containing this repository.

```
mesh-viewer $ mkdir build
mesh-viewer $ cd build
mesh-viewer/build $ cmake ..
mesh-viewer/build $ make
```

To run each program from build, you would type

```
mesh-viewer/build $ ../bin/mesh-viewer
```

## Demo of basic features

### Press 'N' to go to next model

![next](https://user-images.githubusercontent.com/75283980/227418474-fad92d59-6e2f-4a55-af5c-2385c69fcb3a.gif)

### Press 'P' to go to previous model

![previous](https://user-images.githubusercontent.com/75283980/227418529-69f88832-9736-4915-913e-6579ce060cb9.gif)

### Press 'S' to cycle through shaders

![shaders](https://user-images.githubusercontent.com/75283980/227418614-b2c5554f-e4c1-4f2c-93fa-f9857246aed6.gif)

### Mouse wheel or shift-click-drag to zoom; click-drag to move orbit camera

![zoom_and_orbit](https://user-images.githubusercontent.com/75283980/227418801-fb378955-8e8b-4ff7-aecc-9764e86f1bdc.gif)

### Normal shader

![normal](https://user-images.githubusercontent.com/75283980/227418957-5f5f8242-752e-41f5-a18f-498491335eba.gif)

### Phong vertex shader

![phong_vertex](https://user-images.githubusercontent.com/75283980/227418994-781d12b2-1973-471e-8d69-21572313887d.gif)

### Phong pixel shader

![phong_pixel](https://user-images.githubusercontent.com/75283980/227419020-f9d4a30c-b394-4802-9863-a6faab378d33.gif)

---

## Unique features 

### Texture shader (press 'T' to cycle through textures)

![textures](https://user-images.githubusercontent.com/75283980/227419585-3b26bb34-e400-4b1e-b581-ecf0a2592130.gif)

### Toon shader

![toon](https://user-images.githubusercontent.com/75283980/227419604-b07dd897-d374-4351-b59d-8a4aec3c2596.gif)

### Spotlight shader

![spotlight](https://user-images.githubusercontent.com/75283980/227419656-fdc790f8-ecf4-43fb-946a-248120487dea.gif)

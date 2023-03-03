//--------------------------------------------------
// Author: JL
// Date: 3 March 2023
// Description: Visualizes ply files in 3D,
// implements orbit camera controls
//--------------------------------------------------

#include <cmath>
#include <string>
#include <vector>
#include "agl/window.h"
#include "plymesh.h"
#include "osutils.h"

using namespace std;
using namespace glm;
using namespace agl;

class MeshViewer : public Window {
 public:
  MeshViewer() : Window() {  }

  void setup() override {
    // mesh.load("../models/cube.ply");
    modelNames = GetFilenamesInDir("../models", "ply");
    string path = "../models/";  // path to models directory
    // load all meshes
    // for (int i = 0; i < modelNames.size(); i++) {
    //   PLYMesh m(path + modelNames[i]);
    //   cout << "Loading: " << path + modelNames[i] << endl;
    //   meshes.push_back(m);
    // }
    PLYMesh m(string("../models/") + modelNames[currentModel]);
    mesh = m;
  }

  void mouseMotion(int x, int y, int dx, int dy) override {
    if (dragging) {
      float azimuth = (float) x / width() * 2.0f * M_PI;
      float elevation = (((float) y / height()) - 0.5) * M_PI;
      eyePos = radius * vec3(sin(azimuth) * cos(elevation), sin(elevation),
          cos(azimuth) * cos(elevation));  // x-axis, forward direction
      vec3 right = cross(eyePos, vec3(0, 1, 0));  // z-axis
      up = cross(eyePos, right);  // y-axis
    }
    if (scrolling) {
      eyePos /= radius;
      radius += dy / (height() * 0.05f);
      eyePos *= radius;
    }
  }

  void mouseDown(int button, int mods) override {
    cout << "Mouse DOWN" << endl;
    if (button == GLFW_MOUSE_BUTTON_LEFT && mods == 0) {
      dragging = true;
    } else if (button == GLFW_MOUSE_BUTTON_LEFT && mods == GLFW_MOD_SHIFT) {
      scrolling = true;
    }
  }

  void mouseUp(int button, int mods) override {
    cout << "Mouse UP" << endl;
    if (button == GLFW_MOUSE_BUTTON_LEFT && mods == 0) {
      dragging = false;
    } else if (button == GLFW_MOUSE_BUTTON_LEFT && mods == GLFW_MOD_SHIFT) {
      scrolling = false;
    }
  }

  void scroll(float dx, float dy) override {
    printf("Scrolled (%f, %f)\n", dx, dy);
    eyePos /= radius;
    // scrolling down is positive dy, increase radius, zoom out
    // scrolling up is negative dy, decrease radius, zoom in
    radius += dy;
    eyePos *= radius;
  }

  void keyUp(int key, int mods) override {
    if (key == GLFW_KEY_N && (mods == 0 || mods == GLFW_MOD_SHIFT)) {
      cout << "Pressed N: next model: " << endl;
      if (currentModel == modelNames.size() - 1) {
        // loop back to beginning
        currentModel = 0;
      } else {
        currentModel++;
      }
      cout << "Loading: " << currentModel << " " << modelNames[currentModel]
          << endl;
      PLYMesh m(string("../models/") + modelNames[currentModel]);
      mesh = m;
    } else if (key == GLFW_KEY_P && (mods == 0 || mods == GLFW_MOD_SHIFT)) {
      cout << "Pressed P: previous model: " << endl;
      if (currentModel == 0) {
        // loop back to end
        currentModel = modelNames.size() - 1;
      } else {
        currentModel--;
      }
      cout << "Loading: " << currentModel << " " << modelNames[currentModel]
          << endl;
      PLYMesh m(string("../models/") + modelNames[currentModel]);
      mesh = m;
    }
  }

  void draw() override {
    float aspect = ((float) width()) / height();
    renderer.perspective(glm::radians(60.0f), aspect, 0.1f, 50.0f);
    renderer.lookAt(eyePos, lookPos, up);
    // fit to 10x10x10 view volume
    vec3 maxBounds = mesh.maxBounds();
    vec3 minBounds = mesh.minBounds();
    vec3 sizes = maxBounds - minBounds;
    float factor = 10.0f / fmax((fmax(sizes.x, sizes.y)), sizes.z);
    renderer.rotate(vec3(0,0,0));
    renderer.scale(vec3(factor));
    renderer.translate(-vec3((minBounds + (sizes / 2.0f))));
    renderer.mesh(mesh);
    // renderer.cube(); // for debugging!
  }

 protected:
  PLYMesh mesh;
  std::vector<string> modelNames;
  // std::vector<PLYMesh> meshes;
  int currentModel = 0;  // index of currently loaded model in modelNames
  float radius = 10;  // radius of viewing sphere
  vec3 eyePos = vec3(radius, 0, 0);
  vec3 lookPos = vec3(0, 0, 0);
  vec3 up = vec3(0, 1, 0);
  bool dragging = false;  // if true, compute change in camPos
  bool scrolling = false;  // if true, change radius
};

int main(int argc, char** argv) {
  MeshViewer viewer;
  viewer.run();
  return 0;
}


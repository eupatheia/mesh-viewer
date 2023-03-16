//--------------------------------------------------
// Author: JL
// Date: 16 March 2023
// Description: Visualizes ply files in 3D,
// implements orbit camera controls,
// switching through models, and switching through shaders
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
    modelNames = GetFilenamesInDir("../models", "ply");
    string path = "../models/";  // path to models directory
    // load all meshes
    for (int i = 0; i < modelNames.size(); i++) {
      PLYMesh m(path + modelNames[i]);
      cout << "Loading: " << path + modelNames[i] << endl;
      meshes.push_back(m);
    }
    // load all shaders
    renderer.loadShader("normals", "../shaders/normals.vs",
        "../shaders/normals.fs");
    renderer.loadShader("phong-vertex", "../shaders/phong-vertex.vs",
        "../shaders/phong-vertex.fs");
    renderer.loadShader("phong-pixel", "../shaders/phong-pixel.vs",
        "../shaders/phong-pixel.fs");
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
    eyePos /= radius;
    // scrolling down is positive dy, increase radius, zoom out
    // scrolling up is negative dy, decrease radius, zoom in
    radius += dy;
    eyePos *= radius;
  }

  void keyUp(int key, int mods) override {
    if (key == GLFW_KEY_N && (mods == 0 || mods == GLFW_MOD_SHIFT)) {
      if (currentModel == modelNames.size() - 1) {
        // loop back to beginning
        currentModel = 0;
      } else {
        currentModel++;
      }
      cout << "Pressed N: next model: " << currentModel << " "
          << modelNames[currentModel] << endl;
    } else if (key == GLFW_KEY_P && (mods == 0 || mods == GLFW_MOD_SHIFT)) {
      if (currentModel == 0) {
        // loop back to end
        currentModel = modelNames.size() - 1;
      } else {
        currentModel--;
      }
      cout << "Pressed P: previous model: " << currentModel << " "
          << modelNames[currentModel] << endl;
    } else if (key == GLFW_KEY_S && (mods == 0 || mods == GLFW_MOD_SHIFT)) {
      if (currentShader == shaderNames.size() - 1) {
        // loop back to beginning
        currentShader = 0;
      } else {
        currentShader++;
      }
      cout << "Pressed S: next shader: " << currentShader << " "
          << shaderNames[currentShader] << endl;
    }
  }

  void draw() override {
    renderer.beginShader(shaderNames[currentShader]);
    renderer.setUniform("light.pos", vec4(20.0f,10.0f,-10.0f,1.0f));
    renderer.setUniform("light.La", vec3(1.0f));
    renderer.setUniform("light.Ld", vec3(1.0f));
    renderer.setUniform("light.Ls", vec3(1.0f));
    // blue plastic material
    renderer.setUniform("material.shine", 50.0f);
    renderer.setUniform("material.Ka", vec3(0,0,0.2f));
    renderer.setUniform("material.Kd", vec3(0,0,0.7f));
    renderer.setUniform("material.Ks", vec3(1.0f));
    // gold material
    // renderer.setUniform("material.shine", 83.2f);
    // renderer.setUniform("material.Ka", vec3(0.24725f, 0.2245f, 0.0645f));
    // renderer.setUniform("material.Kd", vec3(0.34615f, 0.3143f, 0.0903f));
    // renderer.setUniform("material.Ks", vec3(0.797357f, 0.723991f, 0.208006f));

    float aspect = ((float) width()) / height();
    renderer.perspective(glm::radians(60.0f), aspect, 0.1f, 50.0f);
    renderer.lookAt(eyePos, lookPos, up);
    // fit to 10x10x10 view volume
    vec3 maxBounds = meshes[currentModel].maxBounds();
    vec3 minBounds = meshes[currentModel].minBounds();
    vec3 sizes = maxBounds - minBounds;
    float factor = 10.0f / fmax((fmax(sizes.x, sizes.y)), sizes.z);
    renderer.rotate(vec3(0,180,0));
    renderer.scale(vec3(factor));
    renderer.translate(-vec3((minBounds + (sizes / 2.0f))));
    renderer.mesh(meshes[currentModel]);
    // renderer.cube(); // for debugging!

    renderer.endShader();
  }

 protected:
  std::vector<string> modelNames;
  std::vector<PLYMesh> meshes;
  int currentModel = 0;  // index of currently loaded model in modelNames
  std::vector<string> shaderNames = {"phong-pixel", "phong-vertex", "normals"};
  int currentShader = 0;  // index of currently loaded shader in shaderNames
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


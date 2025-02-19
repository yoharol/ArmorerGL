#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Eigen/Core>
#include <iostream>

#include "igl/readOBJ.h"
#include "ArmorerPhys/RenderCore.h"

// settings
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 800;

int main() {
  GLFWwindow *window =
      aphys::create_window(SCR_WIDTH, SCR_HEIGHT, "Example4: Pipeline");

  aphys::DiffuseMaterial material{
      {255, 255, 255},  // diffuse color
      {255, 255, 255},  // specular color
      0.5f              // specular strength
  };
  aphys::Scene scene = aphys::create_scene(
      aphys::Light{
          {242, 242, 242},     // light color
          {9, 5, 88},          // ambient color
          {0.0f, 0.35f, 5.0f}  // light position
      },
      aphys::create_camera(
          {2.0f, 0.0f, -2.0f},                  // camera position
          {0.0f, 0.35f, 0.0f},                  // camera target
          {0.0f, 1.0f, 0.0f},                   // camera up axis
          float(SCR_WIDTH) / float(SCR_HEIGHT)  // camera aspect
          ));
  aphys::Gui gui = aphys::create_gui(window, "gui");
  gui.width = 300;
  gui.height = 100;

  std::cout << gui.window_width << " " << gui.window_height << std::endl;

  aphys::Vec3f diffuse_color(0.0f, 211.f / 255.f, 239.f / 255.f);
  aphys::add_gui_func(gui, [&diffuse_color]() {
    ImGui::Text("Diffuse Color:");
    ImGui::ColorEdit3("#c1", diffuse_color.data());
  });

  aphys::MatxXf V;
  aphys::MatxXi F;
  igl::readOBJ(std::string(ASSETS_PATH) + "/spot.obj", V, F);
  int n_vertices = V.rows();
  int n_faces = F.rows();

  aphys::MatxXf v_p(4, 3);
  // select 289, 577, 572, 284 vertex from V to v_p
  v_p.row(0) = V.row(289);
  v_p.row(1) = V.row(577);
  v_p.row(2) = V.row(572);
  v_p.row(3) = V.row(284);

  aphys::Points points = aphys::create_points();
  points.color = {255, 0, 0};
  points.point_size = 10.0f;
  aphys::Lines lines = aphys::create_lines();
  lines.color = {0, 155, 155};
  lines.mode = GL_LINE_LOOP;

  aphys::set_points_data(points, v_p, aphys::MatxXf());
  aphys::set_lines_data(lines, v_p, aphys::MatxXf());

  aphys::DiffuseMesh mesh = aphys::create_diffuse_mesh(material);
  aphys::set_mesh_data(mesh, V, F);

  aphys::add_render_func(scene, aphys::get_render_func(mesh));
  aphys::add_render_func(scene, aphys::get_render_func(lines),
                         false);  // disable depth test
  aphys::add_render_func(scene, aphys::get_render_func(points),
                         false);  // disable depth test

  aphys::add_gui_mouse_input_func(gui, [&]() {
    if (ImGui::IsMouseClicked(0)) {
      ImVec2 pos = ImGui::GetMousePos();
      float x = pos.x / gui.window_width;
      float y = 1.0 - pos.y / gui.window_height;
      aphys::Vec3f ray_origin, ray_dir;
      aphys::camera_screen_to_raycast(scene.camera, x, y, ray_origin, ray_dir);
      int idx =
          aphys::raycast_face(V.cast<double>(), F, ray_origin.cast<double>(),
                              ray_dir.cast<double>());
      std::cout << "Select vert " << idx << std::endl;
    }
  });

  aphys::set_wireframe_mode(false);

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    aphys::handle_gui_input(gui);
    aphys::set_background_RGB(aphys::RGB(250, 240, 228));

    float curr_time = glfwGetTime();
    aphys::orbit_camera_control(window, scene.camera, 10.0, scene.delta_time);
    scene.light.position = aphys::Vec3f(5.0f * sin(scene.time * 1.0f), 0.35f,
                                        5.0f * cos(scene.time * 1.0f));

    aphys::render_scene(scene);
    aphys::render_gui(gui);

    aphys::use_program(mesh.program);
    aphys::set_uniform_float3(mesh.program, "diffuseColor", diffuse_color);
    aphys::unuse_program();

    glfwSwapBuffers(window);
  }
  aphys::delete_mesh(mesh);
  aphys::destroy_gui(gui);
  glfwTerminate();
  return 0;
}

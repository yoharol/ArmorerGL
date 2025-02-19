#ifndef GL_OBJS_H_
#define GL_OBJS_H_

#include "ArmorerPhys/type.h"
#include "ArmorerPhys/render/buffer.h"
#include "ArmorerPhys/render/shader.h"
#include "ArmorerPhys/render/scene.h"
#include "ArmorerPhys/geom.h"

namespace aphys {

struct TexturedMesh {
  VAO vertex_array;
  VBO vertex_buffer;
  VBO uv_buffer;
  EBO index_buffer;
  Texture texture;
  int n_faces;
  int n_vertices;
  Program program;
};

TexturedMesh create_textured_mesh(const MatxXf &vertices,
                                  const MatxXf &texCoords,
                                  const MatxXi &indices,
                                  const ShaderSource &shader,
                                  bool dynamic = false);

struct DiffuseMesh {
  VAO vertex_array;
  VBO vertex_buffer;
  VBO normal_buffer;
  EBO index_buffer;
  Program program;
  DiffuseMaterial material;
  int n_vertices;
  int n_faces;
  float alpha;
};

DiffuseMesh create_diffuse_mesh(DiffuseMaterial &material);

void set_mesh_data(DiffuseMesh &mesh, const MatxXf &V, const MatxXi &F);

RenderFunc get_render_func(DiffuseMesh &mesh);

void delete_mesh(DiffuseMesh &mesh);

struct ColorMesh {
  VAO vertex_array;
  VBO vertex_buffer;
  VBO normal_buffer;
  VBO color_buffer;
  EBO index_buffer;
  Program program;
  DiffuseMaterial material;
  int n_vertices;
  int n_faces;
};

ColorMesh create_color_mesh(DiffuseMaterial &material);

void set_color_mesh_data(ColorMesh &mesh, const MatxXf &V, const MatxXi &F,
                         const MatxXf &C);

RenderFunc get_render_func(ColorMesh &mesh);

void delete_mesh(ColorMesh &mesh);

struct Points {
  int n_points;
  VAO vertex_array;
  VBO vertex_buffer;
  VBO color_buffer;
  Program program;
  RGB color;
  bool uniform_color;
  float point_size;
};

Points create_points();

void set_points_data(Points &points, const MatxXf &points_data,
                     const MatxXf &per_point_color);

RenderFunc get_render_func(Points &points);

struct Lines {
  int n_points;
  VAO vertex_array;
  VBO vertex_buffer;
  VBO color_buffer;
  Program program;
  RGB color;
  float alpha;
  bool uniform_color;
  float width;
  GLenum mode;
};

Lines create_lines();

void set_lines_data(Lines &lines, const MatxXf &points_data,
                    const MatxXf &per_line_color);

RenderFunc get_render_func(Lines &lines);

struct Edges {
  int n_edges;
  VAO vertex_array;
  VBO vertex_buffer;
  EBO index_buffer;
  VBO color_buffer;
  Program program;
  RGB color;
  float alpha;
  bool uniform_color;
  float width;
  GLenum mode;
};

Edges create_edges();

void set_edges_data(Edges &edges, const MatxXf &points_data,
                    const Matx2i &indices, const MatxXf &per_line_color);

RenderFunc get_render_func(Edges &edges);

Lines create_deriv_lines();

void set_deriv_lines_data(Lines &lines, const MatxXf &points_data,
                          const MatxXf &deriv_data, const float deriv_scale,
                          const MatxXf &per_line_color);

Edges create_box_edges();

void set_box_edges_data(Edges &edges, const Box3d &box);

struct Triangles {
  int n_faces;
  VAO vertex_array;
  VBO vertex_buffer;
  VBO color_buffer;

  Program program;
  bool uniform_color;
  RGB color;
  float alpha;
  GLenum mode;

  MatxXf vertex_buffer_data;
  MatxXf color_buffer_data;
};

Triangles create_triangles();

void set_triangles_data(Triangles &tris, const MatxXf &points_data,
                        const Matx3i &face_indices,
                        const MatxXf &per_face_color);

RenderFunc get_render_func(Triangles &faces);

}  // namespace aphys

#endif  // GL_OBJS_H_

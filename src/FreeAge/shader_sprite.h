#pragma once

#include <memory>

#include <QOpenGLFunctions_3_2_Core>

#include "FreeAge/shader_program.h"

/// Shader for rendering sprites.
class SpriteShader {
 public:
  SpriteShader();
  ~SpriteShader();
  
  inline ShaderProgram* GetProgram() { return program.get(); }
  
  inline GLint GetTextureLocation() const { return texture_location; }
  inline GLint GetViewMatrixLocation() const { return viewMatrix_location; }
  inline GLint GetSizeLocation() const { return size_location; }
  inline GLint GetTextTopLeftLocation() const { return tex_topleft_location; }
  inline GLint GetTexBottomRightLocation() const { return tex_bottomright_location; }
  
 private:
  std::shared_ptr<ShaderProgram> program;
  GLint texture_location;
  GLint viewMatrix_location;
  GLint size_location;
  GLint tex_topleft_location;
  GLint tex_bottomright_location;
};

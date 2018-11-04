#ifndef VIEW
#define VIEW

mat4* look_at(const vec4* eye_pos, const vec4* look_at_pos, const vec4* up_vector);

mat4* frustum(GLfloat left, GLfloat right, GLfloat top, GLfloat bottom, GLfloat near, GLfloat far);

#endif

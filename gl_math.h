typedef float vec3[3];
typedef float vec4[4];
typedef float mat4[16];

float rad(float deg);

void vec3_cross(vec3 vec, const vec3 a, const vec3 b);
void vec3_normalize(vec3 vec, const vec3 a);
void vec3_sub(vec3 vec, const vec3 a, const vec3 b);

void mat_diag(mat4 mat, float a, float b, float c, float d);
void mat_look_at(mat4 mat, const vec3 camera_pos,
                 const vec3 target_pos, const vec3 up_dir);
void mat_mul(mat4 mat, const mat4 a, const mat4 b);
void mat_rotate_x(mat4 mat, float deg);
void mat_rotate_y(mat4 mat, float deg);
void mat_rotate_z(mat4 mat, float deg);
void mat_translate(mat4 mat, float a, float b, float c);

void print_mat(mat4 mat);


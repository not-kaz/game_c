#ifndef CAMERA_H
#define CAMERA_H

struct camera_direction {
	vec3 up;
	vec3 front;
	vec3 right;
};

struct camera_rotation {
	float pitch;
	float yaw;
};

struct camera_ctrl_params {
	float speed;
	float sensitivity;
};

struct camera {
	vec3 position;
	vec3 target;
	mat4 view_matrix;
	struct camera_direction direction;
	struct camera_rotation rotation;
	struct camera_ctrl_params ctrl_params;
	bool has_target;
};

int camera_init(struct camera *cam, vec3 pos, struct camera_direction dir,
		struct camera_ctrl_params ctrl_params);
void camera_finish(struct camera *cam);
void camera_update(struct camera *cam);
void camera_set_position(struct camera *cam, vec3 pos);
void camera_set_target(struct camera *cam, vec3 target);
void camera_clear_target(struct camera *cam);
void camera_set_ctrl_params(struct camera *cam,
		struct camera_ctrl_params params);

#endif

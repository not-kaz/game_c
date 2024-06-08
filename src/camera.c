#include "camera.h"

#define INITIAL_YAW -90.0f
#define INITIAL_PITCH 0.0f

int camera_init(struct camera *cam, vec3 position, float movement_speed,
		float sensitivity)
{
	if (!cam) {
		return RETURN_CODE_FAILURE;
	}
	memset(cam, 0, sizeof(struct camera));
	cam->position = position;
	cam->movement_speed = movement_speed;
	cam->sensitivity = sensitivity;
}

void camera_finish(struct camera *cam)
{

}

void camera_update(struct camera *cam)
{

}

void camera_set_target(struct camera *cam, vec3 target)
{

}

void camera_clear_target(struct camera *cam)
{

}

void camera_set_ctrl_params(struct camera *cam,
		struct camera_ctrl_params params)
{

}


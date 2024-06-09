#include <cglm/cglm.h>
#include <math.h>
#include <string.h>
#include "camera.h"
#include "common.h"

#define INITIAL_YAW -90.0f
#define INITIAL_PITCH 0.0f

//static vec3 world_up_dir = {0.0f, 1.0f, 0.0f}; // x, y, z

int camera_init(struct camera *cam, vec3 pos, struct camera_direction dir,
		struct camera_ctrl_params ctrl_params)
{
	if (!cam) {
		return RETURN_CODE_FAILURE;
	}
	memset(cam, 0, sizeof(struct camera));
	glm_vec3_copy(pos, cam->position);
	cam->direction = dir;
	cam->rotation.yaw = INITIAL_YAW;
	cam->rotation.pitch = INITIAL_PITCH;
	cam->ctrl_params.speed = ctrl_params.speed;
	cam->ctrl_params.sensitivity = ctrl_params.sensitivity;
	return RETURN_CODE_SUCCESS;
}

void camera_finish(struct camera *cam)
{
	if (!cam) {
		return;
	}
	memset(cam, 0, sizeof(struct camera));
}

void camera_update(struct camera *cam)
{
	if (!cam) {
		return;
	}
}

void camera_set_target(struct camera *cam, vec3 target)
{
	if (!cam) {
		return;
	}
	glm_vec3_copy(cam->target, target);
	cam->has_target = true;
}

void camera_clear_target(struct camera *cam)
{
	if (!cam) {
		return;
	}
	glm_vec3_zero(cam->target);
	cam->has_target = false;
}

void camera_set_ctrl_params(struct camera *cam,
		struct camera_ctrl_params params)
{
	if (!cam) {
		return;
	}
	cam->ctrl_params.speed = params.speed;
	cam->ctrl_params.sensitivity = params.sensitivity;
}

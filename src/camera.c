#include <cglm/cglm.h>
#include <math.h>
#include <string.h>
#include "camera.h"
#include "common.h"
#include "result_code.h"

#define INITIAL_YAW -90.0f
#define INITIAL_PITCH 0.0f

static vec3 world_up_dir = {0.0f, 1.0f, 0.0f}; // x, y, z

int camera_init(struct camera *cam, vec3 pos, const struct camera_direction dir,
		const struct camera_ctrl_params ctrl_params)
{
	if (cam == NULL) {
		return RESULT_CODE_NULL_PTR;
	}
	memset(cam, 0, sizeof(struct camera));
	glm_vec3_copy(pos, cam->position);
	cam->direction = dir;
	cam->rotation.yaw = INITIAL_YAW;
	cam->rotation.pitch = INITIAL_PITCH;
	cam->ctrl_params.speed = ctrl_params.speed;
	cam->ctrl_params.sensitivity = ctrl_params.sensitivity;
	return RESULT_CODE_SUCCESS;
}

int camera_finish(struct camera *cam)
{
	if (cam == NULL) {
		return RESULT_CODE_NULL_PTR;
	}
	memset(cam, 0, sizeof(struct camera));
	return RESULT_CODE_SUCCESS;
}

int camera_update(struct camera *cam)
{
	if (cam == NULL) {
		return RESULT_CODE_NULL_PTR;
	}
	if (cam->has_target) {
		glm_lookat(cam->position, cam->target, world_up_dir,
			cam->view_matrix);
	} else {
		vec3 center;

		glm_vec3_add(cam->position, cam->direction.front, center);
		glm_lookat(cam->position, center, cam->direction.up,
			cam->view_matrix);
	}
	// TODO: Finish implementation.
	return RESULT_CODE_SUCCESS;
}

int camera_set_position(struct camera *cam, vec3 pos)
{
	// REVIEW: This needs to be tested, usually I would scale with move
	//	   speed but I am trying to separate camera and player movement.
	if (cam == NULL) {
		return RESULT_CODE_NULL_PTR;
	}
	glm_vec3_copy(pos, cam->position);
	return RESULT_CODE_SUCCESS;
}

int camera_set_target(struct camera *cam, vec3 target)
{
	if (cam == NULL) {
		return RESULT_CODE_NULL_PTR;
	}
	glm_vec3_copy(cam->target, target);
	cam->has_target = true;
	return RESULT_CODE_SUCCESS;
}

int camera_clear_target(struct camera *cam)
{
	if (cam == NULL) {
		return RESULT_CODE_NULL_PTR;
	}
	glm_vec3_zero(cam->target);
	cam->has_target = false;
	return RESULT_CODE_SUCCESS;
}

int camera_set_ctrl_params(struct camera *cam,
	const struct camera_ctrl_params params)
{
	if (cam == NULL) {
		return RESULT_CODE_NULL_PTR;
	}
	cam->ctrl_params.speed = params.speed;
	cam->ctrl_params.sensitivity = params.sensitivity;
	return RESULT_CODE_SUCCESS;
}

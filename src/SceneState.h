#pragma once
#ifndef __SCENE_STATE__
#define __SCENE_STATE__
enum SceneState
{
	NO_SCENE = -1,
	START_SCENE,
	PLAY_SCENE,
	PLAY_SCENE2,
	END_SCENE,
	WIN_SCENE,
	LOSE_SCENE,
	LVL_SELECT,
	NUM_OF_SCENES
};
#endif /* defined (__SCENE_STATE__) */
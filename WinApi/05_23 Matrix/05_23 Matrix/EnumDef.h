#pragma once

enum class ComponentType {
	Transform,
	Renderer,
	RigidBody,
	Collider,
};
enum class RenderLayer {
	BASE,
	OBJECT,
};
enum class ObjectType {
	PLAYER,
	WEAPON,
	END
};

enum class KeyState {
	NONE,
	TAP,
	HOLD,
	AWAY
};

enum class DIRECTION8 {
	DIR_NONE,
	DIR_LEFT,
	DIR_RIGHT,
	DIR_UP,
	DIR_DOWN,
	DIR_UP_LEFT,
	DIR_UP_RIGHT,
	DIR_DOWN_LEFT,
	DIR_DOWN_RIGHT
};
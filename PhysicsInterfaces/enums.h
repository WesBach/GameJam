#pragma once

namespace nPhysics {
	enum IntegrationType {
		RK4,
		VERLET
	};

	enum eConstraintType {
		CONSTRAINT_TYPE_BALL_AND_SOCKET,
		CONSTRAINT_TYPE_HINGE,
		CONSTRAINT_TYPE_6DOF,
		CONSTRAINT_TYPE_SLIDER,
		CONSTRAINT_TYPE_CONE_TWIST
	};

	enum eCollisionBodyType {
		ENEMY_PROJECTILE_BODY,
		PLAYER_PROJECTILE_BODY,
		PLAYER_BODY,
		ENEMY_BODY,
		ITEM_HEALTH_BODY,
		ITEM_RANGE_BODY,
		SHIELD_BODY,
		BOSS_BODY,
		PLAYER_DUAL_PROJECTILE_BODY,
		OBSTACLE_BODY,
		RIGID_BODY,
		TREE_BODY
	};
}
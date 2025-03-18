#ifndef __DEFINE_H__
#define __DEFINE_H__

enum SCENE_ENUM {
	START,
	MAIN,
	//SHOP,
	FIELD,
	END,
};

enum CLASS_ENUM {
	NONE,
	WARRIOR,
	MAGE,
	ROGUE,
	ARCHOR
};

#define SAFE_DELETE(p) if(p){delete p;}
#define SAFE_DELETE_ARR(p) if(p){delete[] p;}


#endif // !__DEFINE_H__

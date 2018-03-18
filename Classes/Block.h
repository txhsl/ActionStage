#ifndef __BLOCK_H__
#define __BLOCK_H__

#include "cocos2d.h"  
#include "cocostudio/CocoStudio.h"

using namespace cocostudio::timeline;

USING_NS_CC;

class Block : public Sprite
{
public:
	virtual bool init();
	void setPointer(Block *role2) { _block_role = role2; }
	Block* getPointer() { return _block_role; }
	void setPicture(Sprite *pic) { _block_picture = pic; }
	Sprite* getPicture() { return _block_picture; }
	void remove();
	void get_through(Point _player_p, Block * _block_poiter);

	CREATE_FUNC(Block);
private:
	Block* _block_role; //自身指针
	Sprite* _block_picture; //图片指针
};
#endif
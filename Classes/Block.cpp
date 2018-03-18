#include "block.h"
#include "Hero.h"

bool Block::init()
{
	//执行父类的初始化方法  
	Sprite::init();

	//根据图片大小定义人物的宽高  
	Size size = Size(64, 64);
	setContentSize(size);

	//设置人物的物理属性  
	setPhysicsBody(PhysicsBody::createBox(size));

	//设置碰撞标记，使其能触发碰撞监听  
	getPhysicsBody()->setContactTestBitmask(1);

	//设置方块身份
	setTag(2);

	//防止碰撞时的翻转  
	getPhysicsBody()->setRotationEnable(false);

	//禁止方块移动
	getPhysicsBody()->setDynamic(false);
	return true;
}
void Block::remove()
{
	_block_role->getPhysicsBody()->removeFromWorld();
	_block_picture->removeFromParentAndCleanup(true);
	_block_role->removeFromParentAndCleanup(true);
}

void Block::get_through(Point _player_p, Block *_block_pointer)
{
	//穿越方块
	auto _block_y = _block_pointer->getPositionY();
	auto _block_x = _block_pointer->getPositionX();

	if (_player_p.y < _block_y && (_player_p.x - _block_x < 64 || _player_p.x - _block_x > -64))
	{
		_block_pointer->getPhysicsBody()->setEnabled(false);
	}
	else
	{
		_block_pointer->getPhysicsBody()->setEnabled(true);
	}
}
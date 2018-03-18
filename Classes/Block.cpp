#include "block.h"
#include "Hero.h"

bool Block::init()
{
	//ִ�и���ĳ�ʼ������  
	Sprite::init();

	//����ͼƬ��С��������Ŀ��  
	Size size = Size(64, 64);
	setContentSize(size);

	//�����������������  
	setPhysicsBody(PhysicsBody::createBox(size));

	//������ײ��ǣ�ʹ���ܴ�����ײ����  
	getPhysicsBody()->setContactTestBitmask(1);

	//���÷������
	setTag(2);

	//��ֹ��ײʱ�ķ�ת  
	getPhysicsBody()->setRotationEnable(false);

	//��ֹ�����ƶ�
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
	//��Խ����
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
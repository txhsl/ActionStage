#include"Hero.h"  


USING_NS_CC;

bool Hero::init()
{
	//ִ�и���ĳ�ʼ������  
	Sprite::init();

	//����ͼƬ��С��������Ŀ��  
	Size size = Size(32, 64);
	setContentSize(size);

	//��������Ķ���  
	_myNode = CSLoader::createNode("Node.csb");
	_action = CSLoader::createTimeline("Node.csb");
	_myNode->setScale(0.15f);
	_myNode->setAnchorPoint(Vec2(0.5, 0.5));
	_myNode->runAction(_action);
	_action->gotoFrameAndPlay(100, 149, true);

	//�����������������  
	setPhysicsBody(PhysicsBody::createCircle(30));

	//������ײ��ǣ�ʹ���ܴ�����ײ����  
	getPhysicsBody()->setContactTestBitmask(1);

	//������������
	setTag(1);

	//��ֹ��ײʱ�ķ�ת  
	getPhysicsBody()->setRotationEnable(false);

	//������Ծ������
	jumpReset();

	return true;
}

void Hero::moveLeft()
{
	if (_movable)
	{
		Vec2 velocity = _role->getPhysicsBody()->getVelocity();
		velocity.x = -500;
		_role->getPhysicsBody()->setVelocity(velocity);

		//������ת
		if (_direction)
		{
			_myNode->setScaleX(-0.15f);
			_direction = 0;
		}

		//�Ϳռ��
		if (fabs(velocity.y) < 0.1)
		{
			_action->gotoFrameAndPlay(300, 349, true);
		}
	}
}

void Hero::moveRight()
{
	if (_movable)
	{
		Vec2 velocity = _role->getPhysicsBody()->getVelocity();
		velocity.x = 500;
		_role->getPhysicsBody()->setVelocity(velocity);

		if (!_direction)
		{
			_myNode->setScaleX(0.15f);
			_direction = 1;
		}

		if (fabs(velocity.y) < 0.1)
		{
			_action->gotoFrameAndPlay(300, 349, true);
		}
	}
}

void Hero::slide()
{
	if (_movable)
	{
		if (fabs(_role->getPhysicsBody()->getVelocity().y) < 0.1)
		{
			_action->gotoFrameAndPlay(350, 399, false);

			Vec2 velocity = _role->getPhysicsBody()->getVelocity();
			velocity.x += velocity.x / 2;
			_role->getPhysicsBody()->setVelocity(velocity);
		}
	}
}

void Hero::jump()
{
	if (_movable)
	{
		if (_jumpTime > 0)
		{
			_action->gotoFrameAndPlay(150, 199, false);

			Vec2 velocity = _role->getPhysicsBody()->getVelocity();
			velocity.y = 500;
			_role->getPhysicsBody()->setVelocity(velocity);

			//��Ծ����
			jumpMinus();
		}
	}
}

void Hero::jumpReset()
{
	_jumpTime = 2;
}

void Hero::jumpMinus()
{
	_jumpTime--;
}

void Hero::stop()
{
	if (_movable)
	{
		Vec2 velocity = _role->getPhysicsBody()->getVelocity();
		velocity.x = 0;
		_role->getPhysicsBody()->setVelocity(velocity);

		if (fabs(velocity.y) < 0.1)
		{
			_action->gotoFrameAndPlay(100, 149, true);
		}
	}
}

void Hero::die()
{
	if (_movable)
	{
		stop();
		_action->gotoFrameAndPlay(50, 99, false);
		_movable = 0;
	}
}

void Hero::jumpJudge()
{
	if (fabs(getPhysicsBody()->getVelocity().y) > 200)
	{
		getPhysicsBody()->resetForces();
	}
}

bool Hero::attackJudge(Hero* role)
{
	if (_movable)
	{
		//���������ж�
		Vec2 velocity = _role->getPhysicsBody()->getVelocity();
		if (fabs(velocity.y) < 0.1)
		{
			attack();
		}
		else
		{
			jumpAttack();
		}

		//�����ж�
		if (_direction)
		{
			if (role->getPositionX() - getPositionX() < 80 && role->getPositionX() - getPositionX() > 0
				&& fabs(getPositionY() - role->getPositionY()) < 80)
			{
				role->die();
				return true;
			}
		}
		else
		{
			if (getPositionX() - role->getPositionX() < 80 && getPositionX() - role->getPositionX() > 0
				&& fabs(getPositionY() - role->getPositionY()) < 80)
			{
				role->die();
				return true;
			}
		}
	}
	return false;
}

bool Hero::throwJudge()
{
	if (_movable)
	{
		//���������ж�
		Vec2 velocity = _role->getPhysicsBody()->getVelocity();
		if (fabs(velocity.y) < 0.1)
		{
			Throw();
			return true;
		}
		else
		{
			jumpThrow();
			return true;
		}
	}
	return false;
}

void Hero::reset(Vec2 checkPoint)
{
	_role->setPosition(checkPoint.x, checkPoint.y);
	_role->stop();
	_movable = 1;
}

bool Enemy::init()
{
	Sprite::init();
 
	Size size = Size(32, 64);
	setContentSize(size);

	_myNode = CSLoader::createNode("Node2.csb");
	_action = CSLoader::createTimeline("Node2.csb");
	_myNode->setScale(0.15f);
	_myNode->setAnchorPoint(Vec2(0.5, 0.5));
	_myNode->runAction(_action);
	_action->gotoFrameAndPlay(100, 149, true);

	setPhysicsBody(PhysicsBody::createBox(size));
	getPhysicsBody()->setContactTestBitmask(1);
	setTag(1);
	getPhysicsBody()->setRotationEnable(false);

	jumpReset();

	return true;
}

bool Enemy::catchHero(Hero* role)
{
	//AI����
	float x = role->getPositionX();
	float y = role->getPositionY();
	Vec2 velocity = getPhysicsBody()->getVelocity();
	bool hitted = false;

	if (fabs(getPositionY() - y) < 600 && fabs(getPositionX() - x) < 600)
	{
		if (x < getPositionX() - 85)
		{
			moveLeft();
		}
		else if (x > getPositionX() + 85)
		{
			moveRight();
		}
		else
		{
			hitted = attackJudge(role);
			stop();
		}

		if (y > (getPositionY() + 200) && (velocity.x < 100 && velocity.x > -100))
		{
			jump();
		}
	}
	return hitted;
}

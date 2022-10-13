#ifndef __ENTITY_H__
#define __ENTITY_H__

class Entity
{
public:
	std::string _entityName;
	float* _max_hp;
	float* _hp;
	int* _dame;
	int* _attack_speed;
	int* _speed;
	int* _def;
};

#endif // !__ENTITY_H__

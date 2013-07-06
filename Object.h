#pragma once
#ifndef OBJECT_H
#define OBJECT_H

class Object
{
private:
	const char * name;
	const char * desc;
	bool canPickUp;

public:
	Object(const char * name, const char * desc, bool canPickUp)
	{
		this->name = name;
		this->desc = desc;
		this->canPickUp = canPickUp;
	}

	~Object()
	{
	}

	const char * getName() { return name; }
	const char * getDesc() { return desc; }
	bool getCanPickUp() { return canPickUp; }
};

#endif /* OBJECT_H */
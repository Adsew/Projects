#pragma once

// base class for all objects
class Object
{
	DECLARE_ABSTRACT_BASE_CLASS(Object);
private:
	bool m_initialized;
	std::string m_name;
	STRCODE m_nameHashCode;
	UUID m_uuid;
	STRCODE m_uuidHashCode;

public:
	Object();
	~Object();

	virtual void initialize();
	virtual void update(float _deltaTime);
	virtual void load(tinyxml2::XMLElement* element);
	bool isInitialize() const;
	const std::string& getName() const;
	const STRCODE getNameHashCode() const;
	void setName(const std::string& name);
	const STRCODE getUUIDHashCode() const;

	bool operator==(const Object& rhs) const
	{
		return m_uuidHashCode == rhs.getUUIDHashCode();
	}
	bool operator!=(const Object& rhs) const
	{
		return m_uuidHashCode == rhs.getUUIDHashCode();
	}

	bool operator<(const Object& rhs) const
	{
		return m_uuidHashCode < rhs.getUUIDHashCode();
	}

	bool operator<=(const Object& rhs) const
	{
		return m_uuidHashCode <= rhs.getUUIDHashCode();
	}

	bool operator>(const Object& rhs) const
	{
		return m_uuidHashCode > rhs.getUUIDHashCode();
	}

	bool operator>=(const Object& rhs) const
	{
		return m_uuidHashCode >= rhs.getUUIDHashCode();
	}
};


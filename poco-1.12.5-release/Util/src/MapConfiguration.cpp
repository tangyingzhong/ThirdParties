//
// MapConfiguration.cpp
//
// Library: Util
// Package: Configuration
// Module:  MapConfiguration
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Util/MapConfiguration.h"
#include <set>


namespace Poco {
namespace Util {


MapConfiguration::MapConfiguration()
{
}


MapConfiguration::~MapConfiguration()
{
}


void MapConfiguration::copyTo(AbstractConfiguration& config)
{
	AbstractConfiguration::ScopedLock lock(*this);

	for (const auto& p: _map)
	{
		config.setString(p.first, p.second);
	}
}


void MapConfiguration::clear()
{
	AbstractConfiguration::ScopedLock lock(*this);
	
	_map.clear();
}


bool MapConfiguration::getRaw(const std::string& key, std::string& value) const
{
	StringMap::const_iterator it = _map.find(key);
	if (it != _map.end())
	{
		value = it->second;
		return true;
	}
	else return false;
}


void MapConfiguration::setRaw(const std::string& key, const std::string& value)
{
	_map[key] = value;
}


void MapConfiguration::enumerate(const std::string& key, Keys& range) const
{
	std::set<std::string> keys;
	std::string prefix = key;
	if (!prefix.empty()) prefix += '.';
	std::string::size_type psize = prefix.size();
	for (const auto& p: _map)
	{
		if (p.first.compare(0, psize, prefix) == 0)
		{
			std::string subKey;
			std::string::size_type end = p.first.find('.', psize);
			if (end == std::string::npos)
				subKey = p.first.substr(psize);
			else
				subKey = p.first.substr(psize, end - psize);
			if (keys.find(subKey) == keys.end())
			{
				range.push_back(subKey);
				keys.insert(subKey);
			}
		}
	}
}


void MapConfiguration::removeRaw(const std::string& key)
{
	std::string prefix = key;
	if (!prefix.empty()) prefix += '.';
	std::string::size_type psize = prefix.size();
	StringMap::iterator it = _map.begin();
	StringMap::iterator itCur;
	while (it != _map.end())
	{
		itCur = it++;
		if ((itCur->first == key) || (itCur->first.compare(0, psize, prefix) == 0))
		{
			_map.erase(itCur);
		}
	}
}


MapConfiguration::iterator MapConfiguration::begin() const
{
	return _map.begin();
}


MapConfiguration::iterator MapConfiguration::end() const
{
	return _map.end();
}


} } // namespace Poco::Util

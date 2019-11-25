/*
  Copyright 2019 www.dev5.cn, Inc. dev5@qq.com
 
  This file is part of X-MSG-IM.
 
  X-MSG-IM is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  X-MSG-IM is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
 
  You should have received a copy of the GNU Affero General Public License
  along with X-MSG-IM.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "XmsgImGroupObjInfoMgr.h"
#include "../XmsgImGroupCfg.h"

XmsgImGroupObjInfoMgr* XmsgImGroupObjInfoMgr::inst = new XmsgImGroupObjInfoMgr();

XmsgImGroupObjInfoMgr::XmsgImGroupObjInfoMgr()
{
	this->latest4cleanCache = Xsc::clock;
	this->interval4cleanCache = 10 * DateMisc::minute;
}

XmsgImGroupObjInfoMgr* XmsgImGroupObjInfoMgr::instance()
{
	return XmsgImGroupObjInfoMgr::inst;
}

void XmsgImGroupObjInfoMgr::add(shared_ptr<XmsgImGroupObjInfoColl> coll)
{
	string str = coll->gcgt->toString();
	unique_lock<mutex> lock(this->lock4cached);
	++this->size4cached;
	auto it = this->cached.find(str);
	if (it != this->cached.end())
	{
		(*(it->second))[coll->oid] = make_pair<>(coll, DateMisc::gotoGmt0(Xsc::clock));
		return;
	}
	shared_ptr<unordered_map<string, pair<shared_ptr<XmsgImGroupObjInfoColl>, ullong>>> objInfo(new unordered_map<string, pair<shared_ptr<XmsgImGroupObjInfoColl>, ullong>>());
	(*objInfo)[coll->oid] = make_pair<>(coll, DateMisc::gotoGmt0(Xsc::clock));
	this->cached[str] = objInfo;
}

shared_ptr<XmsgImGroupObjInfoColl> XmsgImGroupObjInfoMgr::get(SptrCgt gcgt, const string& oid)
{
	unique_lock<mutex> lock(this->lock4cached);
	auto it = this->cached.find(gcgt->toString());
	if (it == this->cached.end())
		return nullptr;
	auto iter = it->second->find(oid);
	if (iter == it->second->end())
		return nullptr;
	auto coll = iter->second.first;
	iter->second.second = DateMisc::gotoGmt0(Xsc::clock);
	return coll;
}

void XmsgImGroupObjInfoMgr::dida(ullong now)
{
	if (this->latest4cleanCache + this->interval4cleanCache < now) 
		return;
	ullong ts = now - XmsgImGroupCfg::instance()->cfgPb->misc().groupobjinfolru() * DateMisc::hour;
	int count = 0;
	unique_lock<mutex> lock(this->lock4cached);
	for (auto it = this->cached.begin(); it != this->cached.end();)
	{
		shared_ptr<unordered_map<string , pair<shared_ptr<XmsgImGroupObjInfoColl>, ullong>>> objInfo = it->second;
		for (auto iter = objInfo->begin(); iter != objInfo->end();)
		{
			if (iter->second.second + ts > now) 
			{
				++iter;
				continue;
			}
			++count;
			objInfo->erase(iter++);
			--this->size4cached;
		}
		if (!objInfo->empty()) 
		{
			++it;
			continue;
		}
		this->cached.erase(it++);
	}
	if (count < 1)
		return;
	lock.unlock();
	LOG_DEBUG("have some group file-info over the cached time, we will remove them, count: %d", count)
}

int XmsgImGroupObjInfoMgr::size()
{
	return this->size4cached;
}

bool XmsgImGroupObjInfoMgr::loadCb4objInfo(shared_ptr<XmsgImGroupObjInfoColl> coll)
{
	XmsgImGroupObjInfoMgr::instance()->add(coll);
	return true;
}

XmsgImGroupObjInfoMgr::~XmsgImGroupObjInfoMgr()
{

}


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

#include "XmsgImGroupMgr.h"
#include "XmsgImGroupMemberGlocal.h"
#include "XmsgImGroupMemberUlocal.h"
#include "../XmsgImGroupCfg.h"
#include "../XmsgImGroupMisc.h"

XmsgImGroupMgr* XmsgImGroupMgr::inst = new XmsgImGroupMgr();

XmsgImGroupMgr::XmsgImGroupMgr()
{

}

XmsgImGroupMgr* XmsgImGroupMgr::instance()
{
	return XmsgImGroupMgr::inst;
}

void XmsgImGroupMgr::addGroup(shared_ptr<XmsgImGroupMemberG> group)
{
	int index = ((uchar) (group->cgt->uid.data()[group->cgt->uid.length() - 1])) % X_MSG_IM_GROUP_MGR_LOCK_COUNT;
	unique_lock<mutex> lock(this->lock4xx[index]);
	this->group[index][group->cgt->toString()] = group;
}

shared_ptr<XmsgImGroupMemberG> XmsgImGroupMgr::findGroup(const string& cgt)
{
	int index = ((uchar) (cgt.data()[cgt.length() - 1])) % X_MSG_IM_GROUP_MGR_LOCK_COUNT;
	unique_lock<mutex> lock(this->lock4xx[index]);
	auto it = this->group[index].find(cgt);
	return it == this->group[index].end() ? nullptr : it->second;
}

shared_ptr<XmsgImGroupMemberG> XmsgImGroupMgr::findGroup(SptrCgt cgt)
{
	return this->findGroup(cgt->toString());
}

size_t XmsgImGroupMgr::size4group()
{
	size_t s = 0;
	for (int i = 0; i < X_MSG_IM_GROUP_MGR_LOCK_COUNT; ++i)
	{
		unique_lock<mutex> lock(this->lock4xx[i]);
		s += this->group[i].size();
	}
	return s;
}

void XmsgImGroupMgr::addUsr(shared_ptr<XmsgImGroupMemberU> usr)
{
	int index = ((uchar) (usr->cgt->uid.data()[usr->cgt->uid.length() - 1])) % X_MSG_IM_GROUP_MGR_LOCK_COUNT;
	unique_lock<mutex> lock(this->lock4xx[index]);
	this->usr[index][usr->cgt->toString()] = usr;
}

shared_ptr<XmsgImGroupMemberU> XmsgImGroupMgr::findUsr(const string& cgt)
{
	int index = ((uchar) (cgt.data()[cgt.length() - 1])) % X_MSG_IM_GROUP_MGR_LOCK_COUNT;
	unique_lock<mutex> lock(this->lock4xx[index]);
	auto it = this->usr[index].find(cgt);
	return it == this->usr[index].end() ? nullptr : it->second;
}

shared_ptr<XmsgImGroupMemberU> XmsgImGroupMgr::findUsr(SptrCgt cgt)
{
	return this->findUsr(cgt->toString());
}

size_t XmsgImGroupMgr::size4usr()
{
	size_t s = 0;
	for (int i = 0; i < X_MSG_IM_GROUP_MGR_LOCK_COUNT; ++i)
	{
		unique_lock<mutex> lock(this->lock4xx[i]);
		s += this->usr[i].size();
	}
	return s;
}

void XmsgImGroupMgr::addUsrToUsrGroup(const string& key, shared_ptr<XmsgImGroupMemberG> group)
{
	int index = ((uchar) (key.data()[key.length() - 1])) % X_MSG_IM_GROUP_MGR_LOCK_COUNT;
	unique_lock<mutex> lock(this->lock4xx[index]);
	this->usr2usr[index][key] = group;
	lock.unlock();
	index = ((uchar) (group->cgt->uid.data()[group->cgt->uid.length() - 1])) % X_MSG_IM_GROUP_MGR_LOCK_COUNT;
	unique_lock<mutex> lockx(this->lock4xx[index]);
	this->g4usr2usr[index].insert(group->cgt->toString());
}

shared_ptr<XmsgImGroupMemberG> XmsgImGroupMgr::findUsrToUsr(const string& key)
{
	int index = ((uchar) (key.data()[key.length() - 1])) % X_MSG_IM_GROUP_MGR_LOCK_COUNT;
	unique_lock<mutex> lock(this->lock4xx[index]);
	auto it = this->usr2usr[index].find(key);
	return it == this->usr2usr[index].end() ? nullptr : it->second;
}

string XmsgImGroupMgr::genKey(SptrCgt scgt, SptrCgt dcgt)
{
	if (!scgt->isUsr())
	{
		LOG_FAULT("it`s a bug, scgt: %s, dcgt: %s", scgt->toString().c_str(), dcgt->toString().c_str())
		return "";
	}
	if (!dcgt->isUsr())
	{
		LOG_FAULT("it`s a bug, scgt: %s, dcgt: %s", scgt->toString().c_str(), dcgt->toString().c_str())
		return "";
	}
	return scgt->uid > dcgt->uid ? (scgt->toString() + "|" + dcgt->toString()) : (dcgt->toString() + "|" + scgt->toString());
}

SptrCgt XmsgImGroupMgr::chooseXmsgImGroup(SptrCgt scgt, SptrCgt dcgt)
{
	if (!scgt->isUsr() || !dcgt->isUsr())
	{
		LOG_FAULT("it`s a bug, scgt: %s, dcgt: %s", scgt->toString().c_str(), dcgt->toString().c_str())
		return nullptr;
	}
	return Misc::hash(scgt->uid) > Misc::hash(dcgt->uid) ? scgt : dcgt; 
}

SptrCgt XmsgImGroupMgr::genGcgt()
{
	return ChannelGlobalTitle::genGroup(XmsgImGroupCfg::instance()->cgt);
}

size_t XmsgImGroupMgr::size4usr2usr()
{
	size_t s = 0;
	for (int i = 0; i < X_MSG_IM_GROUP_MGR_LOCK_COUNT; ++i)
	{
		unique_lock<mutex> lock(this->lock4xx[i]);
		s += this->g4usr2usr[i].size();
	}
	return s;
}

bool XmsgImGroupMgr::loadCb4groupLocal(shared_ptr<XmsgImGroupColl> coll, list<shared_ptr<XmsgImGroupMemberInfoColl>>& lis, list<shared_ptr<XmsgImGroupMsgNotice>>& latestMsg, ullong msgIdCurrent, ullong msgIdEnd)
{
	if (coll->ver >= XmsgImGroupColl::version)
		XmsgImGroupColl::version.store(coll->ver + 1); 
	for (auto& it : lis) 
	{
		if (it->ver >= XmsgImGroupColl::version)
			XmsgImGroupColl::version.store(it->ver + 1); 
	}
	unordered_map<string , shared_ptr<list<shared_ptr<XmsgImGroupMemberInfoColl>>>> memberMap; 
	for (auto& it : lis)
	{
		shared_ptr<list<shared_ptr<XmsgImGroupMemberInfoColl>>> lisx = nullptr;
		auto iter = memberMap.find(it->mcgt->toString());
		if (iter == memberMap.end())
		{
			lisx.reset(new list<shared_ptr<XmsgImGroupMemberInfoColl>>());
			memberMap[it->mcgt->toString()] = lisx;
		}
		lisx->push_back(it);
	}
	SptrGl gl(new XmsgImGroupMemberGlocal(coll, msgIdCurrent, msgIdEnd, XmsgImGroupCfg::instance()->cfgPb->misc().groupmsgqueuelimit()));
	for (auto& it : memberMap)
	{
		it.second->sort([](auto a, auto b) 
		{
			return a->uts < b->uts;
		});
		shared_ptr<XmsgImGroupMemberInfo> info(new XmsgImGroupMemberInfo());
		auto latest = it.second->back();
		info->cgt = latest->mcgt;
		info->enable = latest->enable;
		info->info = latest->info;
		info->ver = latest->ver;
		info->gts = latest->gts;
		info->uts = latest->uts;
		for (auto& iter : *(it.second))
		{
			shared_ptr<XmsgImGroupMemberInfoHistory> histo(new XmsgImGroupMemberInfoHistory());
			histo->set_start(iter->gts);
			histo->set_end(iter->uts);
			info->history.push_back(histo);
		}
		gl->addMember(info);
		auto cgt = ChannelGlobalTitle::parse(it.first);
		if (cgt == nullptr)
		{
			LOG_FAULT("it`s a bug, can not parse channel global title: %s", it.first.c_str())
			return false;
		}
		if (cgt->isGroup()) 
			continue;
		if (XmsgImGroupMisc::isLocalHlr(cgt)) 
			continue;
	}
	gl->add4init(latestMsg); 
	XmsgImGroupMgr::instance()->addGroup(gl);
	return true;
}

bool XmsgImGroupMgr::loadCb4groupMember(shared_ptr<XmsgImGroupMemberColl> coll, list<shared_ptr<XmsgImGroupUsrClientColl>>& client)
{
	if (coll->ver > XmsgImGroupMemberColl::version)
		XmsgImGroupMemberColl::version.store(coll->ver + 1 ); 
	auto g = XmsgImGroupMgr::instance()->findGroup(coll->gcgt);
	if (g == nullptr)
	{
		LOG_FAULT("it`s a bug, can not found group for cgt: %s", coll->gcgt->toString().c_str())
		return false;
	}
	if (coll->mcgt->isGroup()) 
	{
		if (!coll->mcgt->isSameHlr(XmsgImGroupCfg::instance()->cgt)) 
		{
			LOG_FAULT("it`s a bug, must be local group: %s", coll->toString().c_str())
			return false;
		}
		SptrGl mgroup = static_pointer_cast<XmsgImGroupMemberGlocal>(XmsgImGroupMgr::instance()->findGroup(coll->mcgt));
		if (mgroup == nullptr)
		{
			LOG_FAULT("it`s a bug, can not found group for cgt: %s", coll->mcgt->toString().c_str())
			return false;
		}
		mgroup->group[coll->gcgt->toString()] = coll;
		return true;
	}
	if (!coll->mcgt->isSameHlr(XmsgImGroupCfg::instance()->hlr))
	{
		LOG_FAULT("it`s a bug, must be local group usr: %s, x-msg-im-hlr: %s", coll->toString().c_str(), XmsgImGroupCfg::instance()->hlr->toString().c_str())
		return false;
	}
	auto ul = static_pointer_cast<XmsgImGroupMemberUlocal>(XmsgImGroupMgr::instance()->findUsr(coll->mcgt));
	if (ul == nullptr) 
	{
		ul.reset(new XmsgImGroupMemberUlocal(coll->mcgt));
		ul->initAdd2cache(coll, client);
		return true;
	}
	ul->group[coll->gcgt->toString()] = coll;
	return true;
}

bool XmsgImGroupMgr::loadCb4usrToUsr(shared_ptr<XmsgImGroupUsrToUsrColl> coll)
{
	auto group = XmsgImGroupMgr::instance()->findGroup(coll->gcgt);
	if (group == nullptr)
	{
		LOG_FAULT("it`s a bug, can not found usr to usr group for cgt, coll: %s", coll->toString().c_str())
		return false;
	}
	XmsgImGroupMgr::instance()->addUsrToUsrGroup(coll->key, group);
	return true;
}

void XmsgImGroupMgr::initXmsgImGroupMemberUlocal(SptrCgt cgt, function<void(int ret, const string& desc, SptrUl ul)> cb)
{
	this->initXmsgImGroupMemberUlocalcb(cgt, cb);
}

void XmsgImGroupMgr::setInitXmsgImGroupMemberUlocalcb(function<void(SptrCgt cgt, function<void(int ret, const string& desc, SptrUl ul)> cb)> cb)
{
	this->initXmsgImGroupMemberUlocalcb = cb;
}

XmsgImGroupMgr::~XmsgImGroupMgr()
{

}


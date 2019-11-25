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

#include "XmsgImGroupMemberUlocal.h"
#include "XmsgImGroupMemberG.h"
#include "XmsgImGroupMgr.h"
#include "../usr/XmsgImClientLocalMgr.h"

XmsgImGroupMemberUlocal::XmsgImGroupMemberUlocal(SptrCgt cgt) :
		XmsgImGroupMemberU(cgt)
{

}

void XmsgImGroupMemberUlocal::addClient(SptrClientLocal client)
{
	string platDid = XmsgMisc::makePlatDid(client->plat, client->did);
	auto it = this->client.find(platDid);
	if (it != this->client.end())
	{
		it->second = client;
		return;
	}
	XmsgImClientLocalMgr::instance()->addUsrClient(client, [client](int ret, const string& desc) 
	{
		if(ret == RET_SUCCESS)
		{
			LOG_DEBUG("got a new x-msg-im-client on local group usr, and save to database successful, client: %s", client->toString().c_str())
			return;
		}
		LOG_WARN("got a new x-msg-im-client on local group usr, and save to database failed, ret: %04X, desc: %s, client: %s", ret, desc.c_str(), client->toString().c_str())
	});
	this->client[platDid] = client;
}

void XmsgImGroupMemberUlocal::pubNotice2onlineClient(const string& slot , shared_ptr<Message> notice, SptrClientLocal ignore)
{
	SptrUl ul = static_pointer_cast<XmsgImGroupMemberUlocal>(this->shared_from_this());
	for (auto& it : this->client)
	{
		auto client = it.second.lock();
		if (client == nullptr) 
			continue;
		if (ignore != nullptr && (it.first == XmsgMisc::makePlatDid(ignore->plat, ignore->did))) 
			continue;
		client->sendNotice(ul, slot, notice);
	}
}

void XmsgImGroupMemberUlocal::pubNotice2offlineClient(shared_ptr<Message> notice, shared_ptr<unordered_set<string>> ignore)
{
	for (auto& it : this->client)
	{
		if (it.second.lock() != nullptr) 
			continue;
		if (ignore == nullptr ? false : ignore->find(it.first) != ignore->end()) 
			continue;
		LOG_ERROR("offline push incomplete.")
	}
}

void XmsgImGroupMemberUlocal::pubNotice2offlineClient(shared_ptr<Message> notice, SptrClient ignore, bool persistence)
{
	LOG_ERROR("offline push incomplete.")
}

void XmsgImGroupMemberUlocal::pubXmsgImGroupSyncNotice(shared_ptr<XmsgImGroupMemberG> g, shared_ptr<XmsgImGroupSyncNotice> notice, SptrClient ignore)
{
	string platDid = ignore == nullptr ? "" : XmsgMisc::makePlatDid(ignore->plat, ignore->did);
	SptrUl ul = static_pointer_cast<XmsgImGroupMemberUlocal>(this->shared_from_this());
	for (auto& it : this->client)
	{
		SptrClientLocal client = it.second.lock();
		if (client == nullptr) 
		{
			this->pubNotice2offlineClient(notice, ignore, MSG_NOT_PERSISTENCE );
			continue;
		}
		if (!client->subGroupEvent) 
			continue;
		if (ignore != nullptr && (it.first == platDid)) 
			continue;
		g->future([g, client] 
		{
			LOG_DEBUG("we will subscribe this group`s event and message for this member`s client, group: %s, client: %s", g->toString().c_str(), client->toString().c_str())
			g->subEvn(client);
			g->subMsg(client);
		});
		client->sendNotice(ul, notice->event().cgt(), notice);
	}
}

shared_ptr<XmsgImGroupMemberColl> XmsgImGroupMemberUlocal::inGroupInfo(const string& gcgt)
{
	auto it = this->group.find(gcgt);
	return it == this->group.end() ? nullptr : it->second;
}

void XmsgImGroupMemberUlocal::clientLost(const string& plat, const string& did)
{
	this->client[XmsgMisc::makePlatDid(plat, did)] = weak_ptr<XmsgImClientLocal>(); 
}

void XmsgImGroupMemberUlocal::initAdd2cache(shared_ptr<XmsgImGroupMemberColl> coll, list<shared_ptr<XmsgImGroupUsrClientColl>>& client)
{
	SptrUl ul = static_pointer_cast<XmsgImGroupMemberUlocal>(this->shared_from_this());
	this->group[coll->gcgt->toString()] = coll; 
	for (auto& it : client) 
	{
		shared_ptr<XmsgAp> apLocal = nullptr;
		SptrClientLocal c(new XmsgImClientLocal(it->plat, it->did, ul, "", apLocal));
		string platDid = XmsgMisc::makePlatDid(it->plat, it->did);
		if (this->client.find(platDid) != this->client.end())
		{
			LOG_FAULT("it`s a bug, duplicate x-msg-im-client for this local group usr, this: %s, client: %s", this->toString().c_str(), it->toString().c_str())
		}
		this->client[platDid] = c;
	}
	XmsgImGroupMgr::instance()->addUsr(ul); 
}

string XmsgImGroupMemberUlocal::toString()
{
	string str;
	SPRINTF_STRING(&str, "cgt: %s", this->cgt->toString().c_str())
	return str;
}

XmsgImGroupMemberUlocal::~XmsgImGroupMemberUlocal()
{

}


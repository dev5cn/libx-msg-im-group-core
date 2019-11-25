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

#include "XmsgImGroupMemberG.h"
#include "XmsgImGroupMemberUlocal.h"
#include "XmsgImGroupMgr.h"
#include "../XmsgImGroupCfg.h"
#include "../XmsgImGroupMisc.h"
#include "../usr/XmsgImClientLocalMgr.h"

XmsgImGroupMemberG::XmsgImGroupMemberG(shared_ptr<XmsgImGroupColl> dat, size_t limit4msgQueue) :
		XmsgImGroupMember(dat->cgt)
{
	this->dat = dat;
	this->limit4msgQueue = limit4msgQueue;
}

void XmsgImGroupMemberG::addMember(shared_ptr<XmsgImGroupMemberInfo> member)
{
	string cgt = member->cgt->toString();
	this->member[cgt] = member;
	if (member->cgt->isUsr())
	{
		if (XmsgImGroupMisc::isLocalHlr(member->cgt)) 
		{
			this->memberLocalUsr[cgt] = member;
			return;
		}
		return;
	}
	if (XmsgImGroupMisc::isLocalGroup(member->cgt))
	{
		this->memberLocalGroup[cgt] = member;
		return;
	}
}

void XmsgImGroupMemberG::delMember(SptrCgt cgt)
{
	LOG_FAULT("it`s a bug, incomplete")
}

shared_ptr<XmsgImGroupMemberInfo> XmsgImGroupMemberG::isMember(SptrCgt mcgt)
{
	auto it = this->member.find(mcgt->toString());
	return it == this->member.end() ? nullptr : (it->second->enable ? it->second : nullptr);
}

shared_ptr<XmsgImGroupMemberInfo> XmsgImGroupMemberG::findMember(SptrCgt mcgt)
{
	auto it = this->member.find(mcgt->toString());
	return it == this->member.end() ? nullptr : it->second;
}

void XmsgImGroupMemberG::addMsgNotice(shared_ptr<XmsgImGroupMsgNotice> msg)
{
	this->msgQueue.push_back(msg);
	if (this->msgQueue.size() > this->limit4msgQueue) 
	{
		LOG_TRACE("message queue over the limit, we will pop a one, queue-size: %zu, limit: %zu, this: %s", this->msgQueue.size(), this->limit4msgQueue, this->toString().c_str())
		this->msgQueue.pop_front();
	}
}

void XmsgImGroupMemberG::pubMsgNotice2localUsr(shared_ptr<XmsgImGroupMsgNotice> msg, SptrClient ignore)
{
	unordered_map<string , shared_ptr<unordered_set<string >>> sent; 
	if (!this->sub4msg.empty())
	{
		this->pub2localSub(msg, this->sub4msg, sent, ignore); 
	} else
	{
		LOG_TRACE("no member`s client subscribe this group`s message, this: %s", this->toString().c_str())
	}
	this->pub2localOfflineMember(msg, sent); 
}

void XmsgImGroupMemberG::pubMsgNotice2localGroup(shared_ptr<XmsgImGroupMsgNotice> msg)
{
	LOG_FAULT("it`s a bug, incomplete")
}

weak_ptr<XmsgImClientLocal> XmsgImGroupMemberG::subMsg(SptrClientLocal client )
{
	string cgt = client->cgt->toString();
	auto it = this->sub4msg.find(cgt);
	shared_ptr<unordered_map<string, weak_ptr<XmsgImClientLocal>>> map;
	if (it == this->sub4msg.end())
	{
		map.reset(new unordered_map<string, weak_ptr<XmsgImClientLocal>>());
		this->sub4msg[cgt] = map;
		map->insert(make_pair<>(XmsgMisc::makePlatDid(client->plat, client->did), client));
		return weak_ptr<XmsgImClientLocal>();
	}
	map = it->second;
	auto iter = map->find(XmsgMisc::makePlatDid(client->plat, client->did));
	if (iter != map->end())
	{
		auto old = iter->second;
		iter->second = client;
		return old;
	}
	return weak_ptr<XmsgImClientLocal>();
}

weak_ptr<XmsgImClientLocal> XmsgImGroupMemberG::subEvn(SptrClientLocal client )
{
	string cgt = client->cgt->toString();
	auto it = this->sub4evn.find(cgt);
	shared_ptr<unordered_map<string, weak_ptr<XmsgImClientLocal>>> map;
	if (it == this->sub4evn.end())
	{
		map.reset(new unordered_map<string, weak_ptr<XmsgImClientLocal>>());
		this->sub4evn[cgt] = map;
		map->insert(make_pair<>(XmsgMisc::makePlatDid(client->plat, client->did), client));
		return weak_ptr<XmsgImClientLocal>();
	}
	map = it->second;
	auto iter = map->find(XmsgMisc::makePlatDid(client->plat, client->did));
	if (iter != map->end())
	{
		auto old = iter->second;
		iter->second = client;
		return old;
	}
	return weak_ptr<XmsgImClientLocal>();
}

void XmsgImGroupMemberG::pubEvn2localUsr(shared_ptr<Message> evn, SptrClientLocal ignore)
{
	unordered_map<string , shared_ptr<unordered_set<string >>> sent;
	if (!this->sub4evn.empty())
	{
		this->pub2localSub(evn, this->sub4evn, sent, ignore); 
	} else
	{
		LOG_TRACE("no member`s client subscribe this group`s event, this: %s", this->toString().c_str())
	}
	this->pub2localOfflineMember(evn, sent); 
}

shared_ptr<XmsgImGroupMsgSyncRsp> XmsgImGroupMemberG::makeXmsgImGroupMsgSyncRsp(shared_ptr<XmsgImGroupMsgSyncReq> req)
{
	shared_ptr<XmsgImGroupMsgSyncRsp> rsp(new XmsgImGroupMsgSyncRsp());
	size_t i = 0;
	for (auto it = this->msgQueue.rbegin(); it != this->msgQueue.rend() && i < this->limit4msgQueue; ++it) 
	{
		shared_ptr<XmsgImGroupMsgNotice> notice = *it;
		if (notice->msgid() <= req->msgid()) 
			continue;
		XmsgImGroupMsgHistory* histo = rsp->add_msg();
		histo->set_scgt(notice->scgt());
		histo->set_msgid(notice->msgid());
		histo->mutable_msg()->CopyFrom(notice->msg());
		histo->set_gts(notice->gts());
		++i;
	}
	return rsp;
}

void XmsgImGroupMemberG::pubEvnGroupCreate(SptrClient ignore, function<void()> cb)
{
	shared_ptr<atomic_int> refCounter(new atomic_int(this->member.size()));
	this->pubEvnGroupCreate2localUsr(ignore, refCounter, cb); 
	this->pubEvnGroupCreate2localGroup(ignore); 
}

void XmsgImGroupMemberG::pubEvnGroupCreate2localUsr(SptrClient ignore, shared_ptr<atomic_int> refCounter, function<void()> cb)
{
	shared_ptr<XmsgImGroupMemberG> g = static_pointer_cast<XmsgImGroupMemberG>(this->shared_from_this());
	for (auto& it : this->memberLocalUsr)
	{
		shared_ptr<XmsgImGroupMemberUlocal> ul = static_pointer_cast<XmsgImGroupMemberUlocal>(XmsgImGroupMgr::instance()->findUsr(it.first));
		if (ul == nullptr)
		{
			LOG_FAULT("it`s a bug, can not found group local member for cgt: %s", it.first.c_str())
			if (refCounter->fetch_sub(1) == 1)
			{
				LOG_DEBUG("the all local usr already subscribed the event and message on this group, this: %s", this->toString().c_str())
				cb();
			}
			continue;
		}
		shared_ptr<XmsgImGroupColl> gdat = this->dat;
		ul->future([g, gdat, ul, ignore, refCounter, cb]
		{
			string gcgt = gdat->cgt->toString();
			shared_ptr<XmsgImGroupMemberColl> gm = ul->inGroupInfo(gcgt);
			if (gm == nullptr) 
			{
				LOG_FAULT("it`s a bug , current usr is not in this group, gcgt: %s, mcgt: %s", gcgt.c_str(), ul->cgt->toString().c_str())
				if (refCounter->fetch_sub(1) == 1)
				{
					g->future([g, cb]
							{
								LOG_DEBUG("the all local usr already subscribed the event and message on this group, this: %s", g->toString().c_str())
								cb();
							});
				}
				return;
			}
			shared_ptr<XmsgImGroupSyncNotice> notice(new XmsgImGroupSyncNotice());
			auto evn = notice->mutable_event();
			evn->set_cgt(gcgt);
			evn->set_oper("insert");
			*(evn->mutable_info()) = gdat->info->kv();
			evn->set_ver4usr(gm->ver);
			evn->set_gts4usr(gm->gts);
			evn->set_uts4usr(gm->uts);
			evn->set_gts4group(gdat->gts);
			ul->pubXmsgImGroupSyncNotice(g, notice, (ignore != nullptr && ignore->cgt->isSame(ul->cgt) ) ? ignore : nullptr);
			if (refCounter->fetch_sub(1) == 1)
			{
				g->future([g, cb]
						{
							LOG_DEBUG("the all local usr already subscribed the event and message on this group, this: %s", g->toString().c_str())
							cb();
						});
			}
		});
	}
}

void XmsgImGroupMemberG::pubEvnGroupCreate2localGroup(SptrClient ignore)
{
	LOG_FAULT("it`s a bug, incomplete")
}

void XmsgImGroupMemberG::pubEvnGroupCreateUsrToUsr(SptrClient ignore, function<void()> cb)
{
	shared_ptr<atomic_int> refCounter(new atomic_int(this->member.size()));
	this->pubEvnGroupCreate2localUsr(ignore, refCounter, cb); 
}

void XmsgImGroupMemberG::pub2localSub(shared_ptr<Message> evn, unordered_map<string, shared_ptr<unordered_map<string, weak_ptr<XmsgImClientLocal>>>>& sub4xx, unordered_map<string, shared_ptr<unordered_set<string>>>& sent, SptrClient ignore)
{
	SptrCgt gcgt = this->cgt;
	for (auto it = sub4xx.begin(); it != sub4xx.end();) 
	{
		shared_ptr<unordered_map<string , weak_ptr<XmsgImClientLocal>>> sub = it->second;
		for (auto iter = sub->begin(); iter != sub->end();)
		{
			SptrClientLocal client = iter->second.lock();
			if (client == nullptr)
			{
				LOG_DEBUG("x-msg-im-client channel lost, we will remove it, gcgt: %s, mcgt: %s, plat-did: %s", this->cgt->toString().c_str(), it->first.c_str(), iter->first.c_str())
				sub->erase(iter++); 
				continue;
			}
			if (ignore != nullptr && ignore->equals(client)) 
			{
				shared_ptr<unordered_set<string>> set(new unordered_set<string>());
				set->insert(XmsgMisc::makePlatDid(ignore->plat, ignore->did));
				sent[ignore->u.lock()->cgt->toString()] = set;
				++iter;
				continue;
			}
			auto ul = static_pointer_cast<XmsgImGroupMemberUlocal>(client->u.lock());
			if (ul == nullptr)
			{
				LOG_FAULT("it`s a bug, usr local can not be null, client: %s, notice: %s", client->toString().c_str(), evn->ShortDebugString().c_str())
				continue;
			}
			ul->future([ul, client, gcgt, evn]
			{
				LOG_TRACE("send message notice to subscriber, client: %s, msg: %s", client->toString().c_str(), evn->ShortDebugString().c_str())
				client->sendNotice(ul, gcgt->toString(), evn); 
			});
			auto itx = sent.find(it->first );
			if (itx == sent.end())
			{
				shared_ptr<unordered_set<string>> set(new unordered_set<string>());
				set->insert(XmsgMisc::makePlatDid(client->plat, client->did));
				sent[it->first] = set;
			} else
				itx->second->insert(XmsgMisc::makePlatDid(client->plat, client->did));
			++iter;
		}
		if (sub->empty()) 
		{
			LOG_DEBUG("x-msg-im-usr all client lost, we will remove it, gcgt: %s, mcgt: %s", this->cgt->toString().c_str(), it->first.c_str())
			sub4xx.erase(it++);
			continue;
		}
		++it;
	}
}

void XmsgImGroupMemberG::pub2localOfflineMember(shared_ptr<Message> evn, unordered_map<string, shared_ptr<unordered_set<string>>>& ignore )
{
	for (auto& it : this->memberLocalUsr)
	{
		shared_ptr<XmsgImGroupMemberInfo> m = it.second;
		if (!m->enable) 
			continue;
		shared_ptr<XmsgImGroupMemberUlocal> ul = static_pointer_cast<XmsgImGroupMemberUlocal>(XmsgImGroupMgr::instance()->findUsr(m->cgt));
		if (ul == nullptr)
		{
			LOG_FAULT("it`s a bug, can not found group local member for cgt: %s", m->cgt->toString().c_str())
			continue;
		}
		auto set = ignore[it.first];
		ul->future([ul, evn, set]
		{
			ul->pubNotice2offlineClient(evn, set);
		});
	}
}

void XmsgImGroupMemberG::add4init(list<shared_ptr<XmsgImGroupMsgNotice>>& latestMsg)
{
	latestMsg.sort([](auto a, auto b) 
	{
		return a->gts() < b->gts();
	});
	for (auto& it : latestMsg)
		this->msgQueue.push_back(it);
}

int XmsgImGroupMemberG::msgQueueSize()
{
	return this->msgQueue.size();
}

XmsgImGroupMemberG::~XmsgImGroupMemberG()
{

}

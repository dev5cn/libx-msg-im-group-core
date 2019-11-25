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

#ifndef GM_XMSGIMGROUPMEMBERG_H_
#define GM_XMSGIMGROUPMEMBERG_H_

#include <libx-msg-common-dat-struct-cpp.h>
#include "XmsgImGroupMember.h"
#include "XmsgImGroupMemberInfo.h"
#include "../coll/XmsgImGroupColl.h"
#include "../usr/XmsgImClientLocal.h"

class XmsgImGroupMemberG: public XmsgImGroupMember
{
public:
	shared_ptr<XmsgImGroupColl> dat; 
	unordered_map<string , shared_ptr<XmsgImGroupMemberInfo> > member; 
	unordered_map<string , shared_ptr<XmsgImGroupMemberInfo> > memberLocalUsr; 
	unordered_map<string , shared_ptr<XmsgImGroupMemberInfo> > memberLocalGroup; 
	unordered_map<string , shared_ptr<unordered_map<string , shared_ptr<XmsgImGroupMemberInfo> >>> memberForeignUsr; 
	unordered_map<string , shared_ptr<unordered_map<string , shared_ptr<XmsgImGroupMemberInfo> >>> memberForeignGroup; 
public:
	void addMember(shared_ptr<XmsgImGroupMemberInfo> member); 
	void delMember(SptrCgt cgt); 
	shared_ptr<XmsgImGroupMemberInfo> findMember(SptrCgt mcgt); 
	shared_ptr<XmsgImGroupMemberInfo> isMember(SptrCgt mcgt); 
public:
	void addMsgNotice(shared_ptr<XmsgImGroupMsgNotice> msg); 
	void pubMsgNotice2localUsr(shared_ptr<XmsgImGroupMsgNotice> msg, SptrClient ignore); 
	void pubMsgNotice2localGroup(shared_ptr<XmsgImGroupMsgNotice> msg); 
public:
	weak_ptr<XmsgImClientLocal> subMsg(SptrClientLocal client ); 
	weak_ptr<XmsgImClientLocal> subEvn(SptrClientLocal client ); 
	void pubEvn2localUsr(shared_ptr<Message> evn, SptrClientLocal ignore = nullptr); 
	shared_ptr<XmsgImGroupMsgSyncRsp> makeXmsgImGroupMsgSyncRsp(shared_ptr<XmsgImGroupMsgSyncReq> req); 
public:
	void pubEvnGroupCreate(SptrClient ignore, function<void()> cb); 
	void pubEvnGroupCreate2localUsr(SptrClient ignore, shared_ptr<atomic_int> refCounter, function<void()> cb); 
	void pubEvnGroupCreate2localGroup(SptrClient ignore); 
	void pubEvnGroupCreateUsrToUsr(SptrClient ignore, function<void()> cb); 
public:
	list<shared_ptr<XmsgImGroupMsgNotice>> msgQueue; 
	size_t limit4msgQueue; 
	unordered_map<string , shared_ptr<unordered_map<string , weak_ptr<XmsgImClientLocal>>>> sub4msg; 
	unordered_map<string , shared_ptr<unordered_map<string , weak_ptr<XmsgImClientLocal>>>> sub4evn; 
private:
	void pub2localSub(shared_ptr<Message> evn, unordered_map<string, shared_ptr<unordered_map<string, weak_ptr<XmsgImClientLocal>>>>& sub4xx, unordered_map<string, shared_ptr<unordered_set<string>>>& sent, SptrClient ignore = nullptr); 
	void pub2localOfflineMember(shared_ptr<Message> evn, unordered_map<string, shared_ptr<unordered_set<string>>>& ignore); 
public:
	void add4init(list<shared_ptr<XmsgImGroupMsgNotice>>& latestMsg); 
	int msgQueueSize(); 
	XmsgImGroupMemberG(shared_ptr<XmsgImGroupColl> dat, size_t limit4msgQueue);
	virtual ~XmsgImGroupMemberG();
};

typedef shared_ptr<XmsgImGroupMemberG> SptrGroup;

#endif 

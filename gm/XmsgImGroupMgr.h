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

#ifndef GM_XMSGIMGROUPMGR_H_
#define GM_XMSGIMGROUPMGR_H_

#include "XmsgImGroupMemberG.h"
#include "XmsgImGroupMemberUlocal.h"
#include "../coll/XmsgImGroupColl.h"
#include "../coll/XmsgImGroupMemberColl.h"
#include "../coll/XmsgImGroupMemberInfoColl.h"
#include "../coll/XmsgImGroupUsrClientColl.h"
#include "../coll/XmsgImGroupUsrToUsrColl.h"

#define X_MSG_IM_GROUP_MGR_LOCK_COUNT																0x40	

class XmsgImGroupMgr
{
public:
	void addGroup(shared_ptr<XmsgImGroupMemberG> group); 
	shared_ptr<XmsgImGroupMemberG> findGroup(const string& cgt); 
	shared_ptr<XmsgImGroupMemberG> findGroup(SptrCgt cgt); 
	size_t size4group(); 
public:
	void addUsr(shared_ptr<XmsgImGroupMemberU> usr); 
	shared_ptr<XmsgImGroupMemberU> findUsr(const string& cgt); 
	shared_ptr<XmsgImGroupMemberU> findUsr(SptrCgt cgt); 
	size_t size4usr(); 
public:
	void addUsrToUsrGroup(const string& key, shared_ptr<XmsgImGroupMemberG> group); 
	shared_ptr<XmsgImGroupMemberG> findUsrToUsr(const string& key); 
	string genKey(SptrCgt scgt, SptrCgt dcgt); 
	SptrCgt chooseXmsgImGroup(SptrCgt scgt, SptrCgt dcgt); 
	SptrCgt genGcgt(); 
	size_t size4usr2usr(); 
public:
	static bool loadCb4groupLocal(shared_ptr<XmsgImGroupColl> coll, list<shared_ptr<XmsgImGroupMemberInfoColl>>& lis, list<shared_ptr<XmsgImGroupMsgNotice>>& latestMsg, ullong msgIdCurrent, ullong msgIdEnd); 
	static bool loadCb4groupMember(shared_ptr<XmsgImGroupMemberColl> coll, list<shared_ptr<XmsgImGroupUsrClientColl>>& client); 
	static bool loadCb4usrToUsr(shared_ptr<XmsgImGroupUsrToUsrColl> coll); 
	void setInitXmsgImGroupMemberUlocalcb(function<void(SptrCgt cgt, function<void(int ret, const string& desc, SptrUl ul)> cb)> cb); 
	void initXmsgImGroupMemberUlocal(SptrCgt cgt, function<void(int ret, const string& desc, SptrUl ul)> cb); 
	static XmsgImGroupMgr* instance();
private:
	unordered_map<string , shared_ptr<XmsgImGroupMemberG>> group[X_MSG_IM_GROUP_MGR_LOCK_COUNT]; 
	unordered_map<string , shared_ptr<XmsgImGroupMemberU>> usr[X_MSG_IM_GROUP_MGR_LOCK_COUNT]; 
	unordered_map<string , shared_ptr<XmsgImGroupMemberG>> usr2usr[X_MSG_IM_GROUP_MGR_LOCK_COUNT]; 
	unordered_set<string > g4usr2usr[X_MSG_IM_GROUP_MGR_LOCK_COUNT]; 
	mutex lock4xx[X_MSG_IM_GROUP_MGR_LOCK_COUNT];
	function<void(SptrCgt cgt, function<void(int ret, const string& desc, SptrUl ul)> cb)> initXmsgImGroupMemberUlocalcb; 
	static XmsgImGroupMgr* inst;
	XmsgImGroupMgr();
	virtual ~XmsgImGroupMgr();
};

#endif 

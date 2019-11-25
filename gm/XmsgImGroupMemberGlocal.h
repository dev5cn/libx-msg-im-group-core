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

#ifndef GM_XMSGIMGROUPMEMBERGLOCAL_H_
#define GM_XMSGIMGROUPMEMBERGLOCAL_H_

#include "XmsgImGroupMemberG.h"
#include "../coll/XmsgImGroupMemberColl.h"
#include "../coll/XmsgImGroupMsgColl.h"

class XmsgImGroupMemberGlocal: public XmsgImGroupMemberG
{
public:
	unordered_map<string , shared_ptr<XmsgImGroupMemberColl> > group; 
public:
	ullong getMsgId(); 
	ullong getMsgIdCurrent(); 
	bool isEnable(const string& gcgt); 
	void saveMsg(SptrGroupMsg msg, function<void(int ret, const string& desc)> cb); 
	void applyMsgid(function<void(shared_ptr<XmsgImGroupMemberGlocal> gl, ullong msgIdEnd)> cb); 
	static void setSaveMsgCb(function<void(SptrGroupMsg msg, function<void(int ret, const string& desc)> cb)> cb); 
	static void setApplyMsgidCb(function<void(shared_ptr<XmsgImGroupMemberGlocal> gl, function<void(ullong msgIdEnd)> cb)> cb); 
	string toString();
	XmsgImGroupMemberGlocal(shared_ptr<XmsgImGroupColl> dat, ullong msgIdCurrent, ullong msgIdEnd, size_t limit4msgQueue);
	virtual ~XmsgImGroupMemberGlocal();
private:
	bool isApplyMsgid; 
	ullong msgIdCurrent; 
	ullong msgIdEnd; 
	static function<void(SptrGroupMsg msg, function<void(int ret, const string& desc)> cb)> saveMsgCb; 
	static function<void(shared_ptr<XmsgImGroupMemberGlocal> gl, function<void(ullong msgIdEnd)> cb)> applyMsgidCb; 
};

typedef shared_ptr<XmsgImGroupMemberGlocal> SptrGl;

#endif 

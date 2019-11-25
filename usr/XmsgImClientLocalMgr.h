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

#ifndef USR_XMSGIMCLIENTLOCALMGR_H_
#define USR_XMSGIMCLIENTLOCALMGR_H_

#include "XmsgImClientLocal.h"
#include "../gm/XmsgImGroupMemberG.h"

#define X_MSG_IM_CLIENT_MGR_LOCK_COUNT																0x40	

class XmsgImClientLocalMgr
{
public:
	void add(SptrClientLocal client); 
	SptrClientLocal find(const string& apCcid); 
	SptrClientLocal remove(const string& apCcid); 
	void setAddUsrClientCb(function<void(SptrClientLocal client, function<void(int ret, const string& desc)> cb)> cb); 
	void addUsrClient(SptrClientLocal client, function<void(int ret, const string& desc)> cb); 
	static XmsgImClientLocalMgr* instance();
private:
	unordered_map<string , SptrClientLocal> client[X_MSG_IM_CLIENT_MGR_LOCK_COUNT]; 
	mutex lock[X_MSG_IM_CLIENT_MGR_LOCK_COUNT];
	function<void(SptrClientLocal client, function<void(int ret, const string& desc)> cb)> addUsrClientCb; 
	static XmsgImClientLocalMgr* inst;
	XmsgImClientLocalMgr();
	virtual ~XmsgImClientLocalMgr();
};

#endif 

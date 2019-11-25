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

#ifndef GM_XMSGIMGROUPMEMBERULOCAL_H_
#define GM_XMSGIMGROUPMEMBERULOCAL_H_

#include "XmsgImGroupMemberU.h"
#include "../coll/XmsgImGroupMemberColl.h"
#include "../coll/XmsgImGroupUsrClientColl.h"

class XmsgImGroupMemberG;
class XmsgImClientLocal;
class XmsgImClient;

class XmsgImGroupMemberUlocal: public XmsgImGroupMemberU
{
public:
	unordered_map<string , shared_ptr<XmsgImGroupMemberColl> > group; 
	unordered_map<string , weak_ptr<XmsgImClientLocal>> client; 
public:
	void pubNotice2onlineClient(const string& slot , shared_ptr<Message> notice, shared_ptr<XmsgImClientLocal> ignore = nullptr); 
	void pubNotice2offlineClient(shared_ptr<Message> notice, shared_ptr<unordered_set<string >> ignore = nullptr); 
	void pubNotice2offlineClient(shared_ptr<Message> notice, shared_ptr<XmsgImClient> ignore, bool persistence); 
	void pubXmsgImGroupSyncNotice(shared_ptr<XmsgImGroupMemberG> g, shared_ptr<XmsgImGroupSyncNotice> notice, shared_ptr<XmsgImClient> ignore = nullptr); 
public:
	void addClient(shared_ptr<XmsgImClientLocal> client); 
	shared_ptr<XmsgImGroupMemberColl> inGroupInfo(const string& gcgt); 
	void clientLost(const string& plat, const string& did); 
	string toString();
	XmsgImGroupMemberUlocal(SptrCgt cgt);
	virtual ~XmsgImGroupMemberUlocal();
public:
	void initAdd2cache(shared_ptr<XmsgImGroupMemberColl> coll, list<shared_ptr<XmsgImGroupUsrClientColl>>& client); 
};

typedef shared_ptr<XmsgImGroupMemberUlocal> SptrUl;

#endif 

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

#ifndef USR_XMSGIMCLIENTLOCAL_H_
#define USR_XMSGIMCLIENTLOCAL_H_

#include "XmsgImClient.h"
#include "../gm/XmsgImGroupMemberUlocal.h"

class XmsgImGroupMemberUlocal;
class XmsgAp;

class XmsgImClientLocal: public XmsgImClient
{
public:
	bool subGroupEvent; 
public:
	void sendNotice(shared_ptr<XmsgImGroupMemberUlocal> ul, const string& slot , shared_ptr<Message> notice); 
	void evnDisc(SptrXit xit = nullptr ); 
public:
	string toString();
	XmsgImClientLocal(const string& plat, const string& did, shared_ptr<XmsgImGroupMemberUlocal> ul, const string& ccid, shared_ptr<XmsgAp> apLocal);
	XmsgImClientLocal(const string& plat, const string& did, shared_ptr<XmsgImGroupMemberUlocal> ul, const string& ccid, SptrCgt apForeign);
	virtual ~XmsgImClientLocal();
private:
	void sendNotice4local(shared_ptr<Message> notice, shared_ptr<XmsgImGroupMemberUlocal> ul); 
};

typedef shared_ptr<XmsgImClientLocal> SptrClientLocal;

#endif 

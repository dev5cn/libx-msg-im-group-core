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

#ifndef USR_XMSGIMCLIENT_H_
#define USR_XMSGIMCLIENT_H_

#include "../gm/XmsgImGroupMemberU.h"

class XmsgAp;
class XmsgImClient
{
public:
	SptrCgt cgt; 
	string plat; 
	string did; 
	string apCcid; 
	string ccid; 
	shared_ptr<XmsgAp> apLocal; 
	SptrCgt apForeign; 
	weak_ptr<XmsgImGroupMemberU> u; 
public:
	bool equals(SptrCgt other, const string& plat, const string& did); 
	bool equals(shared_ptr<XmsgImClient> other); 
	bool isOnline(); 
	bool isLocalAttach(); 
	string apCgt(); 
	string toString();
	XmsgImClient(const string& plat, const string& did, shared_ptr<XmsgImGroupMemberU> u);
	virtual ~XmsgImClient();
};

typedef shared_ptr<XmsgImClient> SptrClient;

#endif 

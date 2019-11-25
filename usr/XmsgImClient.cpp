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

#include "XmsgImClient.h"
#include "../ne/XmsgAp.h"

XmsgImClient::XmsgImClient(const string& plat, const string& did, shared_ptr<XmsgImGroupMemberU> u)
{
	this->cgt = u->cgt;
	this->plat = plat;
	this->did = did;
	this->u = u;
	this->apLocal = nullptr;
	this->apForeign = nullptr;
}

bool XmsgImClient::isOnline()
{
	return this->apLocal != nullptr || this->apForeign != nullptr;
}

bool XmsgImClient::isLocalAttach()
{
	return this->apLocal != nullptr;
}

string XmsgImClient::apCgt()
{
	if (this->apLocal != nullptr)
	{
		auto usr = this->apLocal->usr.lock();
		return usr == nullptr ? "" : usr->uid;
	}
	return this->apForeign != nullptr ? this->apForeign->toString() : "";
}

bool XmsgImClient::equals(SptrCgt other, const string& plat, const string& did)
{
	if (!this->cgt->isSame(other))
		return false;
	if (this->plat != plat)
		return false;
	if (this->did != did)
		return false;
	return true;
}

bool XmsgImClient::equals(shared_ptr<XmsgImClient> other)
{
	if (!this->isOnline() || !other->isOnline())
		return false;
	if (this->apCcid != other->apCcid)
		return false;
	if (!this->cgt->isSame(other->cgt))
		return false;
	if (this->plat != other->plat)
		return false;
	if (this->did != other->did)
		return false;
	return true;
}

string XmsgImClient::toString()
{
	auto u = this->u.lock();
	string str;
	SPRINTF_STRING(&str, "cgt: %s, plat: %s, did: %s, u: %s, ap-ccid: %s", 
			this->cgt->toString().c_str(),
			this->plat.c_str(),
			this->did.c_str(),
			u == nullptr ? "null" : u->toString().c_str(),
			this->apCcid.c_str())
	return str;
}

XmsgImClient::~XmsgImClient()
{

}


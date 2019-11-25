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

#include "XmsgImClientLocal.h"
#include "XmsgImClientLocalMgr.h"
#include "../gm/XmsgImGroupMemberUlocal.h"
#include "../gm/XmsgImGroupMgr.h"
#include "../ne/XmsgNe.h"
#include "../ne/XmsgAp.h"

XmsgImClientLocal::XmsgImClientLocal(const string& plat, const string& did, shared_ptr<XmsgImGroupMemberUlocal> ul, const string& ccid, shared_ptr<XmsgAp> apLocal) :
		XmsgImClient(plat, did, ul)
{
	this->subGroupEvent = false;
	if (apLocal != nullptr)
	{
		this->apLocal = apLocal;
		this->apCcid = XmsgMisc::getApCcid(apLocal->usr.lock()->uid, ccid);
		this->ccid = ccid;
	}
}

XmsgImClientLocal::XmsgImClientLocal(const string& plat, const string& did, shared_ptr<XmsgImGroupMemberUlocal> ul, const string& ccid, SptrCgt apForeign) :
		XmsgImClient(plat, did, ul)
{
	this->subGroupEvent = false;
	if (apForeign != nullptr)
	{
		this->apForeign = apForeign;
		this->apCcid = XmsgMisc::getApCcid(apForeign->toString(), ccid);
		this->ccid = ccid;
	}
}

void XmsgImClientLocal::sendNotice(shared_ptr<XmsgImGroupMemberUlocal> ul, const string& slot , shared_ptr<Message> notice)
{
	if (!this->isOnline()) 
	{
		LOG_DEBUG("current x-msg-im-client channel is off-line, can not send any more, this: %s, notice: %s", this->toString().c_str(), notice->ShortDebugString().c_str())
		return;
	}
	this->sendNotice4local(notice, ul);
}

void XmsgImClientLocal::sendNotice4local(shared_ptr<Message> notice, SptrUl ul)
{
	SptrOob oob(new list<pair<uchar, string>>());
	oob->push_back(make_pair<>(XSC_TAG_UID, this->ccid));
	this->apLocal->unidirection(notice, oob);
}

void XmsgImClientLocal::evnDisc(SptrXit xit )
{
	if (!this->isOnline())
	{
		LOG_FAULT("it`s a bug, x-msg-im-client not online, this: %s", this->toString().c_str())
	}
	if (XmsgImClientLocalMgr::instance()->remove(this->apCcid) == nullptr)
	{
		LOG_FAULT("it`s a bug, can not found x-msg-im-client: %s", this->toString().c_str())
	}
	SptrUl ul = static_pointer_cast<XmsgImGroupMemberUlocal>(XmsgImGroupMgr::instance()->findUsr(this->cgt));
	if (ul == nullptr)
	{
		LOG_FAULT("it`s a bug, can not found group usr for channel global title: %s", this->toString().c_str())
	} else
	{
		ul->clientLost(this->plat, this->did);
	}
	LOG_DEBUG("have a x-msg-im-client lost from x-msg-ap: %s", this->toString().c_str())
	this->ccid.clear();
	this->apCcid.clear();
	this->subGroupEvent = false;
	this->apForeign = nullptr;
	this->apLocal = nullptr;
}

string XmsgImClientLocal::toString()
{
	string str = XmsgImClient::toString();
	SPRINTF_STRING(&str, ", ccid: %s, subGroupEvent: %s", 
			this->ccid.c_str(),
			this->subGroupEvent ? "true" : "false")
	return str;
}

XmsgImClientLocal::~XmsgImClientLocal()
{

}


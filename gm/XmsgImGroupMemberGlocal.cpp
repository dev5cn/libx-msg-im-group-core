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

#include "XmsgImGroupMgr.h"
#include "XmsgImGroupMemberGlocal.h"
#include "../XmsgImGroupCfg.h"
#include "../ne/XmsgNe.h"

function<void(SptrGroupMsg msg, function<void(int ret, const string& desc)> cb)> XmsgImGroupMemberGlocal::saveMsgCb; 
function<void(SptrGl gl, function<void(ullong msgIdEnd)> cb)> XmsgImGroupMemberGlocal::applyMsgidCb; 

XmsgImGroupMemberGlocal::XmsgImGroupMemberGlocal(shared_ptr<XmsgImGroupColl> dat, ullong msgIdCurrent, ullong msgIdEnd, size_t limit4msgQueue) :
		XmsgImGroupMemberG(dat, limit4msgQueue)
{
	this->isApplyMsgid = false;
	this->msgIdCurrent = msgIdCurrent;
	this->msgIdEnd = msgIdEnd;
}

ullong XmsgImGroupMemberGlocal::getMsgId()
{
	ullong x = ++(this->msgIdCurrent);
	if (x >= this->msgIdEnd) 
	{
		LOG_FAULT("system exception may be can not get new msgid from database, this: %s", this->toString().c_str())
		return 0ULL;
	}
	if (x + (XmsgImGroupCfg::instance()->cfgPb->misc().verapplycount() / 4) < this->msgIdEnd) 
		return x;
	if (this->isApplyMsgid) 
		return x;
	this->isApplyMsgid = true;
	this->applyMsgid([](SptrGl gl, ullong msgIdEnd)
	{
		gl->isApplyMsgid = false; 
		if (msgIdEnd < 1)
		{
			LOG_ERROR("apply message id from database failed, this: %s", gl->toString().c_str())
			return;
		}
		gl->msgIdEnd = msgIdEnd;
		LOG_DEBUG("apply message id from database successful, this: %s", gl->toString().c_str())
	});
	return x;
}

ullong XmsgImGroupMemberGlocal::getMsgIdCurrent()
{
	return this->msgIdCurrent;
}

bool XmsgImGroupMemberGlocal::isEnable(const string& gcgt)
{
	auto it = this->group.find(gcgt);
	return it == this->group.end() ? false : it->second->enable;
}

void XmsgImGroupMemberGlocal::saveMsg(SptrGroupMsg msg, function<void(int ret, const string& desc)> cb)
{
	XmsgImGroupMemberGlocal::saveMsgCb(msg, [cb](int ret, const string& desc)
	{
		cb(ret, desc);
	});
}

void XmsgImGroupMemberGlocal::applyMsgid(function<void(SptrGl gl, ullong msgIdEnd)> cb)
{
	auto gl = static_pointer_cast<XmsgImGroupMemberGlocal>(this->shared_from_this());
	XmsgImGroupMemberGlocal::applyMsgidCb(gl, [gl, cb](ullong msgIdEnd)
	{
		gl->future([gl, cb, msgIdEnd] 
				{
					cb(gl, msgIdEnd);
				});
	});
}

void XmsgImGroupMemberGlocal::setSaveMsgCb(function<void(SptrGroupMsg msg, function<void(int ret, const string& desc)> cb)> cb)
{
	XmsgImGroupMemberGlocal::saveMsgCb = cb;
}

void XmsgImGroupMemberGlocal::setApplyMsgidCb(function<void(SptrGl gl, function<void(ullong msgIdEnd)> cb)> cb)
{
	XmsgImGroupMemberGlocal::applyMsgidCb = cb;
}

string XmsgImGroupMemberGlocal::toString()
{
	string str;
	SPRINTF_STRING(&str, "cgt: %s, msgid-current: %llu, msgid-end: %llu, member-size: %zu", this->cgt->toString().c_str(), this->msgIdCurrent, this->msgIdEnd, this->member.size())
	return str;
}

XmsgImGroupMemberGlocal::~XmsgImGroupMemberGlocal()
{

}


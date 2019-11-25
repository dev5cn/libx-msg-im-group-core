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

#include "XmsgImGroupMemberInfo.h"

XmsgImGroupMemberInfo::XmsgImGroupMemberInfo()
{

}

bool XmsgImGroupMemberInfo::isAdmin()
{
	return this->info->kv().find("admin") != this->info->kv().end();
}

bool XmsgImGroupMemberInfo::isManager()
{
	return this->info->kv().find("manager") != this->info->kv().end();
}

bool XmsgImGroupMemberInfo::havePermission2modifyGroupInfo()
{
	return this->isAdmin() || this->isManager();
}

XmsgImGroupInfoMember* XmsgImGroupMemberInfo::toMemberInfo()
{
	XmsgImGroupInfoMember* info = new XmsgImGroupInfoMember();
	info->set_cgt(this->cgt->toString());
	info->set_enable(this->enable);
	*(info->mutable_info()) = this->info->kv();
	info->set_ver(this->ver);
	info->set_gts(this->gts);
	info->set_uts(this->uts);
	return info;
}

string XmsgImGroupMemberInfo::toString()
{
	string str;
	SPRINTF_STRING(&str, "cgt: %s, enable: %s, ver: %llu, gts: %s, uts: %s, history: %zu, info: %s", 
			this->cgt->toString().c_str(),
			this->enable ? "true" : "false",
			this->ver,
			DateMisc::to_yyyy_mm_dd_hh_mi_ss_ms(this->gts).c_str(),
			DateMisc::to_yyyy_mm_dd_hh_mi_ss_ms(this->uts).c_str(),
			this->history.size(),
			this->info->ShortDebugString().c_str())
	return str;
}

XmsgImGroupMemberInfo::~XmsgImGroupMemberInfo()
{

}


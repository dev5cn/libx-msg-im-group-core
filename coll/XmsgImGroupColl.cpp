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

#include "XmsgImGroupColl.h"

atomic_ullong XmsgImGroupColl::version(1);

XmsgImGroupColl::XmsgImGroupColl()
{

}

XmsgImGroupInfoSelfInfo* XmsgImGroupColl::toSelfInfo()
{
	XmsgImGroupInfoSelfInfo* info = new XmsgImGroupInfoSelfInfo();
	info->set_cgt(this->cgt->toString());
	info->set_enable(this->enable);
	*(info->mutable_info()) = this->info->kv();
	info->set_ver(this->ver);
	info->set_gts(this->gts);
	info->set_uts(this->uts);
	return info;
}

shared_ptr<XmsgImGroupColl> XmsgImGroupColl::fromSelfInfo(const XmsgImGroupInfoSelfInfo* info, SptrCgt gcgt)
{
	shared_ptr<XmsgImGroupColl> coll(new XmsgImGroupColl());
	coll->cgt = gcgt;
	coll->enable = info->enable();
	coll->info.reset(new XmsgKv());
	*(coll->info->mutable_kv()) = info->info();
	coll->ver = info->ver();
	coll->gts = info->gts();
	coll->uts = info->uts();
	return coll;
}

string XmsgImGroupColl::toString()
{
	string str;
	SPRINTF_STRING(&str, "cgt: %s, enable: %s, ver: %llu, gts: %s, uts: %s, info: %s", 
			this->cgt->toString().c_str(),
			this->enable ? "true" : "false",
			this->ver,
			DateMisc::to_yyyy_mm_dd_hh_mi_ss_ms(this->gts).c_str(),
			DateMisc::to_yyyy_mm_dd_hh_mi_ss_ms(this->uts).c_str(),
			this->info->ShortDebugString().c_str())
	return str;
}

XmsgImGroupColl::~XmsgImGroupColl()
{

}


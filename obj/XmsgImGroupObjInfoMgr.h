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

#ifndef OBJ_XMSGIMGROUPOBJINFOMGR_H_
#define OBJ_XMSGIMGROUPOBJINFOMGR_H_

#include "../coll/XmsgImGroupObjInfoColl.h"

class XmsgImGroupObjInfoMgr
{
public:
	void add(shared_ptr<XmsgImGroupObjInfoColl> coll); 
	shared_ptr<XmsgImGroupObjInfoColl> get(SptrCgt gcgt, const string& oid); 
	void dida(ullong now); 
	int size(); 
	static bool loadCb4objInfo(shared_ptr<XmsgImGroupObjInfoColl> coll); 
	static XmsgImGroupObjInfoMgr* instance();
private:
	unordered_map<string , shared_ptr<unordered_map<string , pair<shared_ptr<XmsgImGroupObjInfoColl>, ullong>>>> cached; 
	mutex lock4cached; 
	int size4cached; 
	ullong latest4cleanCache;
	ullong interval4cleanCache;
	static XmsgImGroupObjInfoMgr* inst;
	XmsgImGroupObjInfoMgr();
	virtual ~XmsgImGroupObjInfoMgr();
};

#endif 

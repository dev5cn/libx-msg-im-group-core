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

#ifndef GM_XMSGIMGROUPMEMBER_H_
#define GM_XMSGIMGROUPMEMBER_H_

#include <libx-msg-common-dat-struct-cpp.h>
#include <libx-msg-im-group-pb.h>

#define MSG_PERSISTENCE																true			
#define MSG_NOT_PERSISTENCE															false			

class XmsgImGroupMember: public Actor
{
public:
	SptrCgt cgt; 
public:
	bool isLocal(); 
	bool isForeign(); 
	XmsgImGroupMember(SptrCgt cgt);
	virtual ~XmsgImGroupMember();
private:
	static int assignXscWorker(SptrCgt cgt); 
};

#endif 

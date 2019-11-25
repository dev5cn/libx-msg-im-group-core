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

#ifndef XMSGIMGROUPCFG_H_
#define XMSGIMGROUPCFG_H_

#include <libx-msg-im-group-pb.h>
#include <libx-msg-common-dat-struct-cpp.h>

#define X_MSG_IM_GROUP_MSG_RECEIVED_MODE_RECEIVED 							0x01	
#define X_MSG_IM_GROUP_MSG_RECEIVED_MODE_DB_RECEIVED 						0x02	
#define X_MSG_IM_GROUP_MSG_RECEIVED_MODE_DB_DISK								0x03	

class XmsgImGroupCfg
{
public:
	shared_ptr<XmsgImGroupCfgPb> cfgPb; 
	SptrCgt cgt; 
	SptrCgt hlr; 
public:
	static shared_ptr<XmsgImGroupCfg> instance(); 
	static shared_ptr<XmsgImGroupCfg> load(const char* path); 
	static void setCfg(shared_ptr<XmsgImGroupCfg> cfg); 
	shared_ptr<XscTcpCfg> xscServerCfg(); 
	string toString();
	XmsgImGroupCfg();
	virtual ~XmsgImGroupCfg();
private:
	static shared_ptr<XmsgImGroupCfg> cfg; 
	bool loadLogCfg(XMLElement* root); 
	bool loadXscServerCfg(XMLElement* root); 
	bool loadXmsgNeH2nCfg(XMLElement* root); 
	bool loadXmsgNeN2hCfg(XMLElement* root); 
	bool loadMysqlCfg(XMLElement* root); 
	bool loadMongodbCfg(XMLElement* root); 
	bool loadMiscCfg(XMLElement* root); 
};

#endif 

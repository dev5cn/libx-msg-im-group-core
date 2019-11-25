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

#include "XmsgImClientLocalMgr.h"

XmsgImClientLocalMgr* XmsgImClientLocalMgr::inst = new XmsgImClientLocalMgr();

XmsgImClientLocalMgr::XmsgImClientLocalMgr()
{

}

XmsgImClientLocalMgr* XmsgImClientLocalMgr::instance()
{
	return XmsgImClientLocalMgr::inst;
}

void XmsgImClientLocalMgr::add(SptrClientLocal client)
{
	int index = ((uchar) (client->apCcid.data()[client->apCcid.length() - 1])) % X_MSG_IM_CLIENT_MGR_LOCK_COUNT;
	unique_lock<mutex> lock(this->lock[index]);
	this->client[index][client->apCcid] = client;
}

SptrClientLocal XmsgImClientLocalMgr::find(const string& apCcid)
{
	int index = ((uchar) (apCcid.data()[apCcid.length() - 1])) % X_MSG_IM_CLIENT_MGR_LOCK_COUNT;
	unique_lock<mutex> lock(this->lock[index]);
	auto it = this->client[index].find(apCcid);
	return it == this->client[index].end() ? nullptr : it->second;
}

SptrClientLocal XmsgImClientLocalMgr::remove(const string& apCcid)
{
	int index = ((uchar) (apCcid.data()[apCcid.length() - 1])) % X_MSG_IM_CLIENT_MGR_LOCK_COUNT;
	unique_lock<mutex> lock(this->lock[index]);
	auto it = this->client[index].find(apCcid);
	if (it == this->client[index].end())
		return nullptr;
	auto c = it->second;
	this->client[index].erase(it);
	return c;
}

void XmsgImClientLocalMgr::addUsrClient(SptrClientLocal client, function<void(int ret, const string& desc)> cb)
{
	this->addUsrClientCb(client, cb);
}

void XmsgImClientLocalMgr::setAddUsrClientCb(function<void(SptrClientLocal client, function<void(int ret, const string& desc)> cb)> cb)
{
	this->addUsrClientCb = cb;
}

XmsgImClientLocalMgr::~XmsgImClientLocalMgr()
{

}


#pragma once
#include "liuzianglib.h"
#include "DC_MySQL.h"
#include "DC_ERROR.h"
//Version 2.4
//20170228

namespace DC {

	class DC_UserInfo {
	public:
		DC_UserInfo() {}

		DC_UserInfo(const DC_UserInfo& input) {
			sql_connection = input.sql_connection;
		}

		~DC_UserInfo() {}

	public:
		DC_UserInfo& operator=(const DC_UserInfo& input) {
			sql_connection = input.sql_connection;
			return *this;
		}

		void SetServer(const std::string& inputIP, const std::string& inputusername, const std::string& inputpassword, const std::string& inputDBname, const std::size_t& inputport) {
			sql_connection.SetServer(inputIP, inputusername, inputpassword, inputDBname, inputport);
		}

		bool Connect() {
			return sql_connection.Connect();
		}

		void disconnect() {
			sql_connection.disconnect();
		}

		bool VerConnection() {
			return sql_connection.VerConnection();
		}

		DC_ERROR NewUser(std::string username, std::string password) {
			auto rv = VerUser(username).GetValue();//顺序没有问题，不要改
			if (rv == 1) return DC_ERROR("Return Value", "NewUser:user already exist", 0);
			if (rv == -1) return DC_ERROR("NetWork ERROR", "NewUser:can't send query", -1);
			if (rv != 0) return DC_ERROR("ERROR", "NewUser:unknown error", -2);
			username = GetHash(username);
			password = GetHash(password);
			std::string make_uniqueid;
			while (1) {
				make_uniqueid = STR::toString(randomer(1000000, 99999999));
				auto rv2 = VerUser_ByUniqueID(make_uniqueid);
				if (rv2.GetValue() == 0) break;
				if (rv2.GetValue() == 1) continue;
				if (rv2.GetValue() == -1) return DC_ERROR("NetWork ERROR", "NewUser:can't send query", -1);
				return DC_ERROR("ERROR", "NewUser:unknown error", -2);
			}
			std::string que("insert into dc_userinfo values(\"" + username + "\",\"" + password + "\",\"" + make_uniqueid + "\");");
			if (!sql_connection.Query(que))
				return DC_ERROR("NetWork ERROR", "NewUser:can't send query", -1);
			return DC_ERROR("Return Value", "NewUser:create new user successfully", 1);
		}

		DC_ERROR DelUser(std::string username) {//不会确定用户是否存在
			username = GetHash(username);
			std::string que("delete from dc_userinfo where username=\"" + username + "\";");
			if (!sql_connection.Query(que))
				return DC_ERROR("NetWork ERROR", "DelUser:can't send query", -1);
			return DC_ERROR("Return Value", "DelUser:delete user command has been send successfully", 1);
		}

		DC_ERROR VerUser(std::string username) {
			username = GetHash(username);
			std::string que("select username from dc_userinfo where username=\"" + username + "\";");
			if (!sql_connection.Query(que))
				return DC_ERROR("NetWork ERROR", "VerUser1:can't send query", -1);
			auto rv = sql_connection.GetResult();
			if (rv.empty())
				return DC_ERROR("Return Value", "VerUser1:user doesn't exist", 0);
			return DC_ERROR("Return Value", "VerUser1:user exist", 1);
		}

		DC_ERROR VerUser(std::string username, std::string password) {
			username = GetHash(username);
			password = GetHash(password);
			std::string que("select username from dc_userinfo where username=\"" + username + "\"" + " and password=\"" + password + "\";");
			if (!sql_connection.Query(que))
				return DC_ERROR("NetWork ERROR", "VerUser2:can't send query", -1);
			auto rv = sql_connection.GetResult();
			if (rv.empty())
				return DC_ERROR("Return Value", "VerUser2:no pass", 0);
			return DC_ERROR("Return Value", "VerUser2:pass", 1);
		}

		DC_ERROR EditUser(std::string username, std::string newusername, std::string newpassword) {
			auto rv = VerUser(username);
			if (rv.GetValue() == 0) return DC_ERROR("Return Value", "EditUser:user doesn't exist", 0);
			if (rv.GetValue() == -1) return DC_ERROR("NetWork ERROR", "EditUser:can't send query", -1);
			if (rv.GetValue() != 1) return DC_ERROR("ERROR", "EditUser:unknown error", -2);
			if (username != newusername) {
				rv = VerUser(newusername);//如果多个EditUser线程同时进行，可能会导致出现多个相同用户名，不过这种情况十分罕见，可以在SQL语句中使用if来解决
				if (rv.GetValue() == 1) return DC_ERROR("Return Value", "EditUser:user already exist", 0);
				if (rv.GetValue() == -1) return DC_ERROR("NetWork ERROR", "EditUser:can't send query", -1);
				if (rv.GetValue() != 0) return DC_ERROR("ERROR", "EditUser:unknown error", -2);
			}
			username = GetHash(username);
			newusername = GetHash(newusername);
			newpassword = GetHash(newpassword);
			std::string que("update dc_userinfo set username=\"" + newusername + "\",password=\"" + newpassword + "\" where username=\"" + username + "\";");
			if (!sql_connection.Query(que))
				return DC_ERROR("NetWork ERROR", "EditUser:can't send query", -1);
			return DC_ERROR("Return Value", "EditUser:edit user command has been send successfully", 1);
		}

		DC_ERROR GetUniqueID(std::string username, std::string& uniqueid) {
			username = GetHash(username);
			std::string que("select unique_id from dc_userinfo where username=\"" + username + "\";");
			if (!sql_connection.Query(que))
				return DC_ERROR("NetWork ERROR", "GetUniqueID:can't send query", -1);
			auto rv = sql_connection.GetResult();
			if (rv.empty())
				return DC_ERROR("Return Value", "GetUniqueID:user doesn't exist", 0);
			uniqueid = rv[0];
			return DC_ERROR("Return Value", "GetUniqueID:get uniqueid successfully", 1);
		}

		DC_ERROR VerUser_ByUniqueID(std::string uniqueid) {
			std::string que("select username from dc_userinfo where unique_id=\"" + uniqueid + "\";");
			if (!sql_connection.Query(que))
				return DC_ERROR("NetWork ERROR", "VerUser_ByUniqueID:can't send query", -1);
			auto rv = sql_connection.GetResult();
			if (rv.empty())
				return DC_ERROR("Return Value", "VerUser_ByUniqueID:user doesn't exist", 0);
			return DC_ERROR("Return Value", "VerUser_ByUniqueID:user exist", 1);
		}

	private:
		std::string GetHash(std::string str) {
			md5.reset();
			md5.update(str);
			std::string temp = md5.toString(), temp2;
			md5.reset();
			md5.update(temp);
			temp2 = md5.toString();
			temp2.resize(8);
			return temp + temp2;
		}

	private:
		DC_MySQL sql_connection;
		MD5 md5;
	};

}
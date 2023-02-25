#pragma once

struct XYZABC
{
	double x;
	double y;
	double z;
	double a;
	double b;
	double c;
};

enum OpenFileMode
{
	NC,
	DAT
};

struct VersionUpdateInfor 
{
	std::string versionNumber;
	std::string url;
	std::string updateTime;
	std::string releaseNote;
};
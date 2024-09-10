/* files.h - MaCross application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#ifndef _FILES_H_
#define _FILES_H_

#include <string>

class File
{
public:
	typedef enum {
		READ_ONLY,
		READ_WRITE,
	}FILE_RULE;

	typedef enum {
		BINARY,
		TEXT,
	}FILE_MODE;
public:
	~File();

	size_t ReadData(unsigned char* buffer, size_t buffer_len);
	bool WriteData(unsigned char* data, size_t data_len);

	static File* OpenFile(const char* name, FILE_RULE rule, FILE_MODE mode);
#if defined(WIN32) || defined(WINCE)
	static File* OpenFile(const uchar_t* name, FILE_RULE rule, FILE_MODE mode);
#endif
	static void CloseFile(File* file);

	static ustring GetCurrentDir(void);
private:
	File();
	void* m_file;
};

ustring CreatePath(ustring& path);

#endif/*_FILES_H_*/


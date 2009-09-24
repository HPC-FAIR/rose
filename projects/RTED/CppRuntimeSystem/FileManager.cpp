#include "FileManager.h"

#include <iostream>
#include <sstream>

#include "CppRuntimeSystem.h"

using namespace std;



// -----------------------    FileInfo  --------------------------------------


FileInfo::FileInfo(FileHandle fh) :
    handle(fh),
    name("Invalid"),
    openMode(INVALID_OPEN_MODE)
{
}

FileInfo::FileInfo(FileHandle fp,
                   const std::string & _name,
                   int _openMode,
                   const SourcePosition & pos):
     handle(fp),
     name(_name),
     openMode(_openMode),
     openPos(pos)
{

}


void FileInfo::print(ostream & os) const
{
    os << hex << handle << "\t" << name ;
    os << " Mode:";
    if(openMode & READ)    os << "R";
    if(openMode & WRITE)   os << "W";
    if(openMode & APPEND)  os << "A";
    os << "\t Opened at " << openPos;
}


ostream& operator<< ( ostream &os, const FileInfo & m)
{
    m.print(os);
    return os;
}



// -----------------------    FileInfo2  --------------------------------------


FileInfo2::FileInfo2(FileHandle2 fh) :
      handle(fh),
    name("Invalid"),
    openMode(INVALID_OPEN_MODE)
{
}

FileInfo2::FileInfo2(FileHandle2 fp,
                   const std::string & _name,
                   int _openMode,
                   const SourcePosition & pos):
       handle(fp),
     name(_name),
     openMode(_openMode),
     openPos(pos)
{

}


void FileInfo2::print(ostream & os ) const
{
  os << hex <<   "\t" << name ;
    os << " Mode:";
    if(openMode & READ)    os << "R";
    if(openMode & WRITE)   os << "W";
    if(openMode & APPEND)  os << "A";
    os << "\t Opened at " << openPos;
}


ostream& operator<< ( ostream &os, const FileInfo2 & m)
{
    m.print(os);
    return os;
}

// -----------------------    FileManager  --------------------------------------


void FileManager::openFile(FileHandle handle,
                           const std::string & fileName,
                           OpenMode mode,
                           const SourcePosition & pos)
{
    RuntimeSystem * rs = RuntimeSystem::instance();

    if(handle == NULL)
    {
        //TODO get errno and print better description of why opening failed
        rs->violationHandler(RuntimeViolation::INVALID_FILE_OPEN,
                             "Couldn't open file. Tried to register NULL-handle");
        return;
    }



    FileInfo compareObj (handle);
    if( openFiles.find(compareObj) != openFiles.end() )
    {
        rs->violationHandler(RuntimeViolation::DOUBLE_FILE_OPEN,
                             "Tried to register the same file-handle twice");
        return;
    }


    openFiles.insert( FileInfo(handle,fileName,mode,pos));
}

void FileManager::openFile(FileHandle handle,
                           const std::string & fileName,
                           const std::string & mode_str,
                           const SourcePosition & pos)
{
	unsigned int mode = (unsigned int) INVALID_OPEN_MODE;

	if( mode_str.find( 'r' ) != string::npos )
		mode |= READ;

	if( mode_str.find( 'w' ) != string::npos )
		mode |= WRITE;

	if( mode_str.find( 'a' ) != string::npos )
		mode |= APPEND;

	openFile( handle, fileName, (OpenMode) mode, pos );
}



void FileManager::closeFile(FileHandle handle)
{
    RuntimeSystem * rs = RuntimeSystem::instance();

    FileInfo compareObj(handle);

    set<FileInfo>::iterator iter = openFiles.find(compareObj);
    if( iter == openFiles.end() )
    {
        rs->violationHandler(RuntimeViolation::INVALID_FILE_CLOSE,
                             "Tried to close a non opened File-Handle");
        return;
    }
    openFiles.erase(iter);
}

void FileManager::checkFileAccess(FileHandle handle, bool read)
{
    RuntimeSystem * rs = RuntimeSystem::instance();

    FileInfo compareObj(handle);
    set<FileInfo>::iterator it = openFiles.find(compareObj);
    //Check if file-handle exists
    if( it == openFiles.end() )
    {
        rs->violationHandler(RuntimeViolation::INVALID_FILE_ACCESS,
                             "Tried to access invalid file-handle");
        return;
    }

    // Check if read access is allowed
    if(read && ! (it->getOpenMode() & READ) )
    {
        rs->violationHandler(RuntimeViolation::INVALID_FILE_ACCESS,
                             "Tried read from file which was only opened for writing");
        return;
    }

    // Check if write access is allowed
    if(!read && ! (it->getOpenMode() & WRITE) )
    {
        rs->violationHandler(RuntimeViolation::INVALID_FILE_ACCESS,
                             "Tried to write to file which was only opened for reading");
        return;
    }
}


void FileManager::checkForOpenFiles()
{
    RuntimeSystem * rs = RuntimeSystem::instance();

    // Check if all files were closed
    if( openFiles.size() > 0 )
    {
        stringstream desc;
        print(desc);
        rs->violationHandler(RuntimeViolation::UNCLOSED_FILES,desc.str());
    }

    // Check if all files were closed
    if( openFiles2.size() > 0 )
    {
        stringstream desc;
        print(desc);
        rs->violationHandler(RuntimeViolation::UNCLOSED_FILES,desc.str());
    }

}



void FileManager::print(ostream & os) const
{
    os << endl;
    os << "----------------------------- Filemanager: Open Files -----------------------------" << endl << endl;

    typedef set<FileInfo>::const_iterator FileIter;
    for( FileIter i = openFiles.begin(); i != openFiles.end(); ++i)
        os << *i << endl;



    os << "-----------------------------------------------------------------------------------" << endl;

}

std::ostream& operator<< (std::ostream &os, const FileManager & m)
{
    m.print(os);
    return os;
}
















void FileManager::openFile(FileHandle2 handle,
                           const std::string & fileName,
                           OpenMode mode,
                           const SourcePosition & pos)
{
    RuntimeSystem * rs = RuntimeSystem::instance();

    if(handle == NULL)
    {
        //TODO get errno and print better description of why opening failed
        rs->violationHandler(RuntimeViolation::INVALID_FILE_OPEN,
                             "Couldn't open file. Tried to register NULL-handle");
        return;
    }



    FileInfo2 compareObj (handle);
    if( openFiles2.find(compareObj) != openFiles2.end() )
    {
        rs->violationHandler(RuntimeViolation::DOUBLE_FILE_OPEN,
                             "Tried to register the same file-handle twice");
        return;
    }


    openFiles2.insert( FileInfo2(handle,fileName,mode,pos));
}

void FileManager::openFile(FileHandle2 handle,
                           const std::string & fileName,
                           const std::string & mode_str,
                           const SourcePosition & pos)
{
	unsigned int mode = (unsigned int) INVALID_OPEN_MODE;

	if( mode_str.find( 'r' ) != string::npos )
		mode |= READ;

	if( mode_str.find( 'w' ) != string::npos )
		mode |= WRITE;

	if( mode_str.find( 'a' ) != string::npos )
		mode |= APPEND;

	openFile( handle, fileName, (OpenMode) mode, pos );
}



void FileManager::closeFile(FileHandle2 handle)
{
    RuntimeSystem * rs = RuntimeSystem::instance();

    FileInfo2 compareObj(handle);

    set<FileInfo2>::iterator iter = openFiles2.find(compareObj);
    if( iter == openFiles2.end() )
    {
        rs->violationHandler(RuntimeViolation::INVALID_FILE_CLOSE,
                             "Tried to close a non opened File-Handle");
        return;
    }
    openFiles2.erase(iter);
}

void FileManager::checkFileAccess(FileHandle2 handle, bool read)
{
    RuntimeSystem * rs = RuntimeSystem::instance();

    FileInfo2 compareObj(handle);
    set<FileInfo2>::iterator it = openFiles2.find(compareObj);
    //Check if file-handle exists
    if( it == openFiles2.end() )
    {
        rs->violationHandler(RuntimeViolation::INVALID_FILE_ACCESS,
                             "Tried to access invalid file-handle");
        return;
    }

    // Check if read access is allowed
    if(read && ! (it->getOpenMode() & READ) )
    {
        rs->violationHandler(RuntimeViolation::INVALID_FILE_ACCESS,
                             "Tried read from file which was only opened for writing");
        return;
    }

    // Check if write access is allowed
    if(!read && ! (it->getOpenMode() & WRITE) )
    {
        rs->violationHandler(RuntimeViolation::INVALID_FILE_ACCESS,
                             "Tried to write to file which was only opened for reading");
        return;
    }
}


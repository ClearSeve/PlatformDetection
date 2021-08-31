#ifndef UTILS_FILE_FIND_H_
#define UTILS_FILE_FIND_H_

#include "FileFindData.h"
#include <functional>

namespace utils {

typedef std::function<void(CFileFindData& findData)> FileFindTraverseCallback;

class CFileFind : public CFileFindData
{
public:
    CFileFind(LPCTSTR lpFindPath);
    ~CFileFind();

public:
    BOOL FindFile();
    BOOL FindNext();

private:
    HANDLE m_hFindFile = INVALID_HANDLE_VALUE;
};

} // namespace utils

#endif // UTILS_FILE_FIND_H_
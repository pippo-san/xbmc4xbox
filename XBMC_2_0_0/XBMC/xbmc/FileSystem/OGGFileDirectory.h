#pragma once

#include "ifiledirectory.h"

using namespace DIRECTORY;

namespace DIRECTORY
{
  class COGGFileDirectory : public IFileDirectory
  {
  public:
    COGGFileDirectory(void);
    virtual ~COGGFileDirectory(void);
    virtual bool GetDirectory(const CStdString& strPath, CFileItemList &items);
    virtual bool Exists(const char* strPath);
    virtual bool ContainsFiles(const CStdString& strPath);
  };
};

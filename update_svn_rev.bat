@echo off
rem subwcrev is included in the tortoise svn client: http://tortoisesvn.net/downloads

SET CWD=%~dp0

SET REV_FILE="%CWD%xbmc\xbox\svn_rev.h"
SET SVN_TEMPLATE="%CWD%xbmc\xbox\svn_rev.tmpl"
SET SVN_TEMPLATE_GIT="%CWD%xbmc\xbox\svn_rev_git.tmpl"

IF EXIST %REV_FILE% del %REV_FILE%

SetLocal EnableDelayedExpansion

IF NOT EXIST "%CWD%.git\" (
  ECHO git repository not found, skipping versioning
  GOTO SKIPVERSION
)

rem get commit hash
FOR /F %%I IN ('git rev-parse --short HEAD') DO SET "REV=%%I"

rem get current date
SET REV_DATE=%DATE%

copy %SVN_TEMPLATE_GIT% %REV_FILE%

echo #define SVN_REV   "%REV%" >> %REV_FILE%

echo #define SVN_DATE  "%REV_DATE%" >> %REV_FILE%

echo #endif >> %REV_FILE%

goto :end

:SKIPVERSION

REM Copy the default unknown revision header if it's not a git repository
IF NOT EXIST %REV_FILE% (
   ECHO using default svn revision unknown header
   copy %CWD%xbmc\xbox\svn_rev.unknown %REV_FILE%
)
goto :end

:end
SET REV_FILE=
SET SUBWCREV=
@rem  Icon Plus MS Windows Configuration Script
@echo off

if "%1" == "--help" goto help
if "%1" == "--icc"  goto cnf_icc
if "%1" == "--bcc"  goto cnf_bcc
if "%1" == "--wcc"  goto cnf_wcc
  
:help

  echo Usage:   configure [options]
  echo Options: --help print this help.
  echo          --icc  configure to IBM VisualAge C++
  echo          --bcc  configure to Borland C++
  echo          --wcc  configure to Open Watcom C++
  goto end

:cnf_icc

  echo Configuring...
  copy Source\makefile_icc Source\makefile
  copy Source\docs\makefile_icc Source\docs\makefile
  echo Done.
  goto end

:cnf_bcc

  echo Configuring...
  copy Source\makefile_bcc Source\makefile
  copy Source\docs\makefile_bcc Source\docs\makefile
  echo Done.
  goto end

:cnf_wcc

  echo Configuring...
  copy Source\makefile_wcc Source\makefile
  copy Source\docs\makefile_wcc Source\docs\makefile
  echo Done.
  goto end

:end

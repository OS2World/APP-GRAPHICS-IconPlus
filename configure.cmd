/* Icon Plus OS/2 REXX Configuration Script */

  if RxFuncQuery('SysLoadFuncs') then do
     call RxFuncAdd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
     call SysLoadFuncs
  end

  comps.1.suffix = "icc"
  comps.1.exe    = "icc.exe"
  comps.1.descrp = "IBM VisualAge C++"
  comps.2.suffix = "wcc"
  comps.2.exe    = "wcl386.exe"
  comps.2.descrp = "Open Watcom C++"
  comps.0        = 2
  comps.selected = 0

  files.1 = "Source\makefile"
  files.2 = "Source\docs\makefile"
  files.0 = 2

  parse arg options

  if options == "--help" then do
     say "Usage:   configure [options]"
     say "Options: --help print this help."
     do i = 1 to comps.0
        say copies(" ",8)" --"substr(comps.i.suffix,1,4) ||,
                           " configure to "comps.i.descrp
     end
     exit 1
  end

  do i = 1 to comps.0
     if options == "--"comps.i.suffix then do
        comps.selected = i
        call configure
     end   
  end

  say "The compiler is not specified (try 'configure --help' for more options)."
  say "Attempts to find any..."

  do i = 1 to comps.0
     if SysSearchPath( "PATH", comps.i.exe ) \= "" then do
        comps.selected = i
        say "Found "comps.i.descrp
        call configure
     end
  end

  say "Nothing it is found. Configuration aborted"
  exit 1

configure: procedure expose comps. files.

  sel = comps.selected
  say "Configuring..."
  do i = 1 to files.0 
     "@copy "files.i"_"comps.sel.suffix" "files.i
  end
  say "Done."
  exit 0
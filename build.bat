@REM Make the gcc avaialable.
cd djgpp
call setenv.bat

@REM Navigate to the source folder.
cd ..
cd src
cd imgview

@REM Call the correct build script.
call build.bat

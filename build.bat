@REM Make the gcc avaialable.
cd djgpp
call setenv.bat

@REM Navigate to the source folder.

cd ..
cd src
cd imgview
call build.bat

cd ..
cd palview
call build.bat

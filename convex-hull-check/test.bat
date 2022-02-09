@echo off

for /r %%i in (tests/*) do (
    echo %%~ni
    call main < tests/%%~nxi
    echo:
    echo:
)
::: Properties
	@echo off
	cls
	cd /d "%~dp0"

::: Update SubModules
	git submodule update --remote --merge
	
::: BugFix: Detached Head
	rem git submodule foreach git checkout master 
	rem git submodule foreach git pull origin master

::: Terminate
	pause
	exit /b 0

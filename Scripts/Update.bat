::: Properties
	@echo off
	cls
	cd /d "%~dp0"

::: Update SubTree
	git subtree pull --prefix=3rd-Party/entt https://github.com/OmniVortexStudios/entt.git master --squash
	git subtree pull --prefix=3rd-Party/glm https://github.com/OmniVortexStudios/glm.git master --squash
	git subtree pull --prefix=3rd-Party/imgui https://github.com/OmniVortexStudios/imgui.git master --squash
	git subtree pull --prefix=3rd-Party/stb https://github.com/OmniVortexStudios/stb.git master --squash
	
::: Update Main
	git pull origin master
	
::: BugFix: Detached Head
	rem git submodule foreach git checkout master 
	rem git submodule foreach git pull origin master

::: Terminate
	pause
	exit /b 0

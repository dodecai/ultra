### Properties
    Clear
	Set-Location "$PSScriptRoot\..\"
	
### Update SubTree
	git subtree pull --prefix=3rd-Party/assimp https://github.com/OmniVortexStudios/assimp.git master --squash
	git subtree pull --prefix=3rd-Party/entt https://github.com/OmniVortexStudios/entt.git master --squash
	git subtree pull --prefix=3rd-Party/freetype https://github.com/OmniVortexStudios/freetype.git master --squash
	git subtree pull --prefix=3rd-Party/glm https://github.com/OmniVortexStudios/glm.git master --squash
	git subtree pull --prefix=3rd-Party/imgui https://github.com/OmniVortexStudios/imgui.git docking --squash
	git subtree pull --prefix=3rd-Party/msdf-atlas-gen https://github.com/OmniVortexStudios/msdf-atlas-gen.git master --squash
	git subtree pull --prefix=3rd-Party/msdfgen https://github.com/OmniVortexStudios/msdfgen.git master --squash
	git subtree pull --prefix=3rd-Party/stb https://github.com/OmniVortexStudios/stb.git master --squash
	git subtree pull --prefix=3rd-Party/yaml-cpp https://github.com/OmniVortexStudios/yaml-cpp.git master --squash
	
### Update MainTree
	git pull origin master
	
### Terminate
	pause
	exit 0

### BugFix: Detached Head
	#git submodule foreach git checkout master 
	#git submodule foreach git pull origin master

### Add SubTree
	#git subtree add --prefix=3rd-Party/x https://github.com/<organization>/<repository>.git <branch> --squash
	# ... apply changes ...
	#git commit -am "Added subtree ..."
	
### Remove SubModule
	#git submodule deinit -f -- 3rd-Party\x
	#Remove-Item .git\modules\3rd-Party\x -Force -Recurse
	#git rm -f 3rd-Party\x
	#git commit -am "Removed submodule ..."
	
### Remove SubTree
	#Remove-Item 3rd-Party/x -Force -Recurse
	#git rm -r --cached 3rd-Party/x
	#git commit -am "Removed subtree ..."

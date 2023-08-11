-- Visual Studio: Solution Items (multiple folders)
-- Source: https://github.com/premake/premake-core/issues/1061
require("vstudio")
premake.api.register {
	name = "solutionitems",
	scope = "workspace",
	kind = "table",
}

premake.override(premake.vstudio.sln2005, "projects", function(base, wks)
    if wks.solutionitems and #wks.solutionitems > 0 then
        local function GetSolutionItems(wks, items, root)
			premake.push('Project("{2150E333-8FDC-42A3-9474-1A3956D46DE8}") = "'..root..'", "'..root..'", "{' .. os.uuid("Solution:"..wks.name..":"..root) .. '}"')
	        premake.push("ProjectSection(SolutionItems) = preProject")

            local directories = {}
            for key, value in pairs(items) do
                if type(value) == "table" then
                    if string.find(key, "*") then
                        local root = key:gsub("[%*/]", "")
                        --table.insert(directories, root)
                        for _, name in pairs(os.matchdirs(key)) do
                            print(name)
                            --table.insert(directories[root], name)
                        end
                    else
                        table.insert(directories, key)
                    end
		        elseif type(value) == "string" then
                    for _, name in ipairs(os.matchfiles(value)) do
				        name = path.rebase(name, ".", wks.location)
				        premake.w(name.." = "..name)
			        end
		        end
	        end
            
	        premake.pop("EndProjectSection")
	        premake.pop("EndProject")

            if directories ~= "" then
				for _, name in ipairs(directories) do
					GetSolutionItems(wks, items[name], name)
				end
            end
        end

        GetSolutionItems(wks, wks.solutionitems, "Solution")
    end
		
	base(wks)
end)

premake.override(premake.vstudio.sln2005, "nestedProjects", function(base, wks)
    premake.push("GlobalSection(NestedProjects) = preSolution")
	local tr = premake.workspace.grouptree(wks)
	if premake.tree.hasbranches(tr) then
		premake.tree.traverse(tr, {
			onnode = function(n)
				if n.parent.uuid then
					premake.w("{%s} = {%s}", (n.project or n).uuid, n.parent.uuid)
				end
			end
		})
	end

    local function GetNestaedSolutionItems(wks, items, parent)
        local parent_uuid = os.uuid("Solution:"..wks.name..":"..parent)

	    for key, value in pairs(items) do
		    if type(value) == "table" then
                if parent ~= "Solution" then
			        project_uuid = os.uuid("Solution:"..wks.name..":"..key)
			        premake.w("{%s} = {%s}", project_uuid, parent_uuid)
                end
			    GetNestaedSolutionItems(wks, value, key)
	        end
        end
    end
    
	GetNestaedSolutionItems(wks, wks.solutionitems, "Solution")
	premake.pop("EndGlobalSection")
end)

-- Visual Studio: C/C++ 20 Conformance Mode Support (was fixed since 5.0-beta.1)
----require('vstudio')
----premake.api.register {
----  name = "confromancemode",
----  scope = "project",
----  kind = "boolean",
----}
----premake.override(premake.vstudio.vc2010.elements, "clCompile", function(base, prj)
----	local m = premake.vstudio.vc2010
----	local calls = base(prj)
----
----	if premake.project.iscpp(prj) then
----		if prj.confromancemode then
----			table.insertafter(calls, premake.xmlDeclaration,  function()
----				premake.w('<ConformanceMode>true</ConformanceMode>')
----			end)
----		else
----			table.insertafter(calls, premake.xmlDeclaration,  function()
----				premake.w('<ConformanceMode>false</ConformanceMode>')
----			end)
----		end
----	end
----	return calls
----end)

-- Visual Studio: C 17 Dialect Support (was fixed since 5.0-beta.2)
----require('vstudio')
----premake.api.register {
----  name = "cdialectx",
----  scope = "project",
----  kind = "string",
----}
----premake.override(premake.vstudio.vc2010.elements, "clCompile", function(base, prj)
----	local m = premake.vstudio.vc2010
----	local calls = base(prj)
----
----	if premake.project.iscpp(prj) then
----		if prj.cdialectx then
----			table.insertafter(calls, premake.xmlDeclaration,  function()
----				premake.w('<LanguageStandard_C>stdc17</LanguageStandard_C>')
----			end)
----		end
----	end
----	return calls
----end)

-- Visual Studio: C++ 20 Dialect Support (was fixed since 5.0-beta.2)
----require('vstudio')
----premake.api.register {
----  name = "cppdialectx",
----  scope = "project",
----  kind = "string",
----}
----premake.override(premake.vstudio.vc2010.elements, "clCompile", function(base, prj)
----	local m = premake.vstudio.vc2010
----	local calls = base(prj)
----
----	if premake.project.iscpp(prj) then
----		if prj.cdialectx then
----			table.insertafter(calls, premake.xmlDeclaration,  function()
----				premake.w('<LanguageStandard>stdcpp20</LanguageStandard>')
----			end)
----		end
----	end
----	return calls
----end)

-- Visual Studio: C++ 20 Modules Support
table.insert(premake.vstudio.vc2010.categories.ClCompile.extensions, ".cppm")
table.insert(premake.vstudio.vc2010.categories.ClCompile.extensions, ".cxx")
table.insert(premake.vstudio.vc2010.categories.ClCompile.extensions, ".ixx")

require('vstudio')
premake.api.register {
  name = "cppmodules",
  scope = "project",
  kind = "boolean",
}
premake.override(premake.vstudio.vc2010.elements, "clCompile", function(base, prj)
	local m = premake.vstudio.vc2010
	local calls = base(prj)

	if premake.project.iscpp(prj) then
		if prj.cppmodules then
			table.insertafter(calls, premake.xmlDeclaration,  function()
				premake.w('<EnableModules>true</EnableModules>')
			end)
		else
			table.insertafter(calls, premake.xmlDeclaration,  function()
				premake.w('<EnableModules>false</EnableModules>')
			end)
		end
		base(prj)
	end
	return calls
end)

-- Visual Studio: Build STL Modules C++ 23
require('vstudio')
premake.api.register {
  name = "buildstlmodules",
  scope = "project",
  kind = "boolean",
}
premake.override(premake.vstudio.vc2010.elements, "clCompile", function(base, prj)
	local m = premake.vstudio.vc2010
	local calls = base(prj)

	if premake.project.iscpp(prj) then
		if prj.cppmodules then
			table.insertafter(calls, premake.xmlDeclaration,  function()
				premake.w('<BuildStlModules>true</BuildStlModules>')
			end)
		else
			table.insertafter(calls, premake.xmlDeclaration,  function()
				premake.w('<BuildStlModules>false</BuildStlModules>')
			end)
		end
		base(prj)
	end
	return calls
end)
